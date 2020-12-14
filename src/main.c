//
// TODO: Right now, it is possible to write to "self" variable in the memfunc
// TODO: Cleanup process_expr and try to remove is_lvalue from there
// TODO: Maybe the first param of IR_PUSH should be entire expression, not just .val
// TODO: Different type for string constant, b/c when we lose type info,
//       assmeber wouldn't know what to do (like PARAM c3 is not const 3, but 3rd string)
// TODO: Don't allow division by 0!!!!
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

// TODO: Move below all includes
static FILE* asm_dest;
#if DUMP_IR
static FILE* ir_dest;
#endif

#include "misc.c"
#include "symtab.c"
#include "frontend.c"

//
// Used to get a filename when error reporting in parser in extern parser
//
char* myfilename = 0;

// Returned string is HEAP ALLOCATED.
static char*
repl_extension(char* in_fname, char* expected_ext, char* dest_ext)
{
    char* retval = malloc(strlen(in_fname) + strlen(dest_ext) + 1);
    retval[0] = 0;
    strcat(retval, in_fname);

    // If input filename ends with desired extension (which it should) replace
    // it with dest_ext. Otherwise instead of just bailing out just append
    // desired extension to the filename.
    char* ext = strrchr(retval, '.');
    if (ext && strcmp(ext, expected_ext) == 0)
        *ext = 0;

    strcat(retval, dest_ext);
    return retval;
}

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

    asm_dest = fopen(repl_extension(argv[1], ".lat", ".s"), "w");
#if DUMP_IR
    ir_dest = fopen(repl_extension(argv[1], ".lat", ".ir"), "w");
#endif

    define_primitives();
    add_classes(parse_tree);
    add_global_funcs(parse_tree);
    add_class_members_and_local_funcs(parse_tree); // TODO: Do it when classes are processed?

    if (has_error)
        no_recover();

    check_global_funcs(parse_tree);
    check_class_funcs(parse_tree);

    fclose(asm_dest);
#if DUMP_IR
    fclose(ir_dest);
#endif

    if (has_error)
        no_recover();

    accept_input();
    return 0;
}
