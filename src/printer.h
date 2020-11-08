#ifndef PRINTER_HEADER
#define PRINTER_HEADER

#include "absyn.h"

/* Certain applications may improve performance by changing the buffer size */
#define BUFFER_INITIAL 2000
/* You may wish to change _L_PAREN or _R_PAREN */
#define _L_PAREN '('
#define _R_PAREN ')'

/* The following are simple heuristics for rendering terminals */
/* You may wish to change them */
void
renderCC(Char c);
void
renderCS(String s);
void
indent(void);
void
backup(void);

char*
printProgram(Program p);

void
ppProgram(Program p, int i);
void
ppTopDef(TopDef p, int i);
void
ppListTopDef(ListTopDef p, int i);
void
ppArg(Arg p, int i);
void
ppListArg(ListArg p, int i);
void
ppBlock(Block p, int i);
void
ppListStmt(ListStmt p, int i);
void
ppStmt(Stmt p, int i);
void
ppItem(Item p, int i);
void
ppListItem(ListItem p, int i);
void
ppType(Type p, int i);
void
ppListType(ListType p, int i);
void
ppExpr(Expr p, int i);
void
ppListExpr(ListExpr p, int i);
void
ppAddOp(AddOp p, int i);
void
ppMulOp(MulOp p, int i);
void
ppRelOp(RelOp p, int i);

char*
showProgram(Program p);

void
shProgram(Program p);
void
shTopDef(TopDef p);
void
shListTopDef(ListTopDef p);
void
shArg(Arg p);
void
shListArg(ListArg p);
void
shBlock(Block p);
void
shListStmt(ListStmt p);
void
shStmt(Stmt p);
void
shItem(Item p);
void
shListItem(ListItem p);
void
shType(Type p);
void
shListType(ListType p);
void
shExpr(Expr p);
void
shListExpr(ListExpr p);
void
shAddOp(AddOp p);
void
shMulOp(MulOp p);
void
shRelOp(RelOp p);

void
ppInteger(Integer n, int i);
void
ppDouble(Double d, int i);
void
ppChar(Char c, int i);
void
ppString(String s, int i);
void
ppIdent(String s, int i);
void
shInteger(Integer n);
void
shDouble(Double d);
void
shChar(Char c);
void
shString(String s);
void
shIdent(String s);
void
bufAppendS(const char* s);
void
bufAppendC(const char c);
void
bufReset(void);
void
resizeBuffer(void);

#endif
