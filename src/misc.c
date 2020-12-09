// Common definitions, type aliases and utility functions

#include "misc.h"

#include <limits.h>

static i32 has_error = 0;

static int
qsort_strcmp(void const* lhs_p, void const* rhs_p)
{
    char const* lhs = *(char const* const*)(lhs_p);
    char const* rhs = *(char const* const*)(rhs_p);

    return strcmp(lhs, rhs);
}

extern void
accept_input(void)
{
    fflush(stdout);
    fprintf(stderr, "OK\n");
}

NOINLINE PRINTF_FUNC(1, 2)
extern void
fatal(char* fmt, ...)
{
    va_list args;
    fflush(stdout);

    if (!has_error)
    {
        fprintf(stderr, "ERROR\n");
        has_error = 1;
    }

    fputs("fatal: ", stderr);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(2);
}

NOINLINE PRINTF_FUNC(2, 3)
extern void
error(mm line, char* fmt, ...) // line = 0 means skip line number (generic error)
{
    va_list args;
    fflush(stdout);

    if (!has_error)
    {
        fprintf(stderr, "ERROR\n");
        has_error = 1;
    }

    if (line > 0)
        fprintf(stderr, "%s:%ld: error: ", myfilename, line);
    else
        fprintf(stderr, "%s: error: ", myfilename);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

NOINLINE PRINTF_FUNC(2, 3)
extern void
note(mm line, char* fmt, ...) // line = 0 means skip line number (generic note)
{
    va_list args;
    fflush(stdout);

    if (line > 0)
        fprintf(stderr, "%s:%ld: note: ", myfilename, line);
    else
        fprintf(stderr, "%s: note: ", myfilename);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

extern void
no_recover(void)
{
    // Should be called after reporting an error(), but just in case:
    if (!has_error)
    {
        fprintf(stderr, "ERROR\n");
        has_error = 1;
    }

    exit(1);
}

//
// Line numbers:
//

extern int yy_mylinenumber; // Defined by Bison parser / Flex lexer

HASHMAP_DECLARE(lnumtab, umm, i32);
#define lnumtab_insert(HM, K, V) lnumtab_insert_((void**)&(HM), K, V)
#define lnumtab_reserve(HM, N) lnumtab_reserve_((void**)&(HM), N)

HASHMAP_DEFINE(lnumtab, umm, i32, HASHMAP_DEFAULT_COMPARE, HASHMAP_DEFAULT_HASH);
static lnumtab_kvp* g_lnumtab = 0;

extern void*
alloc_ast_node(size_t size)
{
    void* mem = malloc(size);
    if (!mem) fatal("Out of memory");

    lnumtab_insert(g_lnumtab, (umm)mem, yy_mylinenumber + 1);
    return mem;
}

extern i32
get_lnum(void* ast_node)
{
    umm val = (umm)ast_node;
    mm idx = lnumtab_findi(g_lnumtab, val);

    return idx >= 0 ? g_lnumtab[idx].value : -1;
}

//
// Other
//

extern int
overflows_32bit(mm val)
{
    mm i32_min = (mm)INT_MIN;
    mm i32_max = (mm)INT_MAX;

    return !(i32_min <= val && val <= i32_max);
}
