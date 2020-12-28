// TODO: Support allocating func params - load allocated params in the func prelude
// TODO: Removed unused regs from eplilogue.s
//
// TODO: foo() || foo(); // as statement expression
//       produces incorrect code, because foo is evaludated more than once
//
// TODO: Reuse vars which are out-of-scope
//
// TODO: Fix broken readInt
//
// TODO: Add explicite LOAD/STORE for writing to arrays and class members
// TODO: Add IRVT for local functions
// TODO: Right now, it is possible to write to "self" variable in the memfunc
// TODO: Cleanup process_expr and try to remove is_lvalue from there
// TODO: Maybe the first param of IR_PUSH should be entire expression, not just .val
// TODO: Fix not all code paths return value now, with new if evaluation
// TODO: Remove evaluation of boolean expressions from process_expr ?
// TODO: Create a test in which boolean func params are evaluated correctly
// TODO: Don't emit jumping code, when condition is constant
// TODO: Check if variables get correct numbers in nested blocks
// TODO: Make NOREACH IR instruction and emit a single ret with this comment
//       there? Or remove appending the 'ret' at the end?
// TODO: Optim: Remove LHS if function result is unused
// BUG: Print strings seems to have a return type (in ir)
// BUG: Unused constants are still emmited (but who cares)?

// TODO: All registers are callee saved - make sure only necesarry regs are
//       saved and that asm code is correct

// Disable leak checking for asan in debug mode
#ifdef DEBUG
extern char const* __asan_default_options(void);
extern char const* __asan_default_options() { return "detect_leaks=0"; }
#endif

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

// TODO: Move below all includes
static FILE* asm_dest;
#if DUMP_IR
static FILE* ir_dest;
#endif

#include "misc.c"
#include "symtab.c"
#include "frontend.c"
#include "optim.c"

#include "codegen.c"

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

    define_primitives();
    add_classes(parse_tree);
    add_global_funcs(parse_tree);
    add_class_members_and_local_funcs(parse_tree); // TODO: Do it when classes are processed?

    if (has_error)
        no_recover();

    check_global_funcs(parse_tree);
    check_class_funcs(parse_tree);

    if (has_error)
        no_recover();

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
#endif

    gen_code();

    fclose(asm_dest);
#if DUMP_IR
    fclose(ir_dest);
#endif

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
