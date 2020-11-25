// TODO:
// * regenerate parser on students, b/c current have different bison version
// * make a frontend.h/c file
// * check if type error work fine for classes
// * typecheck actual code
// * looks like if a constant is too large, -1 is retuend, check and implement accordingly
// * void[] return type / param of the function? void arguments
// * int main() { int int; int x; } allows to redefine int to a variable...

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
get_default_expr_constant(u32 type_id, void* node)
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
        if (type_id & TYPEID_FLAG_ARRAY)
        {
            // TODO: Return an empty array of type
            NOTREACHED;
        }
        else
        {
            // TODO: Return a null of apropiate class type
            NOTREACHED;
        }
    } break;
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

    // Safe for booleans becasue they never overflow
    if (UNLIKELY(overflows_32bit(retval.u.cnst.numeric_val)))
    {
        // TODO: Handle overflow - return constnat expression of 0? Or
        //       computable expression?
        fatal("Constant integer overflow");
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
    array_push(g_local_symbols, 0);
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
        d_type t_expected = g_types[type_id];
        d_type t_got = g_types[e->type_id];

        // TODO: lnum
        error(get_lnum(e->node),
              "expected type \"%s\" but got incompatible type \"%s\"",
              t_expected.name, t_got.name);

        // TODO: Set COMPUTE to true and add some fake value so that we carry on
        //       with a compilation Or move it outside the func, bc/
        //       enforce_type_of can't do it
    }
}

static void
enforce_type_of(evaled_expr* e, u32* type_ids, mm n_type_ids)
{
    for (mm i = 0; i < n_type_ids; ++i)
        if (LIKELY(e->type_id == type_ids[i]))
            return;

    char buf[4096]; // TODO: may index-out
    d_type t_got = g_types[e->type_id];
    buf[0] = 0;
    for (mm i = 1; i < n_type_ids; ++i)
    {
        d_type t_expected = g_types[type_ids[i]];

        strcat(buf, ", \"");
        strcat(buf, t_expected.name);
        strcat(buf, "\"");
    }

    d_type first_t_expected = g_types[type_ids[0]];
    error(get_lnum(e->node),
          "expected one of \"%s\"%s but got incompatible type \"%s\"",
          first_t_expected.name, buf, t_got.name);
}

// TODO: Rename eval_ -> ?

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
            enforce_type(&e2, TYPEID_INT);
            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        case TYPEID_BOOL:
        {
            enforce_type(&e2, TYPEID_BOOL);
            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO
        }

        case TYPEID_STRING:
        {
            enforce_type(&e2, TYPEID_STRING);
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

        default:
        {
            // This will produce an error msg
            u32 allowed_types[] = { TYPEID_INT, TYPEID_BOOL, TYPEID_STRING };
            enforce_type_of(&e1, allowed_types, COUNT_OF(allowed_types));

            // TODO: return something
        } break;
        }

    } break;

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
    case is_ECast:
    case is_EClMem:
    case is_EArrApp:
    case is_ENew:
    case is_ENewArr:
    case is_ENull:
        NOTREACHED;
    }

    NOTREACHED;
};

static evaled_stmt
eval_stmt(Stmt s, u32 return_type, i32 cur_block_id)
{
#if 0
    printf("--\n");
    printf("Statement (type: %d)\n", s->kind);
#endif

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
        printf("    Expression %s constant\n", ee.kind == EET_CONSTANT ? "IS" : "IS NOT");
        printf("    Typed to %u (%s)\n",
               ee.type_id,
               (ee.type_id == 0
                ? "void"
                : (ee.type_id == 1
                   ? "int"
                   : (ee.type_id == 2
                      ? "boolean"
                      : (ee.type_id == 3 ? "string" : "reference")))));

        if (ee.kind == EET_CONSTANT)
        {
            switch (ee.type_id) {
            case TYPEID_INT:
            case TYPEID_BOOL:
            {
                printf("    Evaluated to: %ld\n", ee.u.cnst.numeric_val);
            } break;
            case TYPEID_STRING:
            {
                printf("    Evaluated to: \"%s\"\n",
                       g_str_consts[ee.u.cnst.str_const_id].data);
            } break;
            default:
            {
            } NOTREACHED;
            }
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Decl:
    {
        Type var_type = s->u.decl_.type_;
        char* type_name;
        b32 is_array;
        switch (var_type->kind) { // TODO: Copypaste
        case is_TCls:
        {
            type_name = var_type->u.tcls_.ident_;
            is_array = 0;
        } break;
        case is_TArr:
        {
            type_name = var_type->u.tarr_.ident_;
            is_array = 1;
        } break;
        }

        u32 type_id = symbol_resolve_type(type_name, is_array, var_type);
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // Default to int in order to avoid errors
        } break;
        case TYPEID_VOID:
        case TYPEID_VOID | TYPEID_FLAG_ARRAY:
        {
            error(get_lnum(var_type), "Cannot declare a variable of type \"void%s\"",
                  type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

            type_id = TYPEID_VOID; // If void[] change to void, to aVOID errors
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
                vval = get_default_expr_constant(type_id, i);
            } break;
            case is_Init:
            {
                vname = i->u.init_.ident_;
                vval = eval_expr(i->u.init_.expr_);

                if (type_id != vval.type_id) // TODO: Handle the case with inheritance
                {
                    d_type expected_t = g_types[type_id];
                    d_type got_t = g_types[vval.type_id];
                    error(get_lnum(i),
                          "Expression of type \"%s\" is initialized with uncompatible type \"%s\"\n",
                          expected_t.name, got_t.name);
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
            d_type expected_t = g_types[e1.type_id];
            d_type got_t = g_types[e2.type_id];
            error(get_lnum(e2.node),
                  "Variable of type \"%s\" is assigned with uncompatible type \"%s\"\n",
                  expected_t.name, got_t.name);
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Ret: // TODO: Return value
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

        if (UNLIKELY(e.type_id != TYPEID_INT)) // TODO: Handle the case with inheritance
        {
            d_type expected_t = g_types[e.type_id];
            d_type got_t = g_types[TYPEID_INT];
            error(get_lnum(e.node),
                  "Returning value of type \"%s\" incompatible with type \"%s\"\n",
                  expected_t.name, got_t.name);
        }

        retval.all_branches_return = 1;
        return retval;
    }

    case is_Cond: // TODO: If stmt
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

    case is_Incr: // TODO: Later
    case is_Decr:

    case is_While: // TODO: Loops
    case is_For:

        NOTREACHED;
    }

    return retval; // TODO: Should not be reached
}

static void
eval_body(TopDef fundef)
{
    assert(fundef->kind == is_FnDef);
    Ident fnname = fundef->u.fndef_.ident_;
    u32 f_id = symbol_resolve_func(fnname, fundef);
    assert(f_id != FUNCID_NOTFOUND);

    i32 param_block_id = push_block(); // block in which params get defined
    LIST_FOREACH(arg_it, fundef->u.fndef_, listarg_)
    {
        // TODO: Copypaste
        Arg a = arg_it->arg_;
        Ident aname = a->u.ar_.ident_;
        Type atype = a->u.ar_.type_;
        Ident arg_type_name;
        b32 arg_is_array;
        switch (atype->kind) {
        case is_TCls:
        {
            arg_type_name = atype->u.tcls_.ident_;
            arg_is_array = 0;
        } break;
        case is_TArr:
        {
            arg_type_name = atype->u.tarr_.ident_;
            arg_is_array = 1;
        } break;
        }

        u32 arg_type_id = symbol_resolve_type(atype->u.tcls_.ident_, arg_is_array, atype);
        assert(arg_type_id != TYPEID_NOTFOUND); // Already checked earlier

        d_var var;
        var.lnum = get_lnum(a);
        var.type_id = arg_type_id;
        var.block_id = param_block_id;

        push_var(var, aname, a);
    }

    u32 f_rettype_id = g_funcs[f_id].ret_type_id;
    Block b = fundef->u.fndef_.block_;
    b32 all_branches_return = 0;
    i32 body_block_id = push_block(); // block where body is parsed (so args can be shadowed)
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

    add_primitive_types();
    add_classes(parse_tree);
    add_global_funcs(parse_tree);

    if (has_error)
        no_recover();

#if 0 && DEBUG
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
    printf("\n");
#endif


    LIST_FOREACH(it, parse_tree->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_FnDef)
            continue;

#if 0
        printf("-- ----------------------------------------------------------------\n");
        printf("-- Evaluating body of function \"%s\"\n", t->u.fndef_.ident_);
        printf("-- ----------------------------------------------------------------\n");
#endif

        eval_body(t);
    }

    if (has_error)
        no_recover();

    accept_input();
    return 0;
}
