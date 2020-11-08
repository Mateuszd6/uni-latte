/* This Bison file was machine-generated by BNFC */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "absyn.h"
#define initialize_lexer latte_initialize_lexer
extern int yyparse(void);
extern int yylex(void);
int yy_mylinenumber;
extern int initialize_lexer(FILE * inp);
void yyerror(const char *str)
{
  extern char *lattetext;
  fprintf(stderr,"ERROR\ntest.lat:%d: %s at \"%s\"\n", // TODO: filename
    yy_mylinenumber + 1, str, lattetext);
}

Program YY_RESULT_Program_ = 0;
Program pProgram(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}

TopDef YY_RESULT_TopDef_ = 0;
TopDef pTopDef(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_TopDef_;
  }
}

ListTopDef YY_RESULT_ListTopDef_ = 0;
ListTopDef pListTopDef(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListTopDef_;
  }
}

Arg YY_RESULT_Arg_ = 0;
Arg pArg(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Arg_;
  }
}

ListArg YY_RESULT_ListArg_ = 0;
ListArg pListArg(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArg_;
  }
}

Block YY_RESULT_Block_ = 0;
Block pBlock(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Block_;
  }
}

ListStmt YY_RESULT_ListStmt_ = 0;
ListStmt pListStmt(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListStmt_;
  }
}

Stmt YY_RESULT_Stmt_ = 0;
Stmt pStmt(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Stmt_;
  }
}

Item YY_RESULT_Item_ = 0;
Item pItem(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Item_;
  }
}

ListItem YY_RESULT_ListItem_ = 0;
ListItem pListItem(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListItem_;
  }
}

Type YY_RESULT_Type_ = 0;
Type pType(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}

ListType YY_RESULT_ListType_ = 0;
ListType pListType(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListType_;
  }
}

Expr YY_RESULT_Expr_ = 0;
Expr pExpr(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Expr_;
  }
}

ListExpr YY_RESULT_ListExpr_ = 0;
ListExpr pListExpr(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExpr_;
  }
}

AddOp YY_RESULT_AddOp_ = 0;
AddOp pAddOp(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_AddOp_;
  }
}

MulOp YY_RESULT_MulOp_ = 0;
MulOp pMulOp(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_MulOp_;
  }
}

RelOp YY_RESULT_RelOp_ = 0;
RelOp pRelOp(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_RelOp_;
  }
}


ListTopDef reverseListTopDef(ListTopDef l)
{
  ListTopDef prev = 0;
  ListTopDef tmp = 0;
  while (l)
  {
    tmp = l->listtopdef_;
    l->listtopdef_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListArg reverseListArg(ListArg l)
{
  ListArg prev = 0;
  ListArg tmp = 0;
  while (l)
  {
    tmp = l->listarg_;
    l->listarg_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListStmt reverseListStmt(ListStmt l)
{
  ListStmt prev = 0;
  ListStmt tmp = 0;
  while (l)
  {
    tmp = l->liststmt_;
    l->liststmt_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListItem reverseListItem(ListItem l)
{
  ListItem prev = 0;
  ListItem tmp = 0;
  while (l)
  {
    tmp = l->listitem_;
    l->listitem_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListType reverseListType(ListType l)
{
  ListType prev = 0;
  ListType tmp = 0;
  while (l)
  {
    tmp = l->listtype_;
    l->listtype_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListExpr reverseListExpr(ListExpr l)
{
  ListExpr prev = 0;
  ListExpr tmp = 0;
  while (l)
  {
    tmp = l->listexpr_;
    l->listexpr_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}

%}

%union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  Program program_;
  TopDef topdef_;
  ListTopDef listtopdef_;
  Arg arg_;
  ListArg listarg_;
  Block block_;
  ListStmt liststmt_;
  Stmt stmt_;
  Item item_;
  ListItem listitem_;
  Type type_;
  ListType listtype_;
  Expr expr_;
  ListExpr listexpr_;
  AddOp addop_;
  MulOp mulop_;
  RelOp relop_;

}

%token _ERROR_
%token _SYMB_0    /*   (   */
%token _SYMB_1    /*   )   */
%token _SYMB_2    /*   ,   */
%token _SYMB_3    /*   {   */
%token _SYMB_4    /*   }   */
%token _SYMB_5    /*   ;   */
%token _SYMB_6    /*   =   */
%token _SYMB_7    /*   ++   */
%token _SYMB_8    /*   --   */
%token _SYMB_9    /*   -   */
%token _SYMB_10    /*   !   */
%token _SYMB_11    /*   &&   */
%token _SYMB_12    /*   ||   */
%token _SYMB_13    /*   +   */
%token _SYMB_14    /*   *   */
%token _SYMB_15    /*   /   */
%token _SYMB_16    /*   %   */
%token _SYMB_17    /*   <   */
%token _SYMB_18    /*   <=   */
%token _SYMB_19    /*   >   */
%token _SYMB_20    /*   >=   */
%token _SYMB_21    /*   ==   */
%token _SYMB_22    /*   !=   */
%token _SYMB_23    /*   boolean   */
%token _SYMB_24    /*   else   */
%token _SYMB_25    /*   false   */
%token _SYMB_26    /*   if   */
%token _SYMB_27    /*   int   */
%token _SYMB_28    /*   return   */
%token _SYMB_29    /*   string   */
%token _SYMB_30    /*   true   */
%token _SYMB_31    /*   void   */
%token _SYMB_32    /*   while   */

%type <program_> Program
%type <topdef_> TopDef
%type <listtopdef_> ListTopDef
%type <arg_> Arg
%type <listarg_> ListArg
%type <block_> Block
%type <liststmt_> ListStmt
%type <stmt_> Stmt
%type <item_> Item
%type <listitem_> ListItem
%type <type_> Type
%type <listtype_> ListType
%type <expr_> Expr6
%type <expr_> Expr5
%type <expr_> Expr4
%type <expr_> Expr3
%type <expr_> Expr2
%type <expr_> Expr1
%type <expr_> Expr
%type <listexpr_> ListExpr
%type <addop_> AddOp
%type <mulop_> MulOp
%type <relop_> RelOp

%token<string_> _STRING_
%token<int_> _INTEGER_
%token<string_> _IDENT_

%start Program
%%
Program : ListTopDef { $$ = make_Prog($1); YY_RESULT_Program_= $$; } 
;
TopDef : Type _IDENT_ _SYMB_0 ListArg _SYMB_1 Block { $$ = make_FnDef($1, $2, $4, $6);  } 
;
ListTopDef : TopDef { $$ = make_ListTopDef($1, 0);  } 
  | TopDef ListTopDef { $$ = make_ListTopDef($1, $2);  }
;
Arg : Type _IDENT_ { $$ = make_Ar($1, $2);  } 
;
ListArg : /* empty */ { $$ = 0;  } 
  | Arg { $$ = make_ListArg($1, 0);  }
  | Arg _SYMB_2 ListArg { $$ = make_ListArg($1, $3);  }
;
Block : _SYMB_3 ListStmt _SYMB_4 { $$ = make_Blk(reverseListStmt($2));  } 
;
ListStmt : /* empty */ { $$ = 0;  } 
  | ListStmt Stmt { $$ = make_ListStmt($2, $1);  }
;
Stmt : _SYMB_5 { $$ = make_Empty();  } 
  | Block { $$ = make_BStmt($1);  }
  | Type ListItem _SYMB_5 { $$ = make_Decl($1, $2);  }
  | _IDENT_ _SYMB_6 Expr _SYMB_5 { $$ = make_Ass($1, $3);  }
  | _IDENT_ _SYMB_7 _SYMB_5 { $$ = make_Incr($1);  }
  | _IDENT_ _SYMB_8 _SYMB_5 { $$ = make_Decr($1);  }
  | _SYMB_28 Expr _SYMB_5 { $$ = make_Ret($2);  }
  | _SYMB_28 _SYMB_5 { $$ = make_VRet();  }
  | _SYMB_26 _SYMB_0 Expr _SYMB_1 Stmt { $$ = make_Cond($3, $5);  }
  | _SYMB_26 _SYMB_0 Expr _SYMB_1 Stmt _SYMB_24 Stmt { $$ = make_CondElse($3, $5, $7);  }
  | _SYMB_32 _SYMB_0 Expr _SYMB_1 Stmt { $$ = make_While($3, $5);  }
  | Expr _SYMB_5 { $$ = make_SExp($1);  }
;
Item : _IDENT_ { $$ = make_NoInit($1);  } 
  | _IDENT_ _SYMB_6 Expr { $$ = make_Init($1, $3);  }
;
ListItem : Item { $$ = make_ListItem($1, 0);  } 
  | Item _SYMB_2 ListItem { $$ = make_ListItem($1, $3);  }
;
Type : _SYMB_27 { $$ = make_Int();  } 
  | _SYMB_29 { $$ = make_Str();  }
  | _SYMB_23 { $$ = make_Bool();  }
  | _SYMB_31 { $$ = make_Void();  }
;
ListType : /* empty */ { $$ = 0;  } 
  | Type { $$ = make_ListType($1, 0);  }
  | Type _SYMB_2 ListType { $$ = make_ListType($1, $3);  }
;
Expr6 : _IDENT_ { $$ = make_EVar($1);  } 
  | _INTEGER_ { $$ = make_ELitInt($1);  }
  | _SYMB_30 { $$ = make_ELitTrue();  }
  | _SYMB_25 { $$ = make_ELitFalse();  }
  | _IDENT_ _SYMB_0 ListExpr _SYMB_1 { $$ = make_EApp($1, $3);  }
  | _STRING_ { $$ = make_EString($1);  }
  | _SYMB_0 Expr _SYMB_1 { $$ = $2;  }
;
Expr5 : _SYMB_9 Expr6 { $$ = make_Neg($2);  } 
  | _SYMB_10 Expr6 { $$ = make_Not($2);  }
  | Expr6 { $$ = $1;  }
;
Expr4 : Expr4 MulOp Expr5 { $$ = make_EMul($1, $2, $3);  } 
  | Expr5 { $$ = $1;  }
;
Expr3 : Expr3 AddOp Expr4 { $$ = make_EAdd($1, $2, $3);  } 
  | Expr4 { $$ = $1;  }
;
Expr2 : Expr2 RelOp Expr3 { $$ = make_ERel($1, $2, $3);  } 
  | Expr3 { $$ = $1;  }
;
Expr1 : Expr2 _SYMB_11 Expr1 { $$ = make_EAnd($1, $3);  } 
  | Expr2 { $$ = $1;  }
;
Expr : Expr1 _SYMB_12 Expr { $$ = make_EOr($1, $3);  } 
  | Expr1 { $$ = $1;  }
;
ListExpr : /* empty */ { $$ = 0;  } 
  | Expr { $$ = make_ListExpr($1, 0);  }
  | Expr _SYMB_2 ListExpr { $$ = make_ListExpr($1, $3);  }
;
AddOp : _SYMB_13 { $$ = make_Plus();  } 
  | _SYMB_9 { $$ = make_Minus();  }
;
MulOp : _SYMB_14 { $$ = make_Times();  } 
  | _SYMB_15 { $$ = make_Div();  }
  | _SYMB_16 { $$ = make_Mod();  }
;
RelOp : _SYMB_17 { $$ = make_LTH();  } 
  | _SYMB_18 { $$ = make_LE();  }
  | _SYMB_19 { $$ = make_GTH();  }
  | _SYMB_20 { $$ = make_GE();  }
  | _SYMB_21 { $$ = make_EQU();  }
  | _SYMB_22 { $$ = make_NE();  }
;

