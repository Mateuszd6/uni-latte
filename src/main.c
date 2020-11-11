#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "absyn.h"
#include "parser.h"
#include "printer.h"

#define ARRAY_IMPLEMENTATION
#define ARRAY_STATIC
#include "array.h"

#include "misc.c"

extern int yy_mylinenumber;
extern void add_node_lnum(void* ptr);
extern mm get_lnum(void* ast_node); // TODO extern?

typedef struct node_lnum node_lnum;
struct node_lnum
{
    umm ptr_val;
    mm lnum;
};

static node_lnum* node_lnums = NULL;
extern void
add_node_lnum(void* ptr)
{
    node_lnum toadd = { .ptr_val = (umm)ptr, .lnum = yy_mylinenumber + 1 };
    array_push(node_lnums, toadd);
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

static void
usage(void)
{
    printf("usage: Call with one of the following argument combinations:\n");
    printf("\t--help\t\tDisplay this help message.\n");
    printf("\t(no arguments)  Parse stdin verbosely.\n");
    printf("\t(files)\t\tParse content of files verbosely.\n");
    printf("\t-s (files)\tSilent mode. Parse content of files silently.\n");
}

static void
accept_input(void)
{
    fprintf(stderr, "OK\n");
}

// TODO: Move to utils
#define LIST_FOREACH(ITER, HEAD, NEXT_FIELD)                                   \
    for (__typeof(HEAD) ITER = (HEAD);                                         \
         (ITER);                                                               \
         (ITER) = (ITER)-> NEXT_FIELD)

int
main(int argc, char** argv)
{
    FILE* input;
    Program parse_tree;
    // int quiet = 0;
    char* filename = NULL;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-s") == 0)
        {
            // quiet = 1;
            if (argc > 2)
                filename = argv[2];
            else
                input = stdin;
        }
        else
            filename = argv[1];
    }

    if (filename)
    {
        input = fopen(filename, "r");
        if (!input)
        {
            usage();
            exit(1);
        }
    }
    else
        input = stdin;

    array_reserve(node_lnums, 1024);

    parse_tree = pProgram(input);
    if (parse_tree)
    {
        accept_input();
        exit(0);
        LIST_FOREACH(it, parse_tree->u.prog_.listtopdef_, listtopdef_)
        {
            TopDef t = it->topdef_;
            assert(t->kind == is_FnDef);
            printf("There is a declared function called: \"%s\"\n",
                   t->u.fndef_.ident_);
        }

        // printf("\nParse Succesful!\n");
        // if (!quiet)
        // {
            // printf("\n[Abstract Syntax]\n");
            // printf("%s\n\n", showProgram(parse_tree));
            // printf("[Linearized Tree]\n");
            // printf("%s\n\n", printProgram(parse_tree));
        // }

        return 0;
    }


    // TODO: If don't trust bnfc, then possbibly write ERROR here also?
    // Can't lose?
    return 1;
}
