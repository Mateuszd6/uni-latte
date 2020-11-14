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

symboltab_kvp* g_symtab = 0;

static inline symbol
symbol_get(char* name)
{
    symbol_stack* qptr = symboltab_findp(g_symtab, name);
    if (!qptr)
    {
        // TODO: Error gently (could not find symbol).
        error(0, "use of undeclared identifier \"%s\".", name);
    }

    symbol* symbols = qptr->symbols;
    assert(symbols); // If there are no symbols, entire node should be deleted

    return symbols[array_size(symbols) - 1];
}

static inline void
symbol_push(char* name, symbol s)
{
    symbol_stack* qptr = symboltab_findp(g_symtab, name);
    if (!qptr)
    {
        symbol_stack q;
        q.symbols = 0;
        array_push(q.symbols, s);
        symboltab_insert(g_symtab, name, q);
    }
    else // Shadowing an existing symbol
    {
        array_push(qptr->symbols, s);
    }

}

// Assumes that the symbol is already pushed
static inline void
symbol_pop(char* name)
{
    symbol_stack* qptr = symboltab_findp(g_symtab, name);
    assert(qptr);

    array_pop(qptr->symbols);
}
