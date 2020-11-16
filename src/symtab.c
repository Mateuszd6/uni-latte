#include "symtab.h"

#define HASHMAP_IMPLEMENTATION
#define HASHMAP_STATIC
#include "hashmap.h"

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

static void
add_primitive_types(void) // TODO: Rename add -> define?
{
    assert(array_size(g_symtab) == 0); // Primitive types are first that we add

    d_type builtin_type = { .lnum = 0, .is_primitive = 1 };
    array_push(g_types, builtin_type);
    array_push(g_types, builtin_type);
    array_push(g_types, builtin_type);

    char* names[] = {"void", "int", "boolean", "string"};
    for (mm i = 0; i < COUNT_OF(names); ++i)
    {
        symbol s = { .ptr = 0, .type = S_TYPE, .id = (i32)i };
        symbol_push(names[i], s);
    }
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
            .lnum = (i32)get_lnum(t->u.cldef_.clprops_),
            .is_primitive = 0
        };
        array_push(g_types, new_class);

        symbol s = {
            .ptr = t,
            .type = S_TYPE,
            .id = (i32)(array_size(g_types) - 1)
        };

        b32 shadows = symbol_push(t->u.cldef_.ident_, s);
        if (UNLIKELY(shadows))
        {
            // TODO: Make sure there are no symbols for func/vars yet!
            symbol prev_sym = get_shadowed_symbol(t->u.cldef_.ident_);
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

                // TODO: Pop this from the stack, to avoid hardcore issues
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
        assert(retval_type->kind == is_TCls && "Array return types are not supported"); // TODO

        i32 type_id = symbol_resolve_type(retval_type->u.tcls_.ident_, retval_type);
        i32* arg_type_ids = 0;
        LIST_FOREACH(arg_it, t->u.fndef_, listarg_)
        {
            Arg a = arg_it->arg_;
            assert(a->kind == is_Ar);

            Ident aname = a->u.ar_.ident_;
            Type atype = a->u.ar_.type_;

            assert(atype->kind == is_TCls && "Array func params are not supported"); // TODO
            // note((i32)get_lnum(a), "There is an arg: \"%s\"", aname); // TODO
            i32 arg_type_id = symbol_resolve_type(atype->u.tcls_.ident_, atype);
            if (arg_type_id == TYPEID_NOTFOUND)
                arg_type_id = TYPEID_INT;
            array_push(arg_type_ids, arg_type_id);
        }

        d_func f = {
            .lnum = (i32)get_lnum(t->u.fndef_.type_),
            .ret_type_id = type_id,
            .num_args = (i32)array_size(arg_type_ids),
            .arg_type_ids = arg_type_ids,
        };

        array_push(g_funcs, f);

        symbol s = {
            .ptr = t,
            .type = S_FUN,
            .id = (i32)(array_size(g_funcs) - 1)
        };

        b32 shadows = symbol_push(t->u.fndef_.ident_, s);
        if (UNLIKELY(shadows))
        {
            symbol prev_sym = get_shadowed_symbol(t->u.fndef_.ident_);
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

                    // TODO: Pop this from the stack?
                }
                else
                {
                    error(f.lnum, "Cannot name a function after a builtin type \"%s\"",
                          t->u.fndef_.ident_);

                    // TODO: Pop this from the stack, to avoid hardcore issues
                }
            }
        }
    }
}

static inline i32
make_func(TopDef fun_def)
{
    assert(fun_def->kind == is_FnDef);

    d_func f;
    f.lnum = (i32)get_lnum(fun_def->u.fndef_.type_);
    f.ret_type_id = 0; // TODO
    f.num_args = 0; // TODO

    array_push(g_funcs, f);

    return 0;
}

// Assumes that there is a shadowed symbol (stack is of size at least 2)
static inline symbol
get_shadowed_symbol(char* name)
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
        error(get_lnum(node), "use of undeclared identifier \"%s\"", name); // TODO: LNUM
        symbol dummy_sym = { .type = S_NONE };
        return dummy_sym;
    }

    symbol* symbols = stack->symbols;
    assert(symbols); // If there are no symbols, entire node should be deleted

    return symbols[array_size(symbols) - 1];
}

static inline i32
symbol_resolve_type(char* name, void* node)
{
    symbol sym = symbol_get(name, node);
    if (LIKELY(sym.type == S_TYPE))
    {
        return sym.id;
    }
    else
    {
        // TODO: Think about it, b/c maybe it is ok for var/func name to
        //       coexists with a type name.
        switch (sym.type) {
        case S_FUN:
        {
            error(get_lnum(node), "\"%s\" resolved to a function, when type was expected", name);
            note(g_funcs[sym.id].lnum, "Function \"%s\" defined here", name);
        } break;
        case S_VAR:
        {
            error(get_lnum(node), "\"%s\" resolved to a function, when type was expected", name);
            note(g_funcs[sym.id].lnum, "Function \"%s\" defined here", name);
        } break;

        case  S_TYPE:
        case S_NONE:
        {
        } break;
        }
    }

    return TYPEID_NOTFOUND;
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

    // TODO: remove if empty array!
}
