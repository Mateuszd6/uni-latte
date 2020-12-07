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
warn(mm line, char* fmt, ...) // line = 0 means skip line number (generic warning)
{
    va_list args;
    fflush(stdout);

    if (line > 0)
        fprintf(stderr, "%s:%ld: warning: ", myfilename, line);
    else
        fprintf(stderr, "%s: warning: ", myfilename);

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

typedef struct node_lnum node_lnum;
struct node_lnum
{
    umm ptr_val;
    mm lnum;
};

static node_lnum* node_lnums = 0;

extern void*
alloc_ast_node(size_t size)
{
    void* mem = malloc(size);
    if (!mem) fatal("Out of memory");

    node_lnum toadd = { .ptr_val = (umm)mem, .lnum = yy_mylinenumber + 1 };
    array_push(node_lnums, toadd);
    return mem;
}

extern i32
get_lnum(void* ast_node)
{
    umm val = (umm)ast_node;
    mm nnode_lnums = array_size(node_lnums);
    mm i = 0;
    for (; i < nnode_lnums; ++i)
    {
        if (node_lnums[i].ptr_val == val)
            return (i32)node_lnums[i].lnum;
    }

    return -1; // When used in report function, lnum won't be provided
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
