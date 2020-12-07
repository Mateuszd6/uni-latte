// TODO:
// * regenerate parser on students, b/c current have different bison version
// * make a frontend.h/c file
// * looks like if a constant is too large, -1 is retuend, check and implement accordingly
// * void[] return type / param of the function? void arguments
// * tests: void type everywhere!
// * tests: void[] type everywhere!
// * Extract g_types[type_id & (~TYPEID_FLAG_ARRAY)]; to a function

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

typedef enum binary_bool_op_t binary_bool_op_t;
enum binary_bool_op_t
{
    BBOP_AND,
    BBOP_OR,
};

typedef enum binary_eq_op_t binary_eq_op_t;
enum binary_eq_op_t
{
    BEOP_EQ,
    BEOP_NEQ,
};

typedef struct evaled_expr evaled_expr;
struct evaled_expr
{
    void* node; // For lnum in case of error reporting
    u32 type_id;
    evaled_expr_t kind;
    b32 is_lvalue;
    union
    {
        struct
        {
        } cmpt;
        union
        {
            i64 numeric_val; // int or boolean
            mm str_const_id; // string constant
        } cnst;
    } u;
};

typedef struct evaled_stmt evaled_stmt;
struct evaled_stmt
{
    b32 all_branches_return;
};

// Assumes that the type exists
static evaled_expr
get_default_expr(u32 type_id, void* node)
{
    evaled_expr retval;
    retval.node = node;
    retval.type_id = type_id;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;

    switch (type_id) {
    case TYPEID_VOID:
    {
        // TODO: Do we even need void?
        NOTREACHED;
    }
    case TYPEID_INT:
    case TYPEID_BOOL:
    {
        retval.u.cnst.numeric_val = 0;
        return retval;
    }
    case TYPEID_STRING:
    {
        retval.u.cnst.str_const_id = EMPTY_STRING_CONSTANT_ID;
        return retval;
    }
    default:
    {
        retval.node = node;
        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 1;

        return retval;
    }
    }
}

static evaled_stmt
create_empty_statement(void)
{
    evaled_stmt retval;
    retval.all_branches_return = 0;

    return retval;
}

static evaled_expr
compute_binary_boolean_expr(mm v1, mm v2, binary_bool_op_t op, void* node)
{
    evaled_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    switch (op) {
    case BBOP_AND: retval.u.cnst.numeric_val = !!(v1) && !!(v2); break;
    case BBOP_OR: retval.u.cnst.numeric_val = !!(v1) || !!(v2); break;
    }

    return retval;
}

static evaled_expr
compute_binary_integer_expr(mm v1, mm v2, binary_int_op_t op, void* node)
{
    evaled_expr retval;
    retval.type_id = TYPEID_INT;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    switch (op) {
    case BIOP_ADD: retval.u.cnst.numeric_val = v1 + v2; break;
    case BIOP_SUB: retval.u.cnst.numeric_val = v1 - v2; break;
    case BIOP_MUL: retval.u.cnst.numeric_val = v1 * v2; break;
    case BIOP_DIV: retval.u.cnst.numeric_val = v1 / v2; break;
    case BIOP_MOD: retval.u.cnst.numeric_val = v1 % v2; break;
    case BIOP_LTH: retval.u.cnst.numeric_val = (v1 < v2); retval.type_id = TYPEID_BOOL; break;
    case BIOP_LE: retval.u.cnst.numeric_val = (v1 <= v2); retval.type_id = TYPEID_BOOL; break;
    case BIOP_GTH: retval.u.cnst.numeric_val = (v1 > v2); retval.type_id = TYPEID_BOOL; break;
    case BIOP_GE: retval.u.cnst.numeric_val = (v1 >= v2); retval.type_id = TYPEID_BOOL; break;
    }

    // Safe for booleans becasue they never overflow. In case of overflow, leave
    // 0 to have something reasonable for computations, we won't compile anyway
    if (UNLIKELY(overflows_32bit(retval.u.cnst.numeric_val)))
    {
        error(get_lnum(node), "Integer constant overflows");
        retval.u.cnst.numeric_val = 0;
    }

    return retval;
}

static evaled_expr
compute_binary_string_expr(mm str1, mm str2, void* node)
{
    evaled_expr retval;
    retval.type_id = TYPEID_STRING;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;

    string_const s1 = g_str_consts[str1];
    string_const s2 = g_str_consts[str2];

    // TODO: This is allocated differently that strings hard-coded in code, and
    //       won't be easy to free!
    char* new_string = malloc((umm)(s1.len + s2.len + 1));
    new_string[0] = 0;
    strcat(new_string, s1.data);
    strcat(new_string, s2.data);

    string_const s;
    s.data = new_string;
    s.len = s1.len + s2.len;

    retval.u.cnst.str_const_id = array_size(g_str_consts);
    array_push(g_str_consts, s);

    return retval;
}

static i32 next_block_id = 1;
static i32
push_block(void)
{
    array_push(g_local_symbols, 0); // null is used to makr that the block ends
    return next_block_id++;
}

static void
pop_block(void)
{
    while (1)
    {
        mm size = array_size(g_local_symbols);
        assert(size > 0);
        char* popped = array_pop(g_local_symbols);
        if (!popped) break;

        printf("Popping variable: %s\n", popped);
        symbol_pop(popped);
    }
}

static evaled_expr
compute_binary_integer_or_boolean_equality_expr(mm v1, mm v2, binary_eq_op_t op, void* node)
{
    evaled_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    retval.u.cnst.numeric_val = !((op == BEOP_EQ) ^ (v1 == v2));

    return retval;
}

static evaled_expr
compute_binary_string_equality_expr(mm str1, mm str2, binary_eq_op_t op, void* node)
{
    b32 compre = strcmp(g_str_consts[str1].data, g_str_consts[str2].data) == 0;
    evaled_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    retval.u.cnst.numeric_val = !((op == BEOP_EQ) ^ compre);

    return retval;
}

static void
enforce_type(evaled_expr* e, u32 type_id)
{
    if (UNLIKELY(e->type_id != type_id))
    {
        d_type t_expected = g_types[type_id & (~TYPEID_FLAG_ARRAY)];
        d_type t_got = g_types[e->type_id & (~TYPEID_FLAG_ARRAY)];

        // TODO: lnum
        error(get_lnum(e->node),
              "Expected type \"%s\" but got incompatible type \"%s\"",
              t_expected.name, t_got.name);

        // TODO: Set COMPUTE to true and add some fake value so that we carry on
        //       with a compilation Or move it outside the func, bc/
        //       enforce_type_of can't do it
    }
}

// TODO: Rename eval_ -> ?

// TODO: Define all of them in the header file
static evaled_expr eval_expr(Expr e);

static void
eval_params(ListExpr arg_exprs, d_func* fun, void* node)
{
    // 1st param "self" for local func. So for local functions we start
    // evaluating params from the second arg. Error messages needs to be
    // adjusted to this hack!
    i32 n_given_args = 0 + !!fun->is_local;

    struct { ListExpr listexpr_; } expr_list;
    expr_list.listexpr_ = arg_exprs;
    LIST_FOREACH(it, expr_list, listexpr_)
    {
        if (LIKELY(n_given_args < fun->num_args)) // if not too many args given
        {
            Expr argexpr = it->expr_;
            evaled_expr argexpr_e = eval_expr(argexpr);
            u32 expected_type_id = fun->arg_type_ids[n_given_args].type_id;

            if (argexpr_e.type_id != expected_type_id) // TODO(ex): Handle inheritance
            {
                d_type t_expected = g_types[expected_type_id & (~TYPEID_FLAG_ARRAY)];
                d_type t_provided = g_types[argexpr_e.type_id & (~TYPEID_FLAG_ARRAY)];

                error(get_lnum(argexpr), "Function \"%s\" expects argument %d to have a type \"%s\"",
                      fun->name, n_given_args + 1 - !!(fun->is_local), t_expected.name);

                note(get_lnum(argexpr), "Given expression of type \"%s\", which is not assignable to type \"%s\"",
                     t_provided.name, t_expected.name);
            }
        }

        n_given_args++;
    }

    if (n_given_args != fun->num_args)
    {
        error(get_lnum(node),
              "Function \"%s\" takes %d arguments, but %d were provided.",
              fun->name,
              fun->num_args - !!fun->is_local,
              n_given_args - !!fun->is_local);
    }
}

static evaled_expr
eval_expr(Expr e)
{
    evaled_expr retval;
    Expr binarg1; // To be set for binary expressions
    Expr binarg2;
    binary_int_op_t binintop;
    binary_bool_op_t binboolop;

    retval.node = e;
    switch (e->kind) {
    case is_ELitTrue:
    case is_ELitFalse:
    {
        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.numeric_val = (e->kind == is_ELitTrue); // true/false
        return retval;
    }

    case is_ELitInt:
    {
        // Looks like BNFC (or bison (or flex?)) leaves here -1 if the number
        // was too large. Print a reasonable warning, and treat this value as 0
        if (e->u.elitint_.integer_ < 0)
        {
            error(get_lnum(e), "Integer constant is too large");
            e->u.elitint_.integer_ = 0;
        }

        retval.type_id = TYPEID_INT;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.numeric_val = (i64)e->u.elitint_.integer_;
        return retval;
    }

    case is_ELitStr:
    {
        retval.type_id = TYPEID_STRING;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;

        char* data = e->u.elitstr_.string_;
        string_const c = { .data = data, .len = (mm)strlen(data) };
        mm idx = array_size(g_str_consts);
        array_push(g_str_consts, c);

        retval.u.cnst.str_const_id = idx;
        return retval;
    }

    case is_Not:
    {
        evaled_expr e1 = eval_expr(e->u.not_.expr_);
        enforce_type(&e1, TYPEID_BOOL);

        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue); // Already an lvalue
            e1.u.cnst.numeric_val = !e1.u.cnst.numeric_val;
            return e1;
        }

        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO
    }

    case is_Neg:
    {
        evaled_expr e1 = eval_expr(e->u.not_.expr_);
        enforce_type(&e1, TYPEID_INT);

        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            // TODO: Check overflow of -2kkk to positive value!!
            assert(!e1.is_lvalue);
            e1.u.cnst.numeric_val = -e1.u.cnst.numeric_val;
            return e1;
        }

        retval.type_id = TYPEID_INT;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO
    }

    case is_EMul:
    {
        binarg1 = e->u.emul_.expr_1;
        binarg2 = e->u.emul_.expr_2;
        switch (e->u.emul_.mulop_->kind) {
        case is_Times: binintop = BIOP_MUL; break;
        case is_Div: binintop = BIOP_DIV; break;
        case is_Mod: binintop = BIOP_MOD; break;
        }

        goto binary_integer_expr;
    }

    case is_EAdd:
    {
        binarg1 = e->u.eadd_.expr_1;
        binarg2 = e->u.eadd_.expr_2;
        switch (e->u.eadd_.addop_->kind) {
        case is_Plus: binintop = BIOP_ADD; break;
        case is_Minus: binintop = BIOP_SUB; break;
        }

        goto binary_integer_expr;
    }

    case is_ERel: // "==" is different expr (EEq), this one is for ints only
    {
        binarg1 = e->u.erel_.expr_1;
        binarg2 = e->u.erel_.expr_2;
        switch (e->u.erel_.relop_->kind) {
        case is_LTH: binintop = BIOP_LTH; break;
        case is_GTH: binintop = BIOP_GTH; break;
        case is_LE: binintop = BIOP_LE; break;
        case is_GE: binintop = BIOP_GE; break;
        }

        goto binary_integer_expr;
    }

    case is_EAnd:
    {
        binarg1 = e->u.eand_.expr_1;
        binarg2 = e->u.eand_.expr_2;
        binboolop = BBOP_AND;

        goto binary_boolean_expr;
    }

    case is_EOr:
    {
        binarg1 = e->u.eor_.expr_1;
        binarg2 = e->u.eor_.expr_2;
        binboolop = BBOP_OR;

        goto binary_boolean_expr;
    }

    binary_boolean_expr:
    {
        evaled_expr e1 = eval_expr(binarg1);
        evaled_expr e2 = eval_expr(binarg2);
        enforce_type(&e1, TYPEID_BOOL);
        enforce_type(&e2, TYPEID_BOOL);
        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.u.cnst.numeric_val;
            mm v2 = e2.u.cnst.numeric_val;
            return compute_binary_boolean_expr(v1, v2, binboolop, e1.node);
        }

        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO
    }

    binary_integer_expr:
    {
        evaled_expr e1 = eval_expr(binarg1);
        evaled_expr e2 = eval_expr(binarg2);

        // This is the only case when an operator other that == and != is
        // overloaded. IMO string concatenation should have separate operator
        // (like '@'). Since this is a unique exception, we hack around it:
        if (e1.type_id == TYPEID_STRING && binintop == BIOP_ADD)
        {
            enforce_type(&e2, TYPEID_STRING);
            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
            {
                assert(!e1.is_lvalue);
                assert(!e2.is_lvalue);

                mm str1 = e1.u.cnst.str_const_id;
                mm str2 = e2.u.cnst.str_const_id;
                return compute_binary_string_expr(str1, str2, e1.node);
            }

            retval.type_id = TYPEID_STRING;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        enforce_type(&e1, TYPEID_INT);
        enforce_type(&e2, TYPEID_INT); // TODO: Avoid doubled error message if both types won't work with "+"?
        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.u.cnst.numeric_val;
            mm v2 = e2.u.cnst.numeric_val;
            return compute_binary_integer_expr(v1, v2, binintop, e1.node);
        }

        // TODO: un-uglify
        if (binintop == BIOP_LTH || binintop == BIOP_LE
            || binintop == BIOP_GTH || binintop == BIOP_GE)
        {
            retval.type_id = TYPEID_BOOL;
        }
        else
        {
            assert(binintop == BIOP_ADD || binintop == BIOP_SUB
                   || binintop == BIOP_MUL || binintop == BIOP_DIV
                   || binintop == BIOP_MOD);
            retval.type_id = TYPEID_INT;
        }

        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO
    }

    case is_EEq:
    {
        evaled_expr e1 = eval_expr(e->u.eeq_.expr_1);
        evaled_expr e2 = eval_expr(e->u.eeq_.expr_2);
        binary_eq_op_t bineqop;
        switch (e->u.eeq_.eqop_->kind) {
        case is_NE: bineqop = BEOP_NEQ; break;
        case is_EQU: bineqop = BEOP_EQ; break;
        }

        switch (e1.type_id) {
        case TYPEID_INT:
        {
            if (e2.type_id != TYPEID_INT)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        case TYPEID_BOOL:
        {
            if (e2.type_id != TYPEID_BOOL)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        case TYPEID_STRING:
        {
            if (e2.type_id != TYPEID_STRING)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_string;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        compute_contant_equality_int_or_bool:
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.u.cnst.numeric_val;
            mm v2 = e2.u.cnst.numeric_val;
            return compute_binary_integer_or_boolean_equality_expr(v1, v2, bineqop, e1.node);
        }

        compute_contant_equality_string:
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm str1 = e1.u.cnst.str_const_id;
            mm str2 = e2.u.cnst.str_const_id;
            return compute_binary_string_equality_expr(str1, str2, bineqop, e1.node);
        }

        type_missmatch:
        {
            d_type lhs_t = g_types[e1.type_id & (~TYPEID_FLAG_ARRAY)];
            d_type rhs_t = g_types[e2.type_id & (~TYPEID_FLAG_ARRAY)];

            error(get_lnum(e1.node), "Comparing expressions with incompatible types");
            note(get_lnum(e1.node), "Left hand side evalutes to \"%s\"", lhs_t.name);
            note(get_lnum(e2.node), "Right hand side evalutes to \"%s\"", rhs_t.name);

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        //
        // Latte is unspecified when it comes to comparing the reference type
        // other than strings, so we asssume that this is basically the equality
        // of references (so that equality against null is meaningful).
        //
        default:
        {
            // TODO(ex): Make sure equality works when type A inherits from type B
            if (e1.type_id != e2.type_id)
                goto type_missmatch;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }
        }
    }

    case is_EVar:
    {
        Ident var_name = e->u.evar_.ident_;
        u32 var_id = symbol_resolve_var(var_name, e);
        if (LIKELY(var_id != VARID_NOTFOUND))
        {
            d_var var = g_vars[var_id];
            retval.type_id = var.type_id;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 1;
            return retval; // TODO
        }
        else
        {
            // TODO: This might not be the best thing to do here
            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 1;
            return retval; // TODO
        }
    }

    case is_EApp: // TODO
    {
        u32 func_id = symbol_resolve_func(e->u.eapp_.ident_, e);
        if (func_id == FUNCID_NOTFOUND)
        {
            // Can't do anything reasonable, so just asume that type is int
            // TODO: Defaults to "int"
            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            return retval;
        }

        d_func* f = g_funcs + func_id;
        eval_params(e->u.eapp_.listexpr_, f, e);

        retval.is_lvalue = 0; // Like in Java, functions always return rvalues
        retval.type_id = f->ret_type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENew:
    {
        char* type_name = e->u.enew_.ident_;
        u32 type_id = symbol_resolve_type(type_name, 0, e);
        assert(type_id == TYPEID_NOTFOUND || !(type_id & TYPEID_FLAG_ARRAY));
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // Default to int in order to avoid errors
        } break;
        case TYPEID_VOID:
        case TYPEID_INT:
        case TYPEID_BOOL:
        case TYPEID_STRING:
        {
            error(get_lnum(e), "Primitive types cannot be heap-allocated with new");
            type_id = TYPEID_INT;
        } break;
        default:
        {
        } break;
        }

        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENewArr:
    {
        char* type_name = e->u.enewarr_.ident_;
        Expr esize = e->u.enewarr_.expr_;
        u32 type_id = symbol_resolve_type(type_name, 0, e);
        assert(type_id == TYPEID_NOTFOUND || !(type_id & TYPEID_FLAG_ARRAY));
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // Default to int in order to avoid errors
        } break;
        case TYPEID_VOID:
        {
            error(get_lnum(e), "\"void\" type cannot be used in an array");
            type_id = TYPEID_INT;
        } break;
        default:
        {
        } break;
        }
        type_id |= TYPEID_FLAG_ARRAY; // The resulting type of the expr is array

        evaled_expr e_esize = eval_expr(esize);
        enforce_type(&e_esize, TYPEID_INT);

        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_EArrApp:
    {
        evaled_expr e_array = eval_expr(e->u.earrapp_.expr_1);
        if (!(e_array.type_id & TYPEID_FLAG_ARRAY))
        {
            error(get_lnum(e->u.earrapp_.expr_1),
                  "Non-array cannot be a left-hand-side of the array subscript");
        }

        evaled_expr e_idx = eval_expr(e->u.earrapp_.expr_2);
        enforce_type(&e_idx, TYPEID_INT);

        // If given array is lvalue then it's subscript also is
        retval.is_lvalue = e_array.is_lvalue;
        retval.type_id = e_array.type_id & (~TYPEID_FLAG_ARRAY);
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_EClMem:
    {
        evaled_expr e_struct = eval_expr(e->u.eclmem_.expr_);
        if (e_struct.type_id & TYPEID_FLAG_ARRAY)
        {
            // In case of array, only .length is possible
            if (UNLIKELY(strcmp(e->u.eclmem_.ident_, "length") != 0))
            {
                error(get_lnum(e->u.eclmem_.expr_),
                      "Member field \"%s\" does not exists on the \"array\" type",
                      e->u.eclmem_.ident_);
            }

            retval.is_lvalue = 0;
            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            return retval;
        }
        else if (e_struct.type_id <= TYPEID_LAST_BUILTIN_TYPE)
        {
            d_type t_provided = g_types[e_struct.type_id & (~TYPEID_FLAG_ARRAY)];
            error(get_lnum(e->u.eclmem_.expr_),
                  "Left-hand-side of . expression is not a struct nor array");
            note(get_lnum(e->u.eclmem_.expr_),
                 "Expression type evaluates to \"%s\"", t_provided.name);

            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            return retval;
        }
        else
        {
            d_type cltype = g_types[e_struct.type_id & (~TYPEID_FLAG_ARRAY)];

            // TODO: Replace with bsearch for suuuper large structs?
            mm n_members = array_size(cltype.members);
            mm idx = 0;
            for (; idx < n_members; ++idx)
                if (strcmp(cltype.members[idx].name, e->u.eclmem_.ident_) == 0)
                {
                    retval.type_id = cltype.members[idx].type_id;
                    break;
                }

            if (UNLIKELY(idx == n_members))
            {
                error(get_lnum(e->u.eclmem_.expr_),
                      "Class \"%s\" does not have a member named \"%s\"",
                      cltype.name,
                      e->u.eclmem_.ident_);

                retval.type_id = TYPEID_INT; // TODO: Defaulting to "int"
            }

            retval.is_lvalue = e_struct.is_lvalue;
            retval.kind = EET_COMPUTE;
            return retval;
        }

        NOTREACHED;
    }

    case is_EClApp:
    {
        evaled_expr e_struct = eval_expr(e->u.eclapp_.expr_);
        d_type cltype = g_types[e_struct.type_id];
        char* fn_name = e->u.eclapp_.ident_;

        if (cltype.is_primitive)
        {
            error(get_lnum(e->u.eclapp_.expr_), "Builtin types does not have a member functions");
            note(get_lnum(e->u.eclapp_.expr_), "Expresion evaluated to type \"%s\"", cltype.name);
            retval.type_id = TYPEID_INT; // TODO: Defaulting to "int"
        }
        else
        {
            // TODO: Replace with bsearch for suuuper large structs?
            mm n_member_funcs = array_size(cltype.member_funcs);
            mm idx = 0;
            for (; idx < n_member_funcs; ++idx)
                if (strcmp(cltype.member_funcs[idx].name, fn_name) == 0)
                {
                    retval.type_id = cltype.member_funcs[idx].ret_type_id;
                    break;
                }

            if (UNLIKELY(idx == n_member_funcs))
            {
                error(get_lnum(e->u.eclapp_.expr_), "Function \"%s\" is not a member of class \"%s\"",
                      fn_name, cltype.name);
                note(cltype.lnum, "Class \"%s\" defined here", cltype.name);
                retval.type_id = TYPEID_INT; // TODO: Defaulting to "int"
            }

            d_func* f = cltype.member_funcs + idx;
            eval_params(e->u.eclapp_.listexpr_, f, e);
        }

        retval.is_lvalue = 0; // Like in Java, functions always return rvalues
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENull:
    {
        retval.type_id = TYPEID_NULL;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.numeric_val = 0;
        return retval;
    }

    case is_ECast:
    {
        char* type_name = e->u.ecast_.ident_;
        u32 type_id = symbol_resolve_type(type_name, 0, e);
        assert(type_id == TYPEID_NOTFOUND || !(type_id & TYPEID_FLAG_ARRAY));
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // TODO: defaults to "int"
        } break;
        case TYPEID_VOID:
        case TYPEID_INT:
        case TYPEID_BOOL:
        case TYPEID_STRING:
        case TYPEID_NULL:
        {
            error(get_lnum(e), "Casting to a builtin type is not allowed.");
            type_id = TYPEID_INT; // TODO: defaults to "int"
        } break;
        default:
        {
        } break;
        }

        evaled_expr expr_to_cast = eval_expr(e->u.ecast_.expr_);
        switch (expr_to_cast.type_id) {
        case TYPEID_NULL: // Null is always castable to desired type
        {
            retval.type_id = type_id;
            retval.kind = EET_COMPUTE;
            return retval;
        }
        default:
        {
            // TODO(ex): Implement casting between types when inhericance is implemented
            warn(get_lnum(e), "NOT IMPLEMENTED"); // TODO
            NOTREACHED;
        } break;
        }
    }
    }

    NOTREACHED;
};

static evaled_stmt
eval_stmt(Stmt s, u32 return_type, i32 cur_block_id)
{
    evaled_stmt retval;
    switch (s->kind) {
    case is_Empty:
    {
        return create_empty_statement();
    }

    case is_BStmt:
    {
        Block b = s->u.bstmt_.block_;
        assert(b->kind == is_Blk);

        b32 all_branches_return = 0;
        i32 new_block_id = push_block();
        LIST_FOREACH(it, b->u.blk_, liststmt_)
        {
            evaled_stmt ev_s = eval_stmt(it->stmt_, return_type, new_block_id);
            all_branches_return |= ev_s.all_branches_return;
        }
        pop_block();

        retval.all_branches_return = all_branches_return;
        return retval;
    }

    case is_SExp:
    {
        evaled_expr ee = eval_expr(s->u.sexp_.expr_);
        (void)ee;

        // TODO(ir): If expression is not constant, generate IT for it anyway,
        //           b/c it can call a function that causes side-effects

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Decl:
    {
        Type var_type = s->u.decl_.type_;
        parsed_type type = parse_type(s->u.decl_.type_);
        u32 type_id = symbol_resolve_type(type.name, type.is_array, var_type);

        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // TODO: Defaults to "int"
        } break;
        case TYPEID_VOID:
        case TYPEID_VOID | TYPEID_FLAG_ARRAY:
        {
            error(get_lnum(var_type), "Cannot declare a variable of type \"void%s\"",
                  type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

            // TODO: Defaulting to \"int\"
            type_id = TYPEID_INT; // If void[] change to void, to aVOID errors
        } break;
        default:
        {
        } break;
        }

        LIST_FOREACH(it, s->u.decl_, listitem_)
        {
            Item i = it->item_;
            char* vname;
            evaled_expr vval;
            switch (i->kind) {
            case is_NoInit:
            {
                vname = i->u.noinit_.ident_;
                vval = get_default_expr(type_id, i);
            } break;
            case is_Init:
            {
                vname = i->u.init_.ident_;
                vval = eval_expr(i->u.init_.expr_);

                if (type_id != vval.type_id) // TODO: Handle the case with inheritance
                {
                    d_type expected_t = g_types[type_id & (~TYPEID_FLAG_ARRAY)];
                    d_type got_t = g_types[vval.type_id & (~TYPEID_FLAG_ARRAY)];
                    error(get_lnum(i),
                          "Expression of type \"%s%s\" is initialized with incompatible type \"%s%s\"",
                          expected_t.name, type_id & TYPEID_FLAG_ARRAY ? "[]" : "",
                          got_t.name, vval.type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

                    // Clarify why null is not implicitely assignable to a reference type:
                    if (vval.type_id == TYPEID_NULL && type_id > TYPEID_LAST_BUILTIN_TYPE)
                    {
                        note(get_lnum(i),
                             "\"null\" is not implicitly assingable to a reference type. Use: \"(%s)null\"",
                             expected_t.name);
                        note(get_lnum(i),
                             "This requirement is forced by the Author of the assignment. Sorry.");
                    }
                }
            } break;
            }

            // Regardless of whether or not inicializing errored declare the
            // variable with a type defined on the left, to avoid error cascade.
            d_var var;
            var.lnum = get_lnum(i);
            var.type_id = type_id;
            var.block_id = cur_block_id;
            push_var(var, vname, i);

            // TODO: Check if shadows or redefined in the same block
            printf("Declaring variable of type %u%s named \"%s\"\n",
                   type_id & TYPEID_MASK,
                   type_id & TYPEID_FLAG_ARRAY ? "[]" : "",
                   vname);

#if 1 // TODO: COPYPASTE, REMOVE
            printf("    Initialized var %s constant\n", vval.kind == EET_CONSTANT ? "IS" : "IS NOT");
            printf("    Typed to %u (%s)\n",
                   vval.type_id,
                   (vval.type_id == 0
                    ? "void"
                    : (vval.type_id == 1
                       ? "int"
                       : (vval.type_id == 2
                          ? "boolean"
                          : (vval.type_id == 3 ? "string" : "reference")))));

            if (vval.kind == EET_CONSTANT)
            {
                switch (vval.type_id) {
                case TYPEID_INT:
                case TYPEID_BOOL:
                {
                    printf("    Evaluated to: %ld\n", vval.u.cnst.numeric_val);
                } break;
                case TYPEID_STRING:
                {
                    printf("    Evaluated to: \"%s\"\n",
                           g_str_consts[vval.u.cnst.str_const_id].data);
                } break;
                case TYPEID_NULL:
                {
                    printf("    Evaluated to: \"NULL\"\n");
                } break;
                default:
                {
                } NOTREACHED;
                }
            }
#endif
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Ass:
    {
        evaled_expr e1 = eval_expr(s->u.ass_.expr_1);
        evaled_expr e2 = eval_expr(s->u.ass_.expr_2);
        if (UNLIKELY(!e1.is_lvalue))
        {
            error(get_lnum(e1.node), "Left side of assingment must be an lvalue");
        }
        else if (UNLIKELY(e1.type_id != e2.type_id)) // TODO: Handle the case with inheritance
        {
            // TODO: Copypase from is_Decl

            d_type expected_t = g_types[e1.type_id & (~TYPEID_FLAG_ARRAY)];
            d_type got_t = g_types[e2.type_id & (~TYPEID_FLAG_ARRAY)];
            error(get_lnum(e2.node),
                  "Variable of type \"%s\" is assigned with incompatible type \"%s\"",
                  expected_t.name, got_t.name);

            // Clarify why null is not implicitely assignable to a reference type:
            if (e2.type_id == TYPEID_NULL && e1.type_id > TYPEID_LAST_BUILTIN_TYPE)
            {
                note(get_lnum(e2.node),
                     "\"null\" is not implicitly assingable to a reference type. Use: \"(%s)null\"",
                     expected_t.name);
                note(get_lnum(e2.node),
                     "This requirement is forced by the Author of the assignment. Sorry.");
            }
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Ret:
    case is_VRet:
    {
        evaled_expr e;
        if (s->kind == is_VRet)
        {
            e.node = s;
            e.type_id = TYPEID_VOID;
            e.kind = EET_CONSTANT;
            e.is_lvalue = 0;
        }
        else
        {
            e = eval_expr(s->u.ret_.expr_);
        }

        if (UNLIKELY(e.type_id != return_type)) // TODO: Handle the case with inheritance
        {
            d_type expected_t = g_types[return_type & (~TYPEID_FLAG_ARRAY)];
            d_type got_t = g_types[e.type_id & (~TYPEID_FLAG_ARRAY)];
            error(get_lnum(e.node),
                  "Returning value of type \"%s\" incompatible with type \"%s\"",
                  got_t.name, expected_t.name);

            if ((e.type_id & (~TYPEID_FLAG_ARRAY)) > TYPEID_LAST_BUILTIN_TYPE)
            {
                note(got_t.lnum, "Type \"%s\" defined here", got_t.name);
            }
        }

        retval.all_branches_return = 1;
        return retval;
    }

    case is_Cond:
    case is_CondElse:
    {
        evaled_expr condex;
        evaled_stmt s1;
        evaled_stmt s2;
        if (s->kind == is_CondElse)
        {
            condex = eval_expr(s->u.condelse_.expr_);
            i32 blk1 = push_block();
            s1 = eval_stmt(s->u.condelse_.stmt_1, return_type, blk1);
            pop_block();
            i32 blk2 = push_block();
            s2 = eval_stmt(s->u.condelse_.stmt_2, return_type, blk2);
            pop_block();
        }
        else
        {
            condex = eval_expr(s->u.condelse_.expr_);
            i32 blk = push_block();
            s1 = eval_stmt(s->u.condelse_.stmt_1, return_type, blk);
            pop_block();
            s2 = create_empty_statement();
        }

        retval.all_branches_return =
            ((condex.kind == EET_CONSTANT && condex.u.cnst.numeric_val == 1 && s1.all_branches_return)
             || (condex.kind == EET_CONSTANT && condex.u.cnst.numeric_val == 0 && s2.all_branches_return)
             || (s1.all_branches_return && s2.all_branches_return));

        return retval;
    }

    case is_Incr:
    case is_Decr:
    {
        Expr e = s->kind == is_Incr ? s->u.incr_.expr_ : s->u.decr_.expr_;
        evaled_expr evaled_e = eval_expr(e);
        if (evaled_e.type_id != TYPEID_INT || !evaled_e.is_lvalue)
        {
            error(get_lnum(e), "Left-hand-side of %s must be an lvalue of type \"int\"",
                  s->kind == is_Incr ? "incrementation" : "decrementation");
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_While:
    {
        Expr condexpr = s->u.while_.expr_;
        evaled_expr condexpr_e = eval_expr(condexpr);

        i32 block_id = push_block();
        Stmt lbody = s->u.while_.stmt_;
        evaled_stmt lbody_e = eval_stmt(lbody, return_type, block_id);
        pop_block();

        if (condexpr_e.type_id != TYPEID_BOOL)
        {
            d_type t_given = g_types[condexpr_e.type_id & (~TYPEID_FLAG_ARRAY)];
            error(get_lnum(condexpr),
                  "Loop condition has a type %s, when boolean was expected",
                  t_given.name);
        }

        // Don't report missing reutrn if loop condition is always true -
        // function will never reach its end. We don't really care what returns
        // in the body, because we may still never enter the loop itself.
        retval.all_branches_return = (condexpr_e.kind == EET_CONSTANT
                                      && condexpr_e.type_id == TYPEID_BOOL
                                      && condexpr_e.u.cnst.numeric_val == 1);

        return retval;
    }

    case is_For:
    {
        char* type_name = s->u.for_.ident_1;
        char* vname = s->u.for_.ident_2;
        Expr e = s->u.for_.expr_;
        evaled_expr e_expr = eval_expr(e);

        if (!(e_expr.type_id & TYPEID_FLAG_ARRAY))
        {
            d_type t_given = g_types[e_expr.type_id & (~TYPEID_FLAG_ARRAY)];

            error(get_lnum(e), "Type in the \"for\" expresion must be a an array");
            note(get_lnum(e), "Given expression resolved to a type \"%s\"", t_given.name);

            // TODO: Change type to int to avoid error cascade?
            //       For example, when RHS of the for loop is void
        }
        else if ((e_expr.type_id & (~TYPEID_FLAG_ARRAY)) == TYPEID_VOID)
        {
            error(get_lnum(e), "Type in the \"for\" expresion must not be a void[]");

            // TODO: Change type to int to avoid error cascade?
            //       For example, when RHS of the for loop is void
        }

        u32 iter_type_id = symbol_resolve_type(type_name, 0, e);
        assert(iter_type_id == TYPEID_NOTFOUND || !(iter_type_id & TYPEID_FLAG_ARRAY));
        switch (iter_type_id) {
        case TYPEID_NOTFOUND:
        {
            // Default to good iterator type anyway
            iter_type_id = e_expr.type_id & ~TYPEID_FLAG_ARRAY;
            // TODO: "Assuming type ..."
        } break;
        case TYPEID_VOID:
        {
            error(get_lnum(e), "Cannot declare a variable of type \"void\" in the range loop");

            // Default to good iterator type anyway
            iter_type_id = e_expr.type_id & ~TYPEID_FLAG_ARRAY;
            // TODO: "Assuming type ..."
        } break;
        default:
        {
        } break;
        }

        if (iter_type_id != (e_expr.type_id & (~(TYPEID_FLAG_ARRAY))))
        {
            // TODO(ex): Support list of classes that inherit from the base class:
            //           for (base b : new dervied[10])

            d_type t_arr = g_types[e_expr.type_id & (~(TYPEID_FLAG_ARRAY))];
            d_type t_iter = g_types[iter_type_id & (~TYPEID_FLAG_ARRAY)];
            error(get_lnum(e), "Given array has a type, which is non assignable to the iterator type");
            note(get_lnum(e), "Iterator has type \"%s\", when \"%s\" was expected",
                 t_iter.name, t_arr.name);
        }

        i32 iter_block_id = push_block(); // For iterator variable
        d_var var;
        var.lnum = get_lnum(e);
        var.type_id = iter_type_id;
        var.block_id = iter_block_id;
        push_var(var, vname, e);
        i32 block_id = push_block(); // For body

        Stmt lbody = s->u.for_.stmt_;
        evaled_stmt lbody_e = eval_stmt(lbody, return_type, block_id);

        pop_block();
        pop_block();

        retval.all_branches_return = 0;
        return retval;
    }
    }

    NOTREACHED; // TODO: Should not be reached?
}

static void
eval_func_body(char* fnname, Block b, void* node)
{
    u32 f_id = symbol_resolve_func(fnname, node);
    assert(f_id != FUNCID_NOTFOUND); // Already added

    i32 param_block_id = push_block(); // Block in which params get defined
    i32 node_lnum = get_lnum(node);
    d_func_arg* args = g_funcs[f_id].arg_type_ids;
    mm n_args = array_size(args);
    for (mm i = 0; i < n_args; ++i)
    {
        d_var var;
        var.lnum = node_lnum; // TODO: lnum for funargs
        var.type_id = args[i].type_id;
        var.block_id = param_block_id;

        push_var(var, args[i].name, node);
    }

    u32 f_rettype_id = g_funcs[f_id].ret_type_id;
    b32 all_branches_return = 0;
    i32 body_block_id = push_block(); // Block where body is parsed (so args can be shadowed)
    assert(b->kind == is_Blk);
    LIST_FOREACH(it, b->u.blk_, liststmt_)
    {
        Stmt st = it->stmt_;
        evaled_stmt ev_s = eval_stmt(st, f_rettype_id, body_block_id);
        all_branches_return |= ev_s.all_branches_return;
    }

    // One for param_block one for body_block
    pop_block();
    pop_block();

    if (!all_branches_return && f_rettype_id != TYPEID_VOID)
        error(g_funcs[f_id].lnum, "Not all paths return a value in a non-void function");
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

        eval_func_body(t->u.fndef_.ident_, t->u.fndef_.block_, t->u.fndef_.type_);
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
                create_func(type.member_funcs[i], type.member_funcs[i].name);

            push_block(); // Block for class memebers
            mm n_members = array_size(type.members);
            for (mm i = 0; i < n_members; ++i)
            {
                char* vname = type.members[i].name;
                d_var var = {
                    .lnum = 0, // TODO: lnum in d_class_mem
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


            eval_func_body(cl->u.cbfndef_.ident_, cl->u.cbfndef_.block_, cl->u.cbfndef_.type_);

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
