#ifndef FRONTEND_H_
#define FRONTEND_H_

#include "absyn.h"
#include "misc.h"
#include "symtab.h"

enum processed_expr_t
{
    EET_COMPUTE,
    EET_CONSTANT,
};
typedef enum processed_expr_t processed_expr_t;

enum ir_op
{
    IR_MOV,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    IR_MOD,
    IR_AND,
    IR_OR,
    IR_NEG,
    IR_CMP_LTH,
    IR_CMP_LE,
    IR_CMP_GTH,
    IR_CMP_GE,
    IR_EQ,
    IR_STR_EQ,
    IR_LABEL,
    IR_JUMP,
    IR_PARAM,
    IR_CALL,
    IR_RET,
    IR_SUBSCR, // Array subscript arr[x] or class member obj.x
    IR_ARR_LEN,
    IR_ALLOC, // new
};
typedef enum ir_op ir_op;

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
typedef enum binary_int_op_t binary_int_op_t;

enum binary_bool_op_t
{
    BBOP_AND,
    BBOP_OR,
};
typedef enum binary_bool_op_t binary_bool_op_t;

enum binary_eq_op_t
{
    BEOP_EQ,
    BEOP_NEQ,
};
typedef enum binary_eq_op_t binary_eq_op_t;

enum ir_val_type
{
    IRVT_VAR,
    IRVT_FNPARAM,
    IRVT_TEMP,
    IRVT_CONST,
};
typedef enum ir_val_type ir_val_type;

typedef struct ir_val ir_val;
struct ir_val
{
    ir_val_type type;
    union
    {
        mm reg_id; // In case of VARIABLE or TEMP
        i64 constant; // For CONSTANT; value or number of string constant
    } u;
};

typedef struct ir_quadr ir_quadr;
struct ir_quadr
{
    // The single IR quadruple like: target = arg1 `op` arg2
    ir_op op; // The opcode
    ir_val target; // taget can be only variable or temp (not constant)
    ir_val arg1;
    ir_val arg2;
};

typedef struct processed_expr processed_expr;
struct processed_expr
{
    void* node; // For lnum in case of error reporting
    u32 type_id;
    processed_expr_t kind;
    b32 is_lvalue; // TODO: Try to remove it? Temp registers are not, variable and fparams are?
    ir_val val;

    // TODO: Try to get rid of this:
    union
    {
        mm reg_id;
        i64 numeric_val; // int or boolean
        mm str_const_id; // string constant
    } u;
};

typedef struct processed_stmt processed_stmt;
struct processed_stmt
{
    b32 all_branches_return;
};

static processed_expr process_expr(Expr e, ir_quadr** ir);
static processed_stmt process_stmt(Stmt s, u32 return_type, i32 cur_block_id, ir_quadr** ir);
static void process_params(ListExpr arg_exprs, d_func* fun, void* node, ir_quadr** ir);
static void process_func_body(char* fnname, Block b, void* node);

static void add_classes(Program p);
static void add_global_funcs(Program p);
static void add_class_members_and_local_funcs(Program p);
static void check_global_funcs(Program p);
static void check_class_funcs(Program p);

#define IR_NEXT_TEMP_REGISTER()                                                \
    { .type = IRVT_TEMP, .u = { .reg_id = g_temp_reg++ } }

#define IR_SET_CONSTANT(EXPR, TYPE, CONSTANT_VALUE)                            \
    do                                                                         \
    {                                                                          \
        ir_val val_ = {                                                        \
            .type = IRVT_CONST,                                                \
            .u = { .constant = CONSTANT_VALUE }                                \
        };                                                                     \
                                                                               \
        EXPR.u.numeric_val = CONSTANT_VALUE;                                   \
        EXPR .type_id = TYPE;                                                  \
        EXPR .kind = EET_CONSTANT;                                             \
        EXPR .is_lvalue = 0;                                                   \
        EXPR .val = val_;                                                      \
    } while (0)

#define IR_SET_EXPR(EXPR, TYPE, IS_LVALUE, TO_SET)                             \
    do                                                                         \
    {                                                                          \
        ir_val val_ = TO_SET ;                                                 \
        EXPR .type_id = TYPE;                                                  \
        EXPR .kind = EET_COMPUTE;                                              \
        EXPR .is_lvalue = IS_LVALUE;                                           \
        EXPR .val = val_;                                                      \
    } while (0)

#define IR_PUSH2(TARG, OPCODE, ARG1, ARG2)                                     \
    do                                                                         \
    {                                                                          \
        ir_quadr quadr_impl_;                                                  \
        quadr_impl_.op = IR_ ## OPCODE;                                        \
        quadr_impl_.target = (TARG);                                           \
        quadr_impl_.arg1 = (ARG1);                                             \
        quadr_impl_.arg2 = (ARG2);                                             \
        array_push(*ir, quadr_impl_);                                          \
    } while (0)

#define IR_PUSH1(TARG, OPCODE, ARG1)                                           \
    do                                                                         \
    {                                                                          \
        ir_quadr quadr_impl_;                                                  \
        quadr_impl_.op = IR_ ## OPCODE;                                        \
        quadr_impl_.target = (TARG);                                           \
        quadr_impl_.arg1 = (ARG1);                                             \
        array_push(*ir, quadr_impl_);                                          \
    } while (0)

#define IR_PUSH0(TARG, OPCODE)                                                 \
    do                                                                         \
    {                                                                          \
        ir_quadr quadr_impl_;                                                  \
        quadr_impl_.op = IR_ ## OPCODE;                                        \
        quadr_impl_.target = (TARG);                                           \
        array_push(*ir, quadr_impl_);                                          \
    } while (0)

// This is a famous macro overloading trick:
#define IR_GET_OVERLOADED_MACRO(_1,_2,_3,_4,NAME,...) NAME
#define IR_PUSH(...)                                                           \
    IR_GET_OVERLOADED_MACRO(                                                   \
        __VA_ARGS__, IR_PUSH2, IR_PUSH1, IR_PUSH0)(__VA_ARGS__)

#endif // FRONTEND_H_
