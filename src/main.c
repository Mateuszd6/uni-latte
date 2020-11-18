// TODO:
// * Support array params by setting up highest bit
// * make a frontend.h/c file
// * typecheck actual code

// TODO: Temprary.
extern char const* __asan_default_options(void);
extern char const* __asan_default_options() { return "detect_leaks=0"; }

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_STATIC
#include "absyn.h"
#include "array.h"
#include "parser.h"
#include "symtab.h"
#include "misc.h"
#include "symtab.h"

#define ARRAY_IMPLEMENTATION
#include "array.h"

#include "symtab.c"
#include "misc.c"

char* myfilename = NULL;

static Program
parse_file(char* fname)
{
    char* in_name = fname;
    myfilename = in_name;

    FILE* input = fopen(in_name, "r");
    if (!input)
        fatal("Could not open file %s for reading.", in_name);

    array_reserve(node_lnums, 1024);
    Program parse_tree = pProgram(input);
    fclose(input);

    return parse_tree;
}

static void
usage(char* argv0)
{
    printf("Usage: %s FILE\n", argv0);
    exit(1);
}

int
main(int argc, char** argv)
{
    if (argc != 2)
        usage(argv[0]);

    Program parse_tree = parse_file(argv[1]);
    if (!parse_tree) no_recover(); // TODO: "ERROR" is printed in the parser code

    add_primitive_types();
    add_classes(parse_tree);
    add_global_funcs(parse_tree);

    if (has_error)
        no_recover();

    accept_input(); // TODO: Resurrect

#if DEBUG
    printf("Types:\n");
    for (mm i = 0; i < array_size(g_types); ++i)
    {
        note(g_types[i].lnum, "(%ld)", i);
    }

    printf("\nFunctions:\n");
    for (mm i = 0; i < array_size(g_funcs); ++i)
    {
        note(g_funcs[i].lnum, "(%ld)", i);
        printf("    Return type: %u%s\n",
               g_funcs[i].ret_type_id & TYPEID_MASK,
               g_funcs[i].ret_type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

        for (mm j = 0; j < array_size(g_funcs[i].arg_type_ids); ++j)
        {
            d_func_arg a = g_funcs[i].arg_type_ids[j];
            printf("    Param: %d%s\n",
                   a.type_id & TYPEID_MASK,
                   a.type_id & TYPEID_FLAG_ARRAY ? "[]" : "");
        }
    }
#endif

    return 0;
}
