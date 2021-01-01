// TODO: Rename executable to latc_x64
//
// TODO: Remove IRVT for local functions
// TODO: Right now, it is possible to write to "self" variable in the memfunc
//       (which breaks because we lookup "self" wehn calling a local functions!
// TODO: Don't emit bitwise-or (test core017), although you would have to know
//       to create a test that actually exploits that

// Disable leak checking for asan in debug mode
#if DEBUG
extern char const* __asan_default_options(void);
extern char const* __asan_default_options() { return "detect_leaks=0,color=never"; }
#endif

// No registers allocation, when compiling without optimistaion
#ifndef OPTIMIZE
#  ifdef MAX_ALLOCATED_REGS
#    undef MAX_ALLOCATED_REGS
#    define MAX_ALLOCATED_REGS 0
#    warning When optimisations are disabled, registers are not allocated
#  endif // MAX_ALLOCATED_REGS
#endif // OPTIMIZE

#include "gen/asm-prelude.h"

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
#include "codegen.h"
#include "frontend.h"
#include "misc.h"
#include "parser.h"
#include "symtab.h"

#include "misc.c"
#include "symtab.c"
#include "frontend.c"
#include "optim.c"
#include "codegen.c"
#include "tsort.c"

//
// Used to get a filename when error parsing reporting in extern parser
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

static NORETURN void
usage(char* argv0)
{
    fprintf(stderr, "ERROR\nUsage: %s FILE\n", argv0);
    exit(1);
}

int
main(int argc, char** argv)
{
    if (argc != 2) usage(argv[0]);

    Program parse_tree = parse_file(argv[1]);
    if (!parse_tree) no_recover();

    define_primitives();
    add_classes(parse_tree);
    if (has_error) no_recover();

    i32* inhtree = build_inhtree(parse_tree);
    if (has_error) no_recover();

    i32* order = sort_classes(inhtree, array_size(g_types));
    if (has_error) no_recover();

    add_global_funcs(parse_tree);
    add_class_members_and_local_funcs(order, inhtree);
    if (has_error) no_recover();

    check_global_funcs(parse_tree);
    check_class_funcs(parse_tree);
    if (has_error) no_recover();

    optimize();

    char* asm_ext = repl_extension(argv[1], ".lat", ".s");
    char* obj_ext = repl_extension(argv[1], ".lat", ".o");
    char* binary_ext = repl_extension(argv[1], ".lat", "");
#if DUMP_IR
    char* ir_ext = repl_extension(argv[1], ".lat", ".ir");
#endif

    asm_dest = fopen(asm_ext, "w");
#if DUMP_IR
    ir_dest = fopen(ir_ext, "w");
#endif

#if DUMP_IR
    gen_ir();
    fclose(ir_dest);
#endif

#if 0 // TODO
    for (mm i = TYPEID_LAST_BUILTIN_TYPE + 1; i < array_size(g_types); ++i)
    {
        printf("type %ld (%s):\n", i, g_types[i].name);
        for (mm j = 0, n_member_funcs = array_size(g_types[i].member_funcs);
             j < n_member_funcs;
             ++j)
        {
            printf("    %ld -> %d\n", j, g_types[i].member_funcs[j].local_id);
        }
    }
    printf("\n\n");
#endif

    gen_code();
    fclose(asm_dest);

    if (strcmp(binary_ext, argv[1]) == 0)
    {
        // Just in case, we don't to mess with the input file
        binary_ext = repl_extension(argv[1], ".lat", ".exe");
    }

    // Accept the input first, becasue nasm and ld may write something to stderr
    accept_input();

    #define ASM_CMD "nasm -f elf64 -F dwarf -g %s -o %s"
    #define LINK_CMD "ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o %s -lc %s"

    char buf[4096];

    sprintf(buf, ASM_CMD, asm_ext, obj_ext);
    system(buf);

    sprintf(buf, LINK_CMD, binary_ext, obj_ext);
    system(buf);

    return 0;
}
