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
    MOV = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    MOD = 5,
    AND = 6,
    OR = 7,
    NOT = 8,
    CMP_LTH = 9,
    CMP_LE = 10,
    CMP_GTH = 11,
    CMP_GE = 12,
    CMP_EQ = 13,
    CMP_NEQ = 14,
    STR_EQ = 15,
    STR_NEQ = 16,
    LABEL = 17,
    JMP = 18,
    PARAM = 19,
    CALL = 20,
    RET = 21,
    SUBSCR = 22, // Array subscript arr[x] or class member obj.x
    ADDROF = 23,
    ALLOC = 24, // new
    STR_ADD = 25,
    JMP_TRUE = 26, // For logical operations
    JMP_FALSE = 27, // For logical operations

    // These will be explicitly inserted by the optimizer, and translate
    // directly to some x64 instructions
    CMP_SET_FLAGS = 28,
    JMP_FLAGS_E = 29,
    JMP_FLAGS_NE = 30,
    JMP_FLAGS_L = 31,
    JMP_FLAGS_LE = 32,
    JMP_FLAGS_G = 33,
    JMP_FLAGS_GE = 34,

    // These are optimisation-specific instructions
    NOP = 35,
    DISPOSE = 36,

    // Extensions
    // TODO: Different ALLOC for objs
    STORE = 37,
    ARR_SET_END = 38,
    ADD_AT_ADDR = 39,
    SET_VTAB = 40,
    VIRTCALL = 41,
    CLEANUP = 42,
};
typedef enum ir_op ir_op;

static char const* const ir_op_name[] = {
    "MOV", "ADD", "SUB", "MUL", "DIV", "MOD", "AND", "OR", "NOT",
    "CMP_LTH", "CMP_LE", "CMP_GTH", "CMP_GE", "CMP_EQ", "CMP_NEQ", "STR_EQ", "STR_NEQ",
    "LABEL", "JMP", "PARAM", "CALL", "RET",
    "SUBSCR", "ADDROF", "ALLOC", "STR_ADD", "JMP_TRUE", "JMP_FALSE",
    "CMP_SET_FLAGS", "JMP_FLAGS_E", "JMP_FLAGS_NE",
    "JMP_FLAGS_L", "JMP_FLAGS_LE", "JMP_FLAGS_G", "JMP_FLAGS_GE",
    "NOP", "DISPOSE", "STORE", "ARR_SET_END", "ADD_AT_ADDR", "SET_VTAB",
    "VIRTCALL", "CLEANUP",
};

static int const ir_op_n_args[] = {
    1, 2, 2, 2, 2, 2, 2, 2, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
    1, 1, 2, 2, 1, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 0, 1, 2, 1, 2,
    2, 2, 1,
};

STATIC_ASSERT(COUNT_OF(ir_op_n_args) == COUNT_OF(ir_op_name), arrays_dont_match);

enum ir_val_type
{
    IRVT_NONE = 0,
    IRVT_VAR = 1,
    IRVT_FNPARAM = 2,
    IRVT_TEMP = 3,
    IRVT_FN = 4,
    IRVT_LOCFN = 5,
    IRVT_CONST = 6,
    IRVT_STRCONST = 7,

};
typedef enum ir_val_type ir_val_type;

static char const* const ir_val_type_name[] =
{
    "NONE" , "v_", "p_", "t_", "gf:", "lf:", "", "str:"
};

typedef struct ir_val ir_val;
struct ir_val
{
    union
    {
        mm reg_id; // In case of VARIABLE or TEMP
        i64 constant; // For CONSTANT; value or number of string constant
    } u;
    ir_val_type type;
};

typedef struct ir_quadr ir_quadr;
struct ir_quadr
{
    // The single IR quadruple like: target = arg1 `op` arg2
    ir_op op; // The opcode
    ir_val target; // taget can be only variable or temp (not constant)

    union
    {
        struct
        {
            ir_val arg1;
            ir_val arg2;
        } a;
        ir_val args[2];
    } u;
};

typedef struct processed_expr processed_expr;
struct processed_expr
{
    void* node; // For lnum in case of error reporting
    u32 type_id;
    processed_expr_t kind;
    ir_val val;
    b8 is_lvalue;
    b8 is_addr;
};

typedef struct preprocessed_jump_expr preprocessed_jump_expr;
struct preprocessed_jump_expr
{
    enum
    {
        PRJE_EXPR,
        PRJE_CONST,
        PRJE_BIN,
    } kind;
    union
    {
        Expr expr;
        u64 constant;
        struct
        {
            mm b1; // Offset in the buffer, where evaluated expressions are
            mm b2;
            enum
            {
                BIN_OP_AND,
                BIN_OP_OR,
            } op;
        } bin;
    } u;
    i32 l_id;
    b32 reversed;
};


typedef struct processed_stmt processed_stmt;
struct processed_stmt
{
    b32 all_branches_return;
};

typedef struct jump_ctx jump_ctx;
struct jump_ctx
{
    i32 l_true;
    i32 l_false;
};

static void process_jumping_expr(
    ir_quadr** ir, preprocessed_jump_expr* e, preprocessed_jump_expr* const pre_buf, jump_ctx ctx);
static preprocessed_jump_expr preprocess_jumping_expr(
    Expr e, preprocessed_jump_expr** buf, b32 reverse);
static processed_expr process_expr(Expr e, ir_quadr** ir, b32 addr_only);
static processed_stmt process_stmt(Stmt s, u32 return_type, i32 cur_block_id, ir_quadr** ir);
static void process_params(ListExpr arg_exprs, d_func* fun, void* node, ir_quadr** ir);
static void process_func_body(char* fnname, Block b, void* node);

static void add_classes(Program p);
static void add_global_funcs(Program p);
static void add_class_members_and_local_funcs(i32* types, i32* exts);
static void check_global_funcs(Program p);
static void check_class_funcs(Program p);

#define IR_CONSTANT(VALUE)                                                     \
    { .type = IRVT_CONST, .u = { .reg_id = (VALUE) } }

#define IR_LOCAL_VARIABLE(VAR_ID)                                              \
    { .type = IRVT_VAR, .u = { .reg_id = VAR_ID } }

#define IR_FUNC_PARAM(VAR_ID)                                                  \
    { .type = IRVT_FNPARAM, .u = { .reg_id = VAR_ID } }

#define IR_NEXT_TEMP_REGISTER()                                                \
    { .type = IRVT_TEMP, .u = { .reg_id = g_temp_reg++ } }

#define IR_EMPTY()                                                             \
    { .type = IRVT_NONE }

// Used when we've reported error, and won't generate IR anyway, but we want to
// continue the flow (as we assume, that most programs won't have errors, and we
// optimize of the most frequent operations anyway)
#define IR_UNDEFINED_EXPR(EXPR, TYPE, IS_LVALUE)                               \
    do                                                                         \
    {                                                                          \
        ir_val val_ = {                                                        \
            .type = IRVT_CONST,                                                \
            .u = { .constant = 0 }                                             \
        };                                                                     \
                                                                               \
        EXPR .type_id = TYPE;                                                  \
        EXPR .kind = EET_COMPUTE;                                              \
        EXPR .is_lvalue = IS_LVALUE;                                           \
        EXPR .is_addr = 0;                                                     \
        EXPR .val = val_;                                                      \
        IR_PUSH(EXPR .val, MOV, val_);                                         \
    } while (0)

#define IR_SET_CONSTANT(EXPR, TYPE, CONSTANT_VALUE)                            \
    do                                                                         \
    {                                                                          \
        ir_val val_ = {                                                        \
            .type = IRVT_CONST,                                                \
            .u = { .constant = CONSTANT_VALUE }                                \
        };                                                                     \
                                                                               \
        EXPR .type_id = TYPE;                                                  \
        EXPR .kind = EET_CONSTANT;                                             \
        EXPR .is_lvalue = 0;                                                   \
        EXPR .is_addr = 0;                                                     \
        EXPR .val = val_;                                                      \
    } while (0)

#define IR_SET_STRCONST(EXPR, TYPE, CONSTANT_VALUE)                            \
    do                                                                         \
    {                                                                          \
        ir_val val_ = {                                                        \
            .type = IRVT_STRCONST,                                             \
            .u = { .constant = CONSTANT_VALUE }                                \
        };                                                                     \
                                                                               \
        EXPR .type_id = TYPE;                                                  \
        EXPR .kind = EET_CONSTANT;                                             \
        EXPR .is_lvalue = 0;                                                   \
        EXPR .is_addr = 0;                                                     \
        EXPR .val = val_;                                                      \
    } while (0)

#define IR_SET_EXPR(EXPR, TYPE, IS_LVALUE, TO_SET)                             \
    do                                                                         \
    {                                                                          \
        ir_val val_ = TO_SET ;                                                 \
        EXPR .type_id = TYPE;                                                  \
        EXPR .kind = EET_COMPUTE;                                              \
        EXPR .is_lvalue = IS_LVALUE;                                           \
        EXPR .is_addr = IS_LVALUE;                                             \
        EXPR .val = val_;                                                      \
    } while (0)

#define IR_PUSH2(TARG, OPCODE, ARG1, ARG2)                                     \
    do                                                                         \
    {                                                                          \
        ir_quadr quadr_impl_;                                                  \
        ir_val targ_ = TARG;                                                   \
        ir_val arg1_ = ARG1;                                                   \
        ir_val arg2_ = ARG2;                                                   \
        quadr_impl_.op = OPCODE;                                               \
        quadr_impl_.target = targ_;                                            \
        quadr_impl_.u.a.arg1 = arg1_;                                          \
        quadr_impl_.u.a.arg2 = arg2_;                                          \
        array_push(*ir, quadr_impl_);                                          \
    } while (0)

#define IR_PUSH1(TARG, OPCODE, ARG1)                                           \
    do                                                                         \
    {                                                                          \
        ir_quadr quadr_impl_;                                                  \
        ir_val targ_ = TARG;                                                   \
        quadr_impl_.op = OPCODE;                                               \
        quadr_impl_.target = targ_;                                            \
        quadr_impl_.u.a.arg1 = ARG1;                                           \
        array_push(*ir, quadr_impl_);                                          \
    } while (0)

#define IR_PUSH0(TARG, OPCODE)                                                 \
    do                                                                         \
    {                                                                          \
        ir_quadr quadr_impl_;                                                  \
        quadr_impl_.op = OPCODE;                                               \
        quadr_impl_.target = TARG;                                             \
        array_push(*ir, quadr_impl_);                                          \
    } while (0)

#define IR_ASSIGNMENT_OR_STORE(VAR, R, IS_ADDR)                                \
    do                                                                         \
    {                                                                          \
        if (IS_ADDR)                                                           \
            IR_PUSH(IR_EMPTY(), STORE, VAR, R);                                \
        else                                                                   \
            IR_PUSH(VAR, MOV, R);                                              \
    } while (0)

// This is a famous macro overloading trick:
#define IR_GET_OVERLOADED_MACRO(_1,_2,_3,_4,NAME,...) NAME
#define IR_PUSH(...)                                                           \
    IR_GET_OVERLOADED_MACRO(                                                   \
        __VA_ARGS__, IR_PUSH2, IR_PUSH1, IR_PUSH0)(__VA_ARGS__)

#endif // FRONTEND_H_
