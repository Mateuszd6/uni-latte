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
    if (!parse_tree) // TODO? Error _ALWAYS_ reported in the pProgram func?
        exit(1);

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
    accept_input();

    LIST_FOREACH(it, parse_tree->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        assert(t->kind == is_FnDef);
        warn(get_lnum(t->u.fndef_.type_),
             "There is a declared function called: \"%s\"",
             t->u.fndef_.ident_);
    }

    return 0;
}
