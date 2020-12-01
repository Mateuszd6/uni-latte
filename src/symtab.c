#include "symtab.h"

#define HASHMAP_IMPLEMENTATION
#include "hashmap.h" // As implementation

static inline int
str_cmp(void const* lhs, void const* rhs)
{
    char* const l = *(char* const*)(lhs);
    char* const r = *(char* const*)(rhs);
    return strcmp(l, r) != 0;
}

static inline size_t
str_hash(void const* key, ptrdiff_t len)
{
    ((void)len); // ignore length
    char* const s = *(char* const*)(key);

    return HASHMAP_DEFAULT_HASH(s, (ptrdiff_t)strlen(s));
}

HASHMAP_DEFINE(symboltab, char*, symbol_stack, str_cmp, str_hash);

//
// TODO: Move some stuff to frontend?
//

static symboltab_kvp* g_symtab = 0;
static d_var* g_vars = 0;
static char** g_local_symbols = 0; // symbols to pop, when leaving a scope,
static d_func* g_funcs = 0;        // NULL means break in the scopes
static d_type* g_types = 0;
static string_const* g_str_consts = 0;

static b32
create_func(d_func f, char* name)
{
    symbol s = {
        .type = S_FUN,
        .id = (i32)array_size(g_funcs),
    };

    array_push(g_funcs, f);
    b32 shadows = symbol_push(name, s);
    return shadows;
}

static void
define_primitive_constants(void)
{
    string_const strc;
    strc.data = malloc(1);
    strc.data[0] = 0;
    strc.len = 1;

    assert(array_size(g_str_consts) == EMPTY_STRING_CONSTANT_ID);
    array_push(g_str_consts, strc);
}

static void
define_primitive_types(void)
{
    assert(array_size(g_symtab) == 0); // Primitive types are first that we add

    char* type_names[] = {"void", "int", "boolean", "string", "null"};
    d_type builtin_types[COUNT_OF(type_names)];

    for (mm i = 0; i < COUNT_OF(type_names); ++i)
    {
        d_type t = {.name = type_names[i], .lnum = 0, .is_primitive = 1};
        builtin_types[i] = t;
    }

    array_pushn(g_types, builtin_types, 4);
    for (mm i = 0; i < COUNT_OF(type_names); ++i)
    {
        symbol s = { .type = S_TYPE, .id = (i32)i };
        symbol_push(type_names[i], s);
    }
}

static void
define_primitive_functions(void)
{
    assert(array_size(g_funcs) == 0); // Primitive funcs are first that we add
    char* fun_names[] = { "printInt", "printString", "error", "readInt", "readString" };

    d_func_arg* argsof_printInt = 0;
    d_func_arg a1 = { .name = "_int", .type_id = TYPEID_INT };
    array_push(argsof_printInt, a1);

    d_func_arg* argsof_printString = 0;
    d_func_arg a2 = { .name = "_string", .type_id = TYPEID_STRING };
    array_push(argsof_printString, a2);

    d_func f[] = {
        // void printInt(int)
        {
            .lnum = 0,
            .ret_type_id = TYPEID_VOID,
            .num_args = 1,
            .arg_type_ids = argsof_printInt,
        },
        // void printString(string)
        {
            .lnum = 0,
            .ret_type_id = TYPEID_VOID,
            .num_args = 1,
            .arg_type_ids = argsof_printString,
        },
        // void error()
        {
            .lnum = 0,
            .ret_type_id = TYPEID_VOID,
            .num_args = 0,
            .arg_type_ids = 0,
        },
        // int readInt()
        {
            .lnum = 0,
            .ret_type_id = TYPEID_INT,
            .num_args = 0,
            .arg_type_ids = 0,
        },
        // string readString()
        {
            .lnum = 0,
            .ret_type_id = TYPEID_STRING,
            .num_args = 0,
            .arg_type_ids = 0,
        },
    };

    for (mm i = 0; i < COUNT_OF(f); ++i)
    {
        b32 shadows = create_func(f[i], fun_names[i]);
        assert(!shadows);
    }
}

static void
define_primitives()
{
    define_primitive_constants();
    define_primitive_types();
    define_primitive_functions();
}

static void
add_classes(Program p)
{
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_ClDef)
            continue;

        d_type new_class = {
            .name = t->u.cldef_.ident_,
            .lnum = get_lnum(t->u.cldef_.clprops_),
            .is_primitive = 0
        };
        array_push(g_types, new_class);

        symbol s = {
            .type = S_TYPE,
            .id = (i32)(array_size(g_types) - 1)
        };

        b32 shadows = symbol_push(t->u.cldef_.ident_, s);
        if (UNLIKELY(shadows))
        {
            // TODO: Make sure there are no symbols for func/vars yet!
            symbol prev_sym = symbol_get_shadowed(t->u.cldef_.ident_);
            assert(prev_sym.type == S_TYPE);

            d_type type = g_types[prev_sym.id];
            if (!type.is_primitive)
            {
                error(new_class.lnum, "Redefinition of class \"%s\"", t->u.cldef_.ident_);
                note(type.lnum, "Previously defined here");
            }
            else
            {
                error(new_class.lnum, "Cannot name a class after a builtin type \"%s\"",
                      t->u.cldef_.ident_);

                symbol_pop(t->u.cldef_.ident_);
            }
        }
    }
}

static void
add_global_funcs(Program p)
{
    b32 has_main = 0;
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_FnDef)
            continue;

        Type retval_type = t->u.fndef_.type_;
        Ident type_name;
        b32 is_array;
        switch (retval_type->kind) {
        case is_TCls:
        {
            type_name = retval_type->u.tcls_.ident_;
            is_array = 0;
        } break;
        case is_TArr:
        {
            type_name = retval_type->u.tarr_.ident_;
            is_array = 1;
        } break;
        }

        u32 type_id = symbol_resolve_type(type_name, is_array, retval_type);
        if (type_id == TYPEID_NOTFOUND)
            type_id = TYPEID_INT;

        if (type_id == (TYPEID_VOID | TYPEID_FLAG_ARRAY))
        {
            error(get_lnum(retval_type), "Return type void[] is not allowed");
            type_id = TYPEID_INT;
        }

        d_func_arg* arg_type_ids = 0;
        LIST_FOREACH(arg_it, t->u.fndef_, listarg_)
        {
            Arg a = arg_it->arg_;
            Ident aname = a->u.ar_.ident_;
            Type atype = a->u.ar_.type_;
            Ident arg_type_name;
            b32 arg_is_array;
            switch (atype->kind) {
            case is_TCls:
            {
                arg_type_name = atype->u.tcls_.ident_;
                arg_is_array = 0;
            } break;
            case is_TArr:
            {
                arg_type_name = atype->u.tarr_.ident_;
                arg_is_array = 1;
            } break;
            }

            assert(a->kind == is_Ar);
            u32 arg_type_id = symbol_resolve_type(atype->u.tcls_.ident_, arg_is_array, atype);
            if (arg_type_id == TYPEID_NOTFOUND)
                arg_type_id = TYPEID_INT;

            if (arg_type_id == TYPEID_VOID || arg_type_id == (TYPEID_VOID | TYPEID_FLAG_ARRAY))
            {
                error(get_lnum(a), "Parameter type void%s is not allowed",
                      arg_type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

                arg_type_id = TYPEID_INT;
            }

            d_func_arg arg = { .name = aname, .type_id = arg_type_id };
            array_push(arg_type_ids, arg);
        }

        // Check if all parameter names are unique
        mm n_args = array_size(arg_type_ids);
        if (n_args)
        {
            char** arg_names = 0;
            array_reserve(arg_names, n_args);
            for (mm i = 0; i < n_args; ++i)
                array_push(arg_names, arg_type_ids[i].name);

            qsort(arg_names, (umm)n_args, sizeof(char*), qsort_strcmp);
            for (mm i = 0; i < n_args - 1; ++i)
            {
                if (UNLIKELY(strcmp(arg_names[i], arg_names[i + 1]) == 0))
                {
                    error(get_lnum(t->u.fndef_.listarg_),
                          "Redefinition of parameter \"%s\"",
                          arg_names[i]);

                    break; // TODO: Report all redefinitions, not just first one
                }
            }

            array_free(arg_names);
        }

        d_func f = {
            .lnum = get_lnum(t->u.fndef_.type_),
            .ret_type_id = type_id,
            .num_args = (i32)n_args,
            .arg_type_ids = arg_type_ids,
        };

        b32 shadows = create_func(f, t->u.fndef_.ident_);
        if (UNLIKELY(shadows))
        {
            symbol prev_sym = symbol_get_shadowed(t->u.fndef_.ident_);
            assert(prev_sym.type == S_FUN || prev_sym.type == S_TYPE);

            if (prev_sym.type == S_FUN)
            {
                d_func prev_f = g_funcs[prev_sym.id];
                error(f.lnum, "Redefinition of global function \"%s\"", t->u.fndef_.ident_);
                note(prev_f.lnum, "Previosuly defined here");
            }
            else
            {
                d_type type = g_types[prev_sym.id];
                if (!type.is_primitive)
                {
                    error(f.lnum, "Function \"%s\" cannot be named same as a class", t->u.fndef_.ident_);
                    note(type.lnum, "Class \"%s\" defined here", t->u.fndef_.ident_);

                    symbol_pop(t->u.fndef_.ident_);
                }
                else
                {
                    error(f.lnum, "Cannot name a function after a builtin type \"%s\"", t->u.fndef_.ident_);
                    symbol_pop(t->u.fndef_.ident_);
                }
            }
        }

        if (!has_main && strcmp("main", t->u.fndef_.ident_) == 0)
        {
            if (f.ret_type_id != TYPEID_INT)
                error(f.lnum, "\"main\" function must return an \"int\"");

            if (n_args != 0)
                error(f.lnum, "\"main\" function must not have any arguments");

            has_main = 1;
        }
    }

    if (!has_main)
    {
        error(0, "No \"main\" function defined");
    }
}

static inline i32
push_var(d_var var, char* vname, void* node)
{
    symbol prev_decl = symbol_get(vname, node, 0);
    switch (prev_decl.type) {
    case S_NONE:
    {
        // No shadow, nothing to do
    } break;
    case S_VAR:
    {
        d_var v = g_vars[prev_decl.id];
        if (UNLIKELY(v.block_id) == var.block_id) // Redefinition, not just shadowing
        {
            error(get_lnum(node), "Variable \"%s\" redefined in the same block", vname);
            note(v.lnum, "Variable \"%s\" declared here", vname);

            // TODO: error, don't push
        }
        else
        {
            //
            // TODO: Printing warnings to the console cause writing stuff to the
            // stderr before "OK" is printed, which is incorrect during the spec
            //
#if 0
            warn(get_lnum(node), "Variable \"%s\" shadows an existing variable", vname);
            note(v.lnum, "Variable \"%s\" declared here", vname);
#endif
        }
    } break;
    case S_FUN:
    {
        d_func f = g_funcs[prev_decl.id];
        error(get_lnum(node), "Variable \"%s\" cannot have same name as a function", vname);
        note(f.lnum, "Function \"%s\" declared here", vname);

        // TODO: error, don't push
    } break;

    case S_TYPE:
    {
        if (prev_decl.id <= TYPEID_LAST_BUILTIN_TYPE)
        {
            error(get_lnum(node), "Variable \"%s\" cannot have same name as a builtin type", vname);
        }
        else
        {
            d_type t = g_types[prev_decl.id];
            error(get_lnum(node), "Variable \"%s\" cannot have same name as a class", vname);
            note(t.lnum, "Class \"%s\" declared here", vname);
        }

        // TODO: error, don't push
    } break;
    }

    mm var_id = array_size(g_vars);
    array_push(g_vars, var);

    symbol sym;
    sym.type = S_VAR;
    sym.id = (i32)var_id;
    symbol_push(vname, sym);

    array_push(g_local_symbols, vname);

    return (i32)var_id;
}

// Assumes that there is a shadowed symbol (stack is of size at least 2)
static inline symbol
symbol_get_shadowed(char* name)
{
    symbol* all_symbols = symboltab_findp(g_symtab, name)->symbols;
    assert(array_size(all_symbols) >= 2);

    symbol prev_sym = all_symbols[array_size(all_symbols) - 2];
    return prev_sym;

}

static inline symbol
symbol_get(char* name, void* node, b32 report_error)
{
    symbol_stack* stack = symboltab_findp(g_symtab, name);
    if (!stack)
    {
        if (report_error)
            error(get_lnum(node), "use of undeclared identifier \"%s\"", name);

        symbol dummy_sym = { .type = S_NONE };
        return dummy_sym;
    }

    symbol* symbols = stack->symbols;
    assert(symbols); // If there are no symbols, entire node should be deleted

    return symbols[array_size(symbols) - 1];
}

static inline u32
symbol_resolve_type(char* name, b32 is_array, void* node)
{
    symbol sym = symbol_get(name, node, 1);
    if (LIKELY(sym.type == S_TYPE))
    {
        return (u32)sym.id | (is_array ? TYPEID_FLAG_ARRAY : 0);
    }
    else
    {
        switch (sym.type) {
        case S_NONE:
        {
        } break;
        case S_FUN:
        {
            error(get_lnum(node), "\"%s\" resolved to a function, when type was expected", name);
            note(g_funcs[sym.id].lnum, "Function \"%s\" defined here", name);
        } break;
        case S_VAR:
        {
            error(get_lnum(node), "\"%s\" resolved to a variable, when type was expected", name);
            note(g_vars[sym.id].lnum, "Variable \"%s\" defined here", name);
        } break;
        case S_TYPE: NOTREACHED;
        }
    }

    return TYPEID_NOTFOUND;
}

static inline u32
symbol_resolve_func(char* name, void* node)
{
    symbol sym = symbol_get(name, node, 1);
    if (LIKELY(sym.type == S_FUN))
    {
        return (u32)sym.id;
    }
    else
    {
        switch (sym.type) {
        case S_NONE:
        {
        } break;
        case S_TYPE:
        {
            error(get_lnum(node), "\"%s\" resolved to a type, when function was expected", name);
            if (sym.id > TYPEID_LAST_BUILTIN_TYPE)
                note(g_types[sym.id].lnum, "Type \"%s\" defined here", name);
        } break;
        case S_VAR:
        {
            error(get_lnum(node), "\"%s\" resolved to a variable, when type was expected", name);
            note(g_vars[sym.id].lnum, "Variable \"%s\" defined here", name);
        } break;

        case S_FUN: NOTREACHED;
        }
    }

    return FUNCID_NOTFOUND;
}

static inline u32
symbol_resolve_var(char* name, void* node)
{
    symbol sym = symbol_get(name, node, 1);
    if (LIKELY(sym.type == S_VAR))
    {
        return (u32)sym.id;
    }
    else
    {
        switch (sym.type) {
        case S_NONE:
        {
        } break;
        case S_FUN:
        {
            error(get_lnum(node), "\"%s\" resolved to a function, when variable was expected", name);
            note(g_funcs[sym.id].lnum, "Function \"%s\" defined here", name);
        } break;
        case S_TYPE:
        {
            error(get_lnum(node), "\"%s\" resolved to a type, when variable was expected", name);
            if (sym.id > TYPEID_LAST_BUILTIN_TYPE)
                note(g_types[sym.id].lnum, "Type \"%s\" defined here", name);
        } break;
        case S_VAR: NOTREACHED;
        }
    }

    return VARID_NOTFOUND;
}

// Return 1 if symbol shadows an existing symbol, 0 otherwise
static inline b32
symbol_push(char* name, symbol s)
{
    symbol_stack* stack = symboltab_findp(g_symtab, name);
    if (!stack)
    {
        symbol_stack q;
        q.symbols = 0;
        array_push(q.symbols, s);
        symboltab_insert(g_symtab, name, q);

        return 0;
    }
    else // Shadowing an existing symbol
    {
        array_push(stack->symbols, s);
        return 1;
    }
}

// Assumes that the symbol is already pushed
static inline void
symbol_pop(char* name)
{
    symbol_stack* stack = symboltab_findp(g_symtab, name);
    assert(stack);

    array_pop(stack->symbols);
    if (array_size(stack->symbols) == 0)
    {
        symboltab_delete(g_symtab, name);
    }
}

//
// Expression validating:
//

// TODO
