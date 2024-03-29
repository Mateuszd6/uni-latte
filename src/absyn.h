#ifndef ABSYN_HEADER
#define ABSYN_HEADER

/* C++ Abstract Syntax Interface generated by the BNF Converter.*/

/********************   TypeDef Section    ********************/
typedef int Integer;
typedef char Char;
typedef double Double;
typedef char* String;
typedef char* Ident;

/********************   Forward Declarations    ********************/

struct ListTopDef_;
typedef struct ListTopDef_* ListTopDef;
struct ListArg_;
typedef struct ListArg_* ListArg;
struct ListStmt_;
typedef struct ListStmt_* ListStmt;
struct ListItem_;
typedef struct ListItem_* ListItem;
struct ListExpr_;
typedef struct ListExpr_* ListExpr;
struct ListClBody_;
typedef struct ListClBody_* ListClBody;
struct Program_;
typedef struct Program_* Program;
struct TopDef_;
typedef struct TopDef_* TopDef;
struct Arg_;
typedef struct Arg_* Arg;
struct Block_;
typedef struct Block_* Block;
struct Stmt_;
typedef struct Stmt_* Stmt;
struct Item_;
typedef struct Item_* Item;
struct Type_;
typedef struct Type_* Type;
struct Expr_;
typedef struct Expr_* Expr;
struct ClBody_;
typedef struct ClBody_* ClBody;
struct ClProps_;
typedef struct ClProps_* ClProps;
struct AddOp_;
typedef struct AddOp_* AddOp;
struct MulOp_;
typedef struct MulOp_* MulOp;
struct RelOp_;
typedef struct RelOp_* RelOp;
struct EqOp_;
typedef struct EqOp_* EqOp;

/********************   Abstract Syntax Classes    ********************/

struct Program_
{
    enum
    {
        is_Prog
    } kind;
    union {
        struct
        {
            ListTopDef listtopdef_;
        } prog_;
    } u;
};

Program
make_Prog(ListTopDef p0);

struct TopDef_
{
    enum
    {
        is_FnDef,
        is_ClDef
    } kind;
    union {
        struct
        {
            Block block_;
            Ident ident_;
            ListArg listarg_;
            Type type_;
        } fndef_;
        struct
        {
            ClProps clprops_;
            Ident ident_;
            ListClBody listclbody_;
        } cldef_;
    } u;
};

TopDef
make_FnDef(Type p0, Ident p1, ListArg p2, Block p3);
TopDef
make_ClDef(Ident p0, ClProps p1, ListClBody p2);

struct ListTopDef_
{
    TopDef topdef_;
    ListTopDef listtopdef_;
};

ListTopDef
make_ListTopDef(TopDef p1, ListTopDef p2);
struct Arg_
{
    enum
    {
        is_Ar
    } kind;
    union {
        struct
        {
            Ident ident_;
            Type type_;
        } ar_;
    } u;
};

Arg
make_Ar(Type p0, Ident p1);

struct ListArg_
{
    Arg arg_;
    ListArg listarg_;
};

ListArg
make_ListArg(Arg p1, ListArg p2);
struct Block_
{
    enum
    {
        is_Blk
    } kind;
    union {
        struct
        {
            ListStmt liststmt_;
        } blk_;
    } u;
};

Block
make_Blk(ListStmt p0);

struct ListStmt_
{
    Stmt stmt_;
    ListStmt liststmt_;
};

ListStmt
make_ListStmt(Stmt p1, ListStmt p2);
struct Stmt_
{
    enum
    {
        is_Empty,
        is_BStmt,
        is_Decl,
        is_Ass,
        is_Incr,
        is_Decr,
        is_Ret,
        is_VRet,
        is_Cond,
        is_CondElse,
        is_While,
        is_For,
        is_SExp
    } kind;
    union {
        struct
        {
            Block block_;
        } bstmt_;
        struct
        {
            ListItem listitem_;
            Type type_;
        } decl_;
        struct
        {
            Expr expr_1, expr_2;
        } ass_;
        struct
        {
            Expr expr_;
        } incr_;
        struct
        {
            Expr expr_;
        } decr_;
        struct
        {
            Expr expr_;
        } ret_;
        struct
        {
            Expr expr_;
            Stmt stmt_;
        } cond_;
        struct
        {
            Expr expr_;
            Stmt stmt_1, stmt_2;
        } condelse_;
        struct
        {
            Expr expr_;
            Stmt stmt_;
        } while_;
        struct
        {
            Expr expr_;
            Ident ident_1, ident_2;
            Stmt stmt_;
        } for_;
        struct
        {
            Expr expr_;
        } sexp_;
    } u;
};

Stmt
make_Empty(void);
Stmt
make_BStmt(Block p0);
Stmt
make_Decl(Type p0, ListItem p1);
Stmt
make_Ass(Expr p0, Expr p1);
Stmt
make_Incr(Expr p0);
Stmt
make_Decr(Expr p0);
Stmt
make_Ret(Expr p0);
Stmt
make_VRet(void);
Stmt
make_Cond(Expr p0, Stmt p1);
Stmt
make_CondElse(Expr p0, Stmt p1, Stmt p2);
Stmt
make_While(Expr p0, Stmt p1);
Stmt
make_For(Ident p0, Ident p1, Expr p2, Stmt p3);
Stmt
make_SExp(Expr p0);

struct Item_
{
    enum
    {
        is_NoInit,
        is_Init
    } kind;
    union {
        struct
        {
            Ident ident_;
        } noinit_;
        struct
        {
            Expr expr_;
            Ident ident_;
        } init_;
    } u;
};

Item
make_NoInit(Ident p0);
Item
make_Init(Ident p0, Expr p1);

struct ListItem_
{
    Item item_;
    ListItem listitem_;
};

ListItem
make_ListItem(Item p1, ListItem p2);
struct Type_
{
    enum
    {
        is_TCls,
        is_TArr
    } kind;
    union {
        struct
        {
            Ident ident_;
        } tcls_;
        struct
        {
            Ident ident_;
        } tarr_;
    } u;
};

Type
make_TCls(Ident p0);
Type
make_TArr(Ident p0);

struct Expr_
{
    enum
    {
        is_EVar,
        is_EApp,
        is_EClApp,
        is_EClMem,
        is_EArrApp,
        is_ENew,
        is_ENewArr,
        is_ELitInt,
        is_ELitStr,
        is_ELitTrue,
        is_ELitFalse,
        is_ENull,
        is_Neg,
        is_Not,
        is_ECast,
        is_EMul,
        is_EAdd,
        is_ERel,
        is_EEq,
        is_EAnd,
        is_EOr
    } kind;
    union {
        struct
        {
            Ident ident_;
        } evar_;
        struct
        {
            Ident ident_;
            ListExpr listexpr_;
        } eapp_;
        struct
        {
            Expr expr_;
            Ident ident_;
            ListExpr listexpr_;
        } eclapp_;
        struct
        {
            Expr expr_;
            Ident ident_;
        } eclmem_;
        struct
        {
            Expr expr_1, expr_2;
        } earrapp_;
        struct
        {
            Ident ident_;
        } enew_;
        struct
        {
            Expr expr_;
            Ident ident_;
        } enewarr_;
        struct
        {
            Integer integer_;
        } elitint_;
        struct
        {
            String string_;
        } elitstr_;
        struct
        {
            Expr expr_;
        } neg_;
        struct
        {
            Expr expr_;
        } not_;
        struct
        {
            Expr expr_;
            Ident ident_;
        } ecast_;
        struct
        {
            Expr expr_1, expr_2;
            MulOp mulop_;
        } emul_;
        struct
        {
            AddOp addop_;
            Expr expr_1, expr_2;
        } eadd_;
        struct
        {
            Expr expr_1, expr_2;
            RelOp relop_;
        } erel_;
        struct
        {
            EqOp eqop_;
            Expr expr_1, expr_2;
        } eeq_;
        struct
        {
            Expr expr_1, expr_2;
        } eand_;
        struct
        {
            Expr expr_1, expr_2;
        } eor_;
    } u;
};

Expr
make_EVar(Ident p0);
Expr
make_EApp(Ident p0, ListExpr p1);
Expr
make_EClApp(Expr p0, Ident p1, ListExpr p2);
Expr
make_EClMem(Expr p0, Ident p1);
Expr
make_EArrApp(Expr p0, Expr p1);
Expr
make_ENew(Ident p0);
Expr
make_ENewArr(Ident p0, Expr p1);
Expr
make_ELitInt(Integer p0);
Expr
make_ELitStr(String p0);
Expr
make_ELitTrue(void);
Expr
make_ELitFalse(void);
Expr
make_ENull(void);
Expr
make_Neg(Expr p0);
Expr
make_Not(Expr p0);
Expr
make_ECast(Ident p0, Expr p1);
Expr
make_EMul(Expr p0, MulOp p1, Expr p2);
Expr
make_EAdd(Expr p0, AddOp p1, Expr p2);
Expr
make_ERel(Expr p0, RelOp p1, Expr p2);
Expr
make_EEq(Expr p0, EqOp p1, Expr p2);
Expr
make_EAnd(Expr p0, Expr p1);
Expr
make_EOr(Expr p0, Expr p1);

struct ListExpr_
{
    Expr expr_;
    ListExpr listexpr_;
};

ListExpr
make_ListExpr(Expr p1, ListExpr p2);
struct ClBody_
{
    enum
    {
        is_CBVar,
        is_CBFnDef
    } kind;
    union {
        struct
        {
            Ident ident_;
            Type type_;
        } cbvar_;
        struct
        {
            Block block_;
            Ident ident_;
            ListArg listarg_;
            Type type_;
        } cbfndef_;
    } u;
};

ClBody
make_CBVar(Type p0, Ident p1);
ClBody
make_CBFnDef(Type p0, Ident p1, ListArg p2, Block p3);

struct ListClBody_
{
    ClBody clbody_;
    ListClBody listclbody_;
};

ListClBody
make_ListClBody(ClBody p1, ListClBody p2);
struct ClProps_
{
    enum
    {
        is_CNone,
        is_CExtends
    } kind;
    union {
        struct
        {
            Ident ident_;
        } cextends_;
    } u;
};

ClProps
make_CNone(void);
ClProps
make_CExtends(Ident p0);

struct AddOp_
{
    enum
    {
        is_Plus,
        is_Minus
    } kind;
    union {
    } u;
};

AddOp
make_Plus(void);
AddOp
make_Minus(void);

struct MulOp_
{
    enum
    {
        is_Times,
        is_Div,
        is_Mod
    } kind;
    union {
    } u;
};

MulOp
make_Times(void);
MulOp
make_Div(void);
MulOp
make_Mod(void);

struct RelOp_
{
    enum
    {
        is_LTH,
        is_LE,
        is_GTH,
        is_GE
    } kind;
    union {
    } u;
};

RelOp
make_LTH(void);
RelOp
make_LE(void);
RelOp
make_GTH(void);
RelOp
make_GE(void);

struct EqOp_
{
    enum
    {
        is_NE,
        is_EQU
    } kind;
    union {
    } u;
};

EqOp
make_NE(void);
EqOp
make_EQU(void);

#endif
