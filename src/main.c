//
// TODO: Right now, it is possible to write to "self" variable in the memfunc
// TODO: Cleanup process_expr and try to remove is_lvalue from there
// TODO: Maybe the first param of IR_PUSH should be entire expression, not just .val
//

// Disable leak checking for asan in debug mode
#ifdef DEBUG
extern char const* __asan_default_options(void);
extern char const* __asan_default_options() { return "detect_leaks=0"; }
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_STATIC
#define ARRAY_IMPLEMENTATION
#include "lib/array.h"

#define HASHMAP_STATIC
#define HASHMAP_IMPLEMENTATION
#include "lib/hashmap.h"

#include "absyn.h"
#include "frontend.h"
#include "misc.h"
#include "parser.h"
#include "symtab.h"

#include "misc.c"
#include "symtab.c"
#include "frontend.c"

//
// Used to get a filename when error reporting in parser in extern parser
//
char* myfilename = 0;

static Program
parse_file(char* fname)
{
    char* in_name = fname;
    myfilename = in_name;

    FILE* input = fopen(in_name, "r");
    if (!input)
        fatal("Could not open file %s for reading", in_name);

    lnumtab_reserve(g_lnumtab, 128);
    Program parse_tree = pProgram(input);
    fclose(input);

    return parse_tree;
}

static void
usage(char* argv0)
{
    fprintf(stderr, "ERROR\nUsage: %s FILE\n", argv0);
    exit(1);
}

int
main(int argc, char** argv)
{
    if (argc != 2)
        usage(argv[0]);

    Program parse_tree = parse_file(argv[1]);
    if (!parse_tree)
        no_recover();

    define_primitives();
    add_classes(parse_tree);
    add_global_funcs(parse_tree);
    add_class_members_and_local_funcs(parse_tree);

    if (has_error)
        no_recover();

    check_global_funcs(parse_tree);
    check_class_funcs(parse_tree);

    if (has_error)
        no_recover();

    accept_input();
    return 0;
}
