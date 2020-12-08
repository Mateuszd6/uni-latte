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

typedef struct processed_expr processed_expr;
struct processed_expr
{
    void* node; // For lnum in case of error reporting
    u32 type_id;
    processed_expr_t kind;
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

typedef struct processed_stmt processed_stmt;
struct processed_stmt
{
    b32 all_branches_return;
};

static processed_expr process_expr(Expr e);
static processed_stmt process_stmt(Stmt s, u32 return_type, i32 cur_block_id);
static void process_params(ListExpr arg_exprs, d_func* fun, void* node);
static void process_func_body(char* fnname, Block b, void* node);

static void add_classes(Program p);
static void add_class_members(Program p);
static void add_global_funcs(Program p);

#endif // FRONTEND_H_
