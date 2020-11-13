#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "absyn.h"
#include "parser.h"
#include "printer.h"
#include "misc.h"

#define ARRAY_IMPLEMENTATION
#define ARRAY_STATIC
#include "array.h"

#include "misc.c"

static void
usage(char* argv0)
{
    printf("Usage: %s [FILE]\n", argv0);
    exit(1);
}

int
main(void /*int argc, char** argv*/)
{
    FILE* input = stdin;
    array_reserve(node_lnums, 1024);

    // if (argc != 1)
    // usage(argv[0]);

    Program parse_tree = pProgram(input);
    if (parse_tree)
    {
        /* printf("\nParse Succesful!\n");
        printf("\n[Abstract Syntax]\n");
        printf("%s\n\n", showProgram(parse_tree));
        printf("[Linearized Tree]\n");
        printf("%s\n\n", printProgram(parse_tree));
        return 0; */

        accept_input();
        LIST_FOREACH(it, parse_tree->u.prog_.listtopdef_, listtopdef_)
        {
            TopDef t = it->topdef_;
            assert(t->kind == is_FnDef);
            printf("There is a declared function called: \"%s\"\n",
                   t->u.fndef_.ident_);
        }

        return 0;
    }


    // TODO: If don't trust bnfc, then possbibly write ERROR here also?
    // Can't lose?
    return 1;
}
