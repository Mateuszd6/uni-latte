#include "symtab.h"

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

static symboltab_kvp* g_symtab = 0;
static d_var* g_vars = 0;
static char** g_local_symbols = 0; // symbols to pop, when leaving a scope
static d_func* g_funcs = 0;
static d_type* g_types = 0;
static string_const* g_str_consts = 0;

static b32
create_func(d_func f)
{
    symbol s = {
        .type = S_FUN,
        .id = (i32)array_size(g_funcs),
    };

    array_push(g_funcs, f);
    b32 shadows = symbol_push(f.name, s);
    return shadows;
}

static void
define_primitive_constants(void)
{
    // Empty string
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
        d_type t = {
            .name = type_names[i],
            .members = 0,
            .member_funcs = 0,
            .lnum = 0,
            .is_primitive = 1,
        };

        builtin_types[i] = t;
    }

    array_pushn(g_types, builtin_types, COUNT_OF(type_names));
    for (mm i = 0; i < COUNT_OF(type_names); ++i)
    {
        symbol s = { .type = S_TYPE, .id = (i32)i };
        symbol_push(type_names[i], s);
    }
}

static void
define_primitive_functions(void)
{
    // Primitive funcs are first that we add. And we defined in the header file
    // how many of the we add
    assert(array_size(g_funcs) == 0);

    d_func_arg* argsof_printInt = 0;
    d_func_arg a1 = { .name = "_int", .type_id = TYPEID_INT, .lnum = 0 };
    array_push(argsof_printInt, a1);

    d_func_arg* argsof_printString = 0;
    d_func_arg a2 = { .name = "_string", .type_id = TYPEID_STRING, .lnum = 0 };
    array_push(argsof_printString, a2);

    d_func f[] = {
        // void printInt(int)
        {
            .name = "printInt",
            .lnum = 0,
            .ret_type_id = TYPEID_VOID,
            .num_args = 1,
            .args = argsof_printInt,
        },
        // void printString(string)
        {
            .name = "printString",
            .lnum = 0,
            .ret_type_id = TYPEID_VOID,
            .num_args = 1,
            .args = argsof_printString,
        },
        // void error()
        {
            .name = "error",
            .lnum = 0,
            .ret_type_id = TYPEID_VOID,
            .num_args = 0,
            .args = 0,
        },
        // int readInt()
        {
            .name = "readInt",
            .lnum = 0,
            .ret_type_id = TYPEID_INT,
            .num_args = 0,
            .args = 0,
        },
        // string readString()
        {
            .name = "readString",
            .lnum = 0,
            .ret_type_id = TYPEID_STRING,
            .num_args = 0,
            .args = 0,
        },
    };

    assert(COUNT_OF(f) == FUNCID_LAST_BUILTIN_FUNC + 1);

    for (mm i = 0; i < COUNT_OF(f); ++i)
    {
        b32 shadows = create_func(f[i]);
        (void)shadows;
        assert(!shadows);
    }
}

static void
define_primitives(void)
{
    define_primitive_constants();
    define_primitive_types();
    define_primitive_functions();
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
symbol_check(char* name)
{
    symbol_stack* stack = symboltab_findp(g_symtab, name);
    if (!stack)
    {
        symbol dummy_sym = { .type = S_NONE };
        return dummy_sym;
    }

    symbol* symbols = stack->symbols;
    assert(symbols); // If there are no symbols, entire node should be deleted

    return symbols[array_size(symbols) - 1];
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
            error(get_lnum(node), "\"%s\" resolved to a variable, when function was expected", name);
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
