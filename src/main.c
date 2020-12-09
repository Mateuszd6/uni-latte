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
#include "array.h"

#define HASHMAP_STATIC
#define HASHMAP_IMPLEMENTATION
#include "hashmap.h"

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

    // lnumtab_reserve(g_lnumtab, 128);
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
    if (!parse_tree)
        no_recover();

    define_primitives();
    add_classes(parse_tree);
    add_class_members(parse_tree);
    add_global_funcs(parse_tree);

    if (has_error)
        no_recover();

    LIST_FOREACH(it, parse_tree->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_FnDef)
            continue;

        process_func_body(t->u.fndef_.ident_, t->u.fndef_.block_, t->u.fndef_.type_);
    }

    LIST_FOREACH(it, parse_tree->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_ClDef)
            continue;

        LIST_FOREACH(cl_it, t->u.cldef_, listclbody_)
        {
            ClBody cl = cl_it->clbody_;
            if (cl->kind != is_CBFnDef)
                continue;

            u32 type_id = symbol_resolve_type(t->u.cldef_.ident_, 0, cl);
            assert(type_id != TYPEID_NOTFOUND);

            d_type type = g_types[type_id];
            mm n_member_funcs = array_size(type.member_funcs);
            for (mm i = 0; i < n_member_funcs; ++i)
                create_func(type.member_funcs[i]);

            push_block(); // Block for class memebers
            mm n_members = array_size(type.members);
            for (mm i = 0; i < n_members; ++i)
            {
                char* vname = type.members[i].name;
                d_var var = {
                    .lnum = type.members[i].lnum,
                    .type_id = type.members[i].type_id,
                    .block_id = -1,
                };

                mm var_id = array_size(g_vars);
                symbol sym;
                sym.type = S_VAR;
                sym.id = (i32)var_id;
                symbol_push(vname, sym);

                array_push(g_vars, var);
                array_push(g_local_symbols, vname);
            }

            process_func_body(cl->u.cbfndef_.ident_, cl->u.cbfndef_.block_, cl->u.cbfndef_.type_);

            pop_block();

            // Pop back the member functions
            for (mm i = 0; i < n_member_funcs; ++i)
                symbol_pop(type.member_funcs[i].name);
            array_popn(g_funcs, n_member_funcs);
        }
    }

    if (has_error)
        no_recover();

    accept_input();
    return 0;
}
