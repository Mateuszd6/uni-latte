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

#include "misc.h" // typedefs

#ifdef USE_ARENA_ALLOCATOR
#  warning Using arena allocator!
#  define ARENA_STATIC
#  define ARENA_IMPLEMENTATION
#  include "arena.h"
#endif

#include "absyn.h"
#include "absyn.c"
#include "frontend.h"

#include "parser.h"
#include "symtab.h"

#ifdef USE_ARENA_ALLOCATOR
static arena g_arena_allocator = {0};
#endif

#include "misc.c"
#include "symtab.c"
#include "frontend.c"

#include <time.h>
#include <sys/types.h>

//
// Used to get a filename when error reporting in parser in extern parser
//
char* myfilename = 0;

#define TIMER_PUSH()                                                           \
    struct timespec tstart={0,0}, tend={0,0};                                  \
    clock_gettime(CLOCK_MONOTONIC, &tstart)

#define TIMER_POP() (clock_gettime(CLOCK_MONOTONIC, &tend),                    \
                     ((((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -           \
                       ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec)) * 1000))

static Program
parse_file(char* fname)
{
    char* in_name = fname;
    myfilename = in_name;

    FILE* input = fopen(in_name, "r");
    if (!input)
        fatal("Could not open file %s for reading", in_name);

    lnumtab_reserve(g_lnumtab, 128);
    Program parse_tree;

    TIMER_PUSH();
    {
        parse_tree = pProgram(input);
        fclose(input);
    }

    double parse_time = TIMER_POP();
    printf("parsing: %.5f\n", parse_time);

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

    Program parse_tree;
    parse_tree = parse_file(argv[1]);
    if (!parse_tree)
        no_recover();

    TIMER_PUSH();
    {
        define_primitives();
        add_classes(parse_tree);
        add_global_funcs(parse_tree);
        add_class_members_and_local_funcs(parse_tree);

        if (has_error)
            no_recover();

        check_global_funcs(parse_tree);
        check_class_funcs(parse_tree);
    }

    double parse_time = TIMER_POP();
    printf("processing: %.5f\n", parse_time);

    if (has_error)
        no_recover();

    accept_input();
    return 0;
}
