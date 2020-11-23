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
static d_func* g_funcs = 0;
static d_type* g_types = 0;
static string_const* g_str_consts = 0;

static void
add_primitive_types(void) // TODO: Rename add -> define? TODO: This also adds other things!!
{
    assert(array_size(g_symtab) == 0); // Primitive types are first that we add

    char* names[] = {"void", "int", "boolean", "string"};
    d_type builtin_types[COUNT_OF(names)];

    for (mm i = 0; i < COUNT_OF(names); ++i)
    {
        d_type t = {.name = names[i], .lnum = 0, .is_primitive = 1};
        builtin_types[i] = t;
    }

    array_pushn(g_types, builtin_types, 4);
    for (mm i = 0; i < COUNT_OF(names); ++i)
    {
        symbol s = { .type = S_TYPE, .id = (i32)i };
        symbol_push(names[i], s);
    }

    string_const strc;
    strc.data = malloc(1);
    strc.data[0] = 0;
    strc.len = 1;

    assert(array_size(g_str_consts) == EMPTY_STRING_CONSTANT_ID);
    array_push(g_str_consts, strc);
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
    assert(array_size(g_funcs) == 0); // Global funcs are first that we add
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

        array_push(g_funcs, f);

        symbol s = {
            .type = S_FUN,
            .id = (i32)(array_size(g_funcs) - 1)
        };

        b32 shadows = symbol_push(t->u.fndef_.ident_, s);
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
    }
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
symbol_get(char* name, void* node)
{
    symbol_stack* stack = symboltab_findp(g_symtab, name);
    if (!stack)
    {
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
    symbol sym = symbol_get(name, node);
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
    symbol sym = symbol_get(name, node);
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
    symbol sym = symbol_get(name, node);
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
