// TODO:
// * regenerate parser on students, b/c current have different bison version
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

#include "absyn.h"
#define ARRAY_STATIC
#include "array.h"
#include "misc.h"
#include "parser.h"
#include "symtab.h"

#define ARRAY_IMPLEMENTATION
#include "array.h" // As implementation
#include "misc.c"
#include "symtab.c"

char* myfilename = 0;

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

typedef enum evaled_expr_t evaled_expr_t; // TODO: Move
enum evaled_expr_t
{
    EET_COMPUTE,
    EET_CONSTANT,
};

typedef enum binary_int_op_t binary_int_op_t;
enum binary_int_op_t
{
    BIOP_ADD,
    BIOP_SUB,
    BIOP_MUL,
    BIOP_DIV,
    BIOP_MOD,
    BIOP_LTH,
    BIOP_LE,
    BIOP_GTH,
    BIOP_GE,
};

typedef struct evaled_expr evaled_expr;
struct evaled_expr
{
    u32 type_id;
    evaled_expr_t kind;
    union
    {
        struct
        {
        } cmpt;
        struct
        {
            i64 numeric_val; // int or boolean
            char* str_val; // string // TODO: make it reasonable
        } cnst;
    } u;
};

static evaled_expr
eval_expr(Expr e)
{
    evaled_expr retval;
    Expr binarg1; // To be set for binary expressions
    Expr binarg2;
    binary_int_op_t binintop;

    switch (e->kind) {
    case is_ELitTrue:
    case is_ELitFalse:
    {
        retval.kind = EET_CONSTANT;
        retval.u.cnst.numeric_val = (e->kind == is_ELitTrue); // true/false
        return retval;
    }

    case is_ELitInt:
    {
        retval.kind = EET_CONSTANT;
        retval.u.cnst.numeric_val = (i64)e->u.elitint_.integer_;
        return retval;
    }

    case is_Not:
    {
        evaled_expr e1 = eval_expr(e->u.not_.expr_);
        // TODO: Validate type
        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            e1.u.cnst.numeric_val = !e1.u.cnst.numeric_val;
            return e1;
        }

        retval.kind = EET_COMPUTE;
        return retval; // TODO
    }

    case is_Neg:
    {
        evaled_expr e1 = eval_expr(e->u.not_.expr_);
        // TODO: Validate type
        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            // TODO: Check overflow of -2kkk to positive value!!
            e1.u.cnst.numeric_val = -e1.u.cnst.numeric_val;
            return e1;
        }

        retval.kind = EET_COMPUTE;
        return retval; // TODO
    }

    case is_EMul:
    {
        binarg1 = e->u.emul_.expr_1;
        binarg2 = e->u.emul_.expr_2;
        switch (e->u.emul_.mulop_->kind) {
        case is_Times: binintop = BIOP_MOD; break;
        case is_Div: binintop = BIOP_DIV; break;
        case is_Mod: binintop = BIOP_MOD; break;
        }
    } goto binary_integer_expr;

    case is_EAdd:
    {
        binarg1 = e->u.eadd_.expr_1;
        binarg2 = e->u.eadd_.expr_2;
        switch (e->u.eadd_.addop_->kind) {
        case is_Plus: binintop = BIOP_ADD; break;
        case is_Minus: binintop = BIOP_SUB; break;
        }
    } goto binary_integer_expr;

    case is_ERel: // "==" is different expr (EEq), this one is for ints only
    {
        binarg1 = e->u.eadd_.expr_1;
        binarg2 = e->u.eadd_.expr_2;
        switch (e->u.erel_.relop_->kind) {
        case is_LTH: binintop = BIOP_LTH; break;
        case is_GTH: binintop = BIOP_GTH; break;
        case is_LE: binintop = BIOP_LE; break;
        case is_GE: binintop = BIOP_GE; break;
        }
    } goto binary_integer_expr;

    binary_integer_expr:
    {
        evaled_expr e1 = eval_expr(binarg1);
        evaled_expr e2 = eval_expr(binarg2);
        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            // TODO(NEXT): Caluclate new integer value and return as constant.
        }

        retval.kind = EET_COMPUTE;
        return retval; // TODO
    }

    case is_EEq:
    {
    } break;

    case is_EAnd:
    case is_EOr:
    {
    } break;

    case is_EVar:
    case is_EApp:

    case is_ECast:
    case is_ELitStr:
    case is_EClMem:
    case is_EArrApp:
    case is_ENew:
    case is_ENewArr:
    case is_ENull:
        NOTREACHED;
    }

    NOTREACHED;
};

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

    Block blk = parse_tree->u.prog_.listtopdef_->topdef_->u.fndef_.block_;
    LIST_FOREACH(it, blk->u.blk_, liststmt_)
    {
        Stmt s = it->stmt_;
        printf("stmt type: %d\n", s->kind);

        switch (s->kind) {
        case is_SExp: {
            evaled_expr ee = eval_expr(s->u.sexp_.expr_);
        } break;

        case is_Empty:
        case is_BStmt:
        case is_Decl:
        case is_Ass:
        case is_Incr:
        case is_Decr:
        case is_Ret:
        case is_VRet:
        case is_Cond:
        case is_CondElse:
        case is_While:
        case is_For:
            NOTREACHED;
        }
    }

    return 0;
}
