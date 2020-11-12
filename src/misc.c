// Common definitions, type aliases and utility functions

#include "misc.h"

static i32 error_reported = 0;

extern void
accept_input(void)
{
    fflush(stdout);
    fprintf(stderr, "OK\n");
}

extern void
fatal(char* err)
{
    fflush(stdout);

    if (!error_reported)
    {
        fprintf(stderr, "ERROR\n");
        error_reported = 1;
    }

    fputs("fatal: ", stderr);
    fputs(err, stderr);
    fprintf(stderr, "\n");

    exit(2);
}

PRINTF_FUNC(2, 3)
extern void
error(mm line, char* fmt, ...)
{
    va_list args;
    fflush(stdout);

    if (!error_reported)
    {
        fprintf(stderr, "ERROR\n");
        error_reported = 1;
    }

    fprintf(stderr, "%s:%ld: error: ", "test.lat", line);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

// Line numbers:

extern int yy_mylinenumber; // Defined by Bison parser / Flex lexer

typedef struct node_lnum node_lnum;
struct node_lnum
{
    umm ptr_val;
    mm lnum;
};

static node_lnum* node_lnums = NULL;

extern void*
alloc_ast_node(size_t size)
{
    void* mem = malloc(size);
    if (!mem) fatal("Out of memory");

    node_lnum toadd = { .ptr_val = (umm)mem, .lnum = yy_mylinenumber + 1 };
    array_push(node_lnums, toadd);
    return mem;
}

extern mm // TODO extern?
get_lnum(void* ast_node)
{
    umm val = (umm)ast_node;
    mm nnode_lnums = array_size(node_lnums);
    mm i = 0;
    for (; i < nnode_lnums; ++i)
    {
        if (node_lnums[i].ptr_val == val)
            return node_lnums[i].lnum;
    }

    return -1; // TODO: Should not reach, but check it!
}
