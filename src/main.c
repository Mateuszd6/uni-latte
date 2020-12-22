//
// TODO: strCmp and strAdd cleanup and tests!
//
// TODO: Reuse vars which are out-of-scope
//
// TODO: Add explicite LOAD/STORE for writing to arrays and class members
// TODO: Add IRVT for local functions
// TODO: Write builtin functions for adding and comparing strings
// TODO: Right now, it is possible to write to "self" variable in the memfunc
// TODO: Sometimes remove lhs from CALL ??
// TODO: Cleanup process_expr and try to remove is_lvalue from there
// TODO: Maybe the first param of IR_PUSH should be entire expression, not just .val
// TODO: Fix not all code paths return value now, with new if evaluation
// TODO: Remove evaluation of boolean expressions from process_expr ?
// TODO: Create a test in which boolean func params are evaluated correctly
// TODO: Don't emit jumping code, when condition is constant
// TODO: Check if variables get correct numbers in nested blocks
// TODO: Make NOREACH IR instruction and emit a single ret with this comment
//       there? Or remove appending the 'ret' at the end?
// BUG: Print strings seems to have a return type (in ir)
// BUG: String constant is emmited more than once. Unused constants are still
//      emmited (but who cares)?
// TODO: Optimize:
//
//       t_2 = CMP_LTH v_0 v_1
//       JMP_TRUE t_2 LAB ; t_2 is last time alive
//
//       CMP v_1 v_1
//       JMP_FLAGS_LTH LAB
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

#if DUMP_IR

// Write IR generated for the function
static void
dump_ir_for_function(u32 f_id)
{
    ir_quadr* ircode = g_funcs[f_id].code;
    char* fname = g_funcs[f_id].name;

    fprintf(ir_dest, "GLOBAL_FUNC_%u ; %s\n", f_id, fname);
    for (mm i = 0, size = array_size(ircode); i < size; ++i)
    {
        if (ircode[i].op == LABEL)
        {
            // NOP
        }
        else if (ircode[i].target.type != IRVT_NONE)
        {
            fprintf(ir_dest, "    %s%ld = ",
                    ir_val_type_name[ircode[i].target.type],
                    ircode[i].target.u.reg_id); // TODO: OR CONSTANT?
        }
        else
        {
            fprintf(ir_dest, "    ");
        }

        fprintf(ir_dest, "%s", ir_op_name[ircode[i].op]);

        for (mm a = 0; a < ir_op_n_args[ircode[i].op]; ++a)
        {
            if (ircode[i].u.args[a].type == IRVT_NONE)
                fprintf(ir_dest, " _");
            else
                fprintf(ir_dest, " %s%ld",
                        ir_val_type_name[ircode[i].u.args[a].type],
                        ircode[i].u.args[a].u.reg_id);
        }

        fprintf(ir_dest, "\n");
    }

    fprintf(ir_dest, "\n");
}
#endif

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

    if (has_error)
        no_recover();

#if DUMP_IR
    for (mm i = FUNCID_LAST_BUILTIN_FUNC + 1, size = array_size(g_funcs); i < size; ++i)
    {
        dump_ir_for_function((u32)i);
    }
#endif

    mm main_id = 0;
    for (mm i = FUNCID_LAST_BUILTIN_FUNC + 1, size = array_size(g_funcs); i < size; ++i)
    {
        gen_glob_func((u32)i);
        if (strcmp(g_funcs[i].name, "main") == 0)
            main_id = i;
    }

    gen_entry_point((i32)main_id);

    // TODO: extract to gen_constants:
    for (mm i = 0, size = array_size(g_str_consts); i < size; ++i)
    {
        fprintf(asm_dest, ".BS%ld:\n    db \"%s\",0x0\n", i, g_str_consts[i].data);
    }

    fclose(asm_dest);
#if DUMP_IR
    fclose(ir_dest);
#endif

    accept_input();
    return 0;
}
