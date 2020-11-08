/*** BNFC-Generated Pretty Printer and Abstract Syntax Viewer ***/

#include "printer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INDENT_WIDTH 2

int _n_;
char *buf_;
int cur_;
int buf_size;

/* You may wish to change the renderC functions */
void renderC(Char c)
{
  if (c == '{')
  {
     bufAppendC('\n');
     indent();
     bufAppendC(c);
     _n_ = _n_ + INDENT_WIDTH;
     bufAppendC('\n');
     indent();
  }
  else if (c == '(' || c == '[')
     bufAppendC(c);
  else if (c == ')' || c == ']')
  {
     backup();
     bufAppendC(c);
  }
  else if (c == '}')
  {
     int t;
     _n_ = _n_ - INDENT_WIDTH;
     for(t=0; t<INDENT_WIDTH; t++) {
       backup();
     }
     bufAppendC(c);
     bufAppendC('\n');
     indent();
  }
  else if (c == ',')
  {
     backup();
     bufAppendC(c);
     bufAppendC(' ');
  }
  else if (c == ';')
  {
     backup();
     bufAppendC(c);
     bufAppendC('\n');
     indent();
  }
  else if (c == 0) return;
  else
  {
     bufAppendC(' ');
     bufAppendC(c);
     bufAppendC(' ');
  }
}
void renderS(String s)
{
  if(strlen(s) > 0)
  {
    bufAppendS(s);
    bufAppendC(' ');
  }
}
void indent(void)
{
  int n = _n_;
  while (n > 0)
  {
    bufAppendC(' ');
    n--;
  }
}
void backup(void)
{
  if (buf_[cur_ - 1] == ' ')
  {
    buf_[cur_ - 1] = 0;
    cur_--;
  }
}
char *printProgram(Program p)
{
  _n_ = 0;
  bufReset();
  ppProgram(p, 0);
  return buf_;
}
char *showProgram(Program p)
{
  _n_ = 0;
  bufReset();
  shProgram(p);
  return buf_;
}
void ppProgram(Program _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Prog:
    if (_i_ > 0) renderC(_L_PAREN);
    ppListTopDef(_p_->u.prog_.listtopdef_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Program!\n");
    exit(1);
  }
}

void ppTopDef(TopDef _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_FnDef:
    if (_i_ > 0) renderC(_L_PAREN);
    ppType(_p_->u.fndef_.type_, 0);
    ppIdent(_p_->u.fndef_.ident_, 0);
    renderC('(');
    ppListArg(_p_->u.fndef_.listarg_, 0);
    renderC(')');
    ppBlock(_p_->u.fndef_.block_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing TopDef!\n");
    exit(1);
  }
}

void ppListTopDef(ListTopDef listtopdef, int i)
{
  while(listtopdef != 0)
  {
    if (listtopdef->listtopdef_ == 0)
    {
      ppTopDef(listtopdef->topdef_, i);

      listtopdef = 0;
    }
    else
    {
      ppTopDef(listtopdef->topdef_, i);
      renderS("");
      listtopdef = listtopdef->listtopdef_;
    }
  }
}

void ppArg(Arg _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Ar:
    if (_i_ > 0) renderC(_L_PAREN);
    ppType(_p_->u.ar_.type_, 0);
    ppIdent(_p_->u.ar_.ident_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Arg!\n");
    exit(1);
  }
}

void ppListArg(ListArg listarg, int i)
{
  while(listarg != 0)
  {
    if (listarg->listarg_ == 0)
    {
      ppArg(listarg->arg_, i);

      listarg = 0;
    }
    else
    {
      ppArg(listarg->arg_, i);
      renderC(',');
      listarg = listarg->listarg_;
    }
  }
}

void ppBlock(Block _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Blk:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('{');
    ppListStmt(_p_->u.blk_.liststmt_, 0);
    renderC('}');

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Block!\n");
    exit(1);
  }
}

void ppListStmt(ListStmt liststmt, int i)
{
  while(liststmt != 0)
  {
    if (liststmt->liststmt_ == 0)
    {
      ppStmt(liststmt->stmt_, i);
      renderS("");
      liststmt = 0;
    }
    else
    {
      ppStmt(liststmt->stmt_, i);
      renderS("");
      liststmt = liststmt->liststmt_;
    }
  }
}

void ppStmt(Stmt _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Empty:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_BStmt:
    if (_i_ > 0) renderC(_L_PAREN);
    ppBlock(_p_->u.bstmt_.block_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Decl:
    if (_i_ > 0) renderC(_L_PAREN);
    ppType(_p_->u.decl_.type_, 0);
    ppListItem(_p_->u.decl_.listitem_, 0);
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Ass:
    if (_i_ > 0) renderC(_L_PAREN);
    ppIdent(_p_->u.ass_.ident_, 0);
    renderC('=');
    ppExpr(_p_->u.ass_.expr_, 0);
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Incr:
    if (_i_ > 0) renderC(_L_PAREN);
    ppIdent(_p_->u.incr_.ident_, 0);
    renderS("++");
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Decr:
    if (_i_ > 0) renderC(_L_PAREN);
    ppIdent(_p_->u.decr_.ident_, 0);
    renderS("--");
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Ret:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("return");
    ppExpr(_p_->u.ret_.expr_, 0);
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_VRet:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("return");
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Cond:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("if");
    renderC('(');
    ppExpr(_p_->u.cond_.expr_, 0);
    renderC(')');
    ppStmt(_p_->u.cond_.stmt_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_CondElse:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("if");
    renderC('(');
    ppExpr(_p_->u.condelse_.expr_, 0);
    renderC(')');
    ppStmt(_p_->u.condelse_.stmt_1, 0);
    renderS("else");
    ppStmt(_p_->u.condelse_.stmt_2, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_While:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("while");
    renderC('(');
    ppExpr(_p_->u.while_.expr_, 0);
    renderC(')');
    ppStmt(_p_->u.while_.stmt_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_SExp:
    if (_i_ > 0) renderC(_L_PAREN);
    ppExpr(_p_->u.sexp_.expr_, 0);
    renderC(';');

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Stmt!\n");
    exit(1);
  }
}

void ppItem(Item _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_NoInit:
    if (_i_ > 0) renderC(_L_PAREN);
    ppIdent(_p_->u.noinit_.ident_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Init:
    if (_i_ > 0) renderC(_L_PAREN);
    ppIdent(_p_->u.init_.ident_, 0);
    renderC('=');
    ppExpr(_p_->u.init_.expr_, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Item!\n");
    exit(1);
  }
}

void ppListItem(ListItem listitem, int i)
{
  while(listitem != 0)
  {
    if (listitem->listitem_ == 0)
    {
      ppItem(listitem->item_, i);

      listitem = 0;
    }
    else
    {
      ppItem(listitem->item_, i);
      renderC(',');
      listitem = listitem->listitem_;
    }
  }
}

void ppType(Type _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Int:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("int");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Str:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("string");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Bool:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("boolean");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Void:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("void");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Fun:
    if (_i_ > 0) renderC(_L_PAREN);
    /* Internal Category */
    ppType(_p_->u.fun_.type_, 0);
    renderC('(');
    ppListType(_p_->u.fun_.listtype_, 0);
    renderC(')');

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Type!\n");
    exit(1);
  }
}

void ppListType(ListType listtype, int i)
{
  while(listtype != 0)
  {
    if (listtype->listtype_ == 0)
    {
      ppType(listtype->type_, i);

      listtype = 0;
    }
    else
    {
      ppType(listtype->type_, i);
      renderC(',');
      listtype = listtype->listtype_;
    }
  }
}

void ppExpr(Expr _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_EVar:
    if (_i_ > 6) renderC(_L_PAREN);
    ppIdent(_p_->u.evar_.ident_, 0);

    if (_i_ > 6) renderC(_R_PAREN);
    break;

  case is_ELitInt:
    if (_i_ > 6) renderC(_L_PAREN);
    ppInteger(_p_->u.elitint_.integer_, 0);

    if (_i_ > 6) renderC(_R_PAREN);
    break;

  case is_ELitTrue:
    if (_i_ > 6) renderC(_L_PAREN);
    renderS("true");

    if (_i_ > 6) renderC(_R_PAREN);
    break;

  case is_ELitFalse:
    if (_i_ > 6) renderC(_L_PAREN);
    renderS("false");

    if (_i_ > 6) renderC(_R_PAREN);
    break;

  case is_EApp:
    if (_i_ > 6) renderC(_L_PAREN);
    ppIdent(_p_->u.eapp_.ident_, 0);
    renderC('(');
    ppListExpr(_p_->u.eapp_.listexpr_, 0);
    renderC(')');

    if (_i_ > 6) renderC(_R_PAREN);
    break;

  case is_EString:
    if (_i_ > 6) renderC(_L_PAREN);
    ppString(_p_->u.estring_.string_, 0);

    if (_i_ > 6) renderC(_R_PAREN);
    break;

  case is_Neg:
    if (_i_ > 5) renderC(_L_PAREN);
    renderC('-');
    ppExpr(_p_->u.neg_.expr_, 6);

    if (_i_ > 5) renderC(_R_PAREN);
    break;

  case is_Not:
    if (_i_ > 5) renderC(_L_PAREN);
    renderC('!');
    ppExpr(_p_->u.not_.expr_, 6);

    if (_i_ > 5) renderC(_R_PAREN);
    break;

  case is_EMul:
    if (_i_ > 4) renderC(_L_PAREN);
    ppExpr(_p_->u.emul_.expr_1, 4);
    ppMulOp(_p_->u.emul_.mulop_, 0);
    ppExpr(_p_->u.emul_.expr_2, 5);

    if (_i_ > 4) renderC(_R_PAREN);
    break;

  case is_EAdd:
    if (_i_ > 3) renderC(_L_PAREN);
    ppExpr(_p_->u.eadd_.expr_1, 3);
    ppAddOp(_p_->u.eadd_.addop_, 0);
    ppExpr(_p_->u.eadd_.expr_2, 4);

    if (_i_ > 3) renderC(_R_PAREN);
    break;

  case is_ERel:
    if (_i_ > 2) renderC(_L_PAREN);
    ppExpr(_p_->u.erel_.expr_1, 2);
    ppRelOp(_p_->u.erel_.relop_, 0);
    ppExpr(_p_->u.erel_.expr_2, 3);

    if (_i_ > 2) renderC(_R_PAREN);
    break;

  case is_EAnd:
    if (_i_ > 1) renderC(_L_PAREN);
    ppExpr(_p_->u.eand_.expr_1, 2);
    renderS("&&");
    ppExpr(_p_->u.eand_.expr_2, 1);

    if (_i_ > 1) renderC(_R_PAREN);
    break;

  case is_EOr:
    if (_i_ > 0) renderC(_L_PAREN);
    ppExpr(_p_->u.eor_.expr_1, 1);
    renderS("||");
    ppExpr(_p_->u.eor_.expr_2, 0);

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing Expr!\n");
    exit(1);
  }
}

void ppListExpr(ListExpr listexpr, int i)
{
  while(listexpr != 0)
  {
    if (listexpr->listexpr_ == 0)
    {
      ppExpr(listexpr->expr_, i);

      listexpr = 0;
    }
    else
    {
      ppExpr(listexpr->expr_, i);
      renderC(',');
      listexpr = listexpr->listexpr_;
    }
  }
}

void ppAddOp(AddOp _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Plus:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('+');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Minus:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('-');

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing AddOp!\n");
    exit(1);
  }
}

void ppMulOp(MulOp _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_Times:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('*');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Div:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('/');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_Mod:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('%');

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing MulOp!\n");
    exit(1);
  }
}

void ppRelOp(RelOp _p_, int _i_)
{
  switch(_p_->kind)
  {
  case is_LTH:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('<');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_LE:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("<=");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_GTH:
    if (_i_ > 0) renderC(_L_PAREN);
    renderC('>');

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_GE:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS(">=");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_EQU:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("==");

    if (_i_ > 0) renderC(_R_PAREN);
    break;

  case is_NE:
    if (_i_ > 0) renderC(_L_PAREN);
    renderS("!=");

    if (_i_ > 0) renderC(_R_PAREN);
    break;


  default:
    fprintf(stderr, "Error: bad kind field when printing RelOp!\n");
    exit(1);
  }
}

void ppInteger(Integer n, int i)
{
  char tmp[16];
  sprintf(tmp, "%d", n);
  bufAppendS(tmp);
}
void ppDouble(Double d, int i)
{
  char tmp[16];
  sprintf(tmp, "%g", d);
  bufAppendS(tmp);
}
void ppChar(Char c, int i)
{
  bufAppendC('\'');
  bufAppendC(c);
  bufAppendC('\'');
}
void ppString(String s, int i)
{
  bufAppendC('\"');
  bufAppendS(s);
  bufAppendC('\"');
}
void ppIdent(String s, int i)
{
  renderS(s);
}

void shProgram(Program _p_)
{
  switch(_p_->kind)
  {
  case is_Prog:
    bufAppendC('(');

    bufAppendS("Prog");

    bufAppendC(' ');

    bufAppendC('[');
    shListTopDef(_p_->u.prog_.listtopdef_);
    bufAppendC(']');

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Program!\n");
    exit(1);
  }
}

void shTopDef(TopDef _p_)
{
  switch(_p_->kind)
  {
  case is_FnDef:
    bufAppendC('(');

    bufAppendS("FnDef");

    bufAppendC(' ');

    bufAppendC('[');
    shType(_p_->u.fndef_.type_);
    bufAppendC(']');
  bufAppendC(' ');
    shIdent(_p_->u.fndef_.ident_);
  bufAppendC(' ');
    bufAppendC('[');
    shListArg(_p_->u.fndef_.listarg_);
    bufAppendC(']');
  bufAppendC(' ');
    bufAppendC('[');
    shBlock(_p_->u.fndef_.block_);
    bufAppendC(']');

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing TopDef!\n");
    exit(1);
  }
}

void shListTopDef(ListTopDef listtopdef)
{
  while(listtopdef != 0)
  {
    if (listtopdef->listtopdef_)
    {
      shTopDef(listtopdef->topdef_);
      bufAppendS(", ");
      listtopdef = listtopdef->listtopdef_;
    }
    else
    {
      shTopDef(listtopdef->topdef_);
      listtopdef = 0;
    }
  }
}

void shArg(Arg _p_)
{
  switch(_p_->kind)
  {
  case is_Ar:
    bufAppendC('(');

    bufAppendS("Ar");

    bufAppendC(' ');

    bufAppendC('[');
    shType(_p_->u.ar_.type_);
    bufAppendC(']');
  bufAppendC(' ');
    shIdent(_p_->u.ar_.ident_);

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Arg!\n");
    exit(1);
  }
}

void shListArg(ListArg listarg)
{
  while(listarg != 0)
  {
    if (listarg->listarg_)
    {
      shArg(listarg->arg_);
      bufAppendS(", ");
      listarg = listarg->listarg_;
    }
    else
    {
      shArg(listarg->arg_);
      listarg = 0;
    }
  }
}

void shBlock(Block _p_)
{
  switch(_p_->kind)
  {
  case is_Blk:
    bufAppendC('(');

    bufAppendS("Blk");

    bufAppendC(' ');

    bufAppendC('[');
    shListStmt(_p_->u.blk_.liststmt_);
    bufAppendC(']');

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Block!\n");
    exit(1);
  }
}

void shListStmt(ListStmt liststmt)
{
  while(liststmt != 0)
  {
    if (liststmt->liststmt_)
    {
      shStmt(liststmt->stmt_);
      bufAppendS(", ");
      liststmt = liststmt->liststmt_;
    }
    else
    {
      shStmt(liststmt->stmt_);
      liststmt = 0;
    }
  }
}

void shStmt(Stmt _p_)
{
  switch(_p_->kind)
  {
  case is_Empty:
  
    bufAppendS("Empty");

  

  
    break;
  case is_BStmt:
    bufAppendC('(');

    bufAppendS("BStmt");

    bufAppendC(' ');

    bufAppendC('[');
    shBlock(_p_->u.bstmt_.block_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_Decl:
    bufAppendC('(');

    bufAppendS("Decl");

    bufAppendC(' ');

    bufAppendC('[');
    shType(_p_->u.decl_.type_);
    bufAppendC(']');
  bufAppendC(' ');
    bufAppendC('[');
    shListItem(_p_->u.decl_.listitem_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_Ass:
    bufAppendC('(');

    bufAppendS("Ass");

    bufAppendC(' ');

    shIdent(_p_->u.ass_.ident_);
  bufAppendC(' ');
    bufAppendC('[');
    shExpr(_p_->u.ass_.expr_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_Incr:
    bufAppendC('(');

    bufAppendS("Incr");

    bufAppendC(' ');

    shIdent(_p_->u.incr_.ident_);

    bufAppendC(')');

    break;
  case is_Decr:
    bufAppendC('(');

    bufAppendS("Decr");

    bufAppendC(' ');

    shIdent(_p_->u.decr_.ident_);

    bufAppendC(')');

    break;
  case is_Ret:
    bufAppendC('(');

    bufAppendS("Ret");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.ret_.expr_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_VRet:
  
    bufAppendS("VRet");

  

  
    break;
  case is_Cond:
    bufAppendC('(');

    bufAppendS("Cond");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.cond_.expr_);
    bufAppendC(']');
  bufAppendC(' ');
    bufAppendC('[');
    shStmt(_p_->u.cond_.stmt_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_CondElse:
    bufAppendC('(');

    bufAppendS("CondElse");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.condelse_.expr_);
    bufAppendC(']');
  bufAppendC(' ');
    shStmt(_p_->u.condelse_.stmt_1);
  bufAppendC(' ');
    shStmt(_p_->u.condelse_.stmt_2);

    bufAppendC(')');

    break;
  case is_While:
    bufAppendC('(');

    bufAppendS("While");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.while_.expr_);
    bufAppendC(']');
  bufAppendC(' ');
    bufAppendC('[');
    shStmt(_p_->u.while_.stmt_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_SExp:
    bufAppendC('(');

    bufAppendS("SExp");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.sexp_.expr_);
    bufAppendC(']');

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Stmt!\n");
    exit(1);
  }
}

void shItem(Item _p_)
{
  switch(_p_->kind)
  {
  case is_NoInit:
    bufAppendC('(');

    bufAppendS("NoInit");

    bufAppendC(' ');

    shIdent(_p_->u.noinit_.ident_);

    bufAppendC(')');

    break;
  case is_Init:
    bufAppendC('(');

    bufAppendS("Init");

    bufAppendC(' ');

    shIdent(_p_->u.init_.ident_);
  bufAppendC(' ');
    bufAppendC('[');
    shExpr(_p_->u.init_.expr_);
    bufAppendC(']');

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Item!\n");
    exit(1);
  }
}

void shListItem(ListItem listitem)
{
  while(listitem != 0)
  {
    if (listitem->listitem_)
    {
      shItem(listitem->item_);
      bufAppendS(", ");
      listitem = listitem->listitem_;
    }
    else
    {
      shItem(listitem->item_);
      listitem = 0;
    }
  }
}

void shType(Type _p_)
{
  switch(_p_->kind)
  {
  case is_Int:
  
    bufAppendS("Int");

  

  
    break;
  case is_Str:
  
    bufAppendS("Str");

  

  
    break;
  case is_Bool:
  
    bufAppendS("Bool");

  

  
    break;
  case is_Void:
  
    bufAppendS("Void");

  

  
    break;
  case is_Fun:
    bufAppendC('(');

    bufAppendS("Fun");

    bufAppendC(' ');

    /* Internal Category */
  bufAppendC(' ');
    bufAppendC('[');
    shType(_p_->u.fun_.type_);
    bufAppendC(']');
  bufAppendC(' ');
    bufAppendC('[');
    shListType(_p_->u.fun_.listtype_);
    bufAppendC(']');

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Type!\n");
    exit(1);
  }
}

void shListType(ListType listtype)
{
  while(listtype != 0)
  {
    if (listtype->listtype_)
    {
      shType(listtype->type_);
      bufAppendS(", ");
      listtype = listtype->listtype_;
    }
    else
    {
      shType(listtype->type_);
      listtype = 0;
    }
  }
}

void shExpr(Expr _p_)
{
  switch(_p_->kind)
  {
  case is_EVar:
    bufAppendC('(');

    bufAppendS("EVar");

    bufAppendC(' ');

    shIdent(_p_->u.evar_.ident_);

    bufAppendC(')');

    break;
  case is_ELitInt:
    bufAppendC('(');

    bufAppendS("ELitInt");

    bufAppendC(' ');

    shInteger(_p_->u.elitint_.integer_);

    bufAppendC(')');

    break;
  case is_ELitTrue:
  
    bufAppendS("ELitTrue");

  

  
    break;
  case is_ELitFalse:
  
    bufAppendS("ELitFalse");

  

  
    break;
  case is_EApp:
    bufAppendC('(');

    bufAppendS("EApp");

    bufAppendC(' ');

    shIdent(_p_->u.eapp_.ident_);
  bufAppendC(' ');
    bufAppendC('[');
    shListExpr(_p_->u.eapp_.listexpr_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_EString:
    bufAppendC('(');

    bufAppendS("EString");

    bufAppendC(' ');

    shString(_p_->u.estring_.string_);

    bufAppendC(')');

    break;
  case is_Neg:
    bufAppendC('(');

    bufAppendS("Neg");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.neg_.expr_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_Not:
    bufAppendC('(');

    bufAppendS("Not");

    bufAppendC(' ');

    bufAppendC('[');
    shExpr(_p_->u.not_.expr_);
    bufAppendC(']');

    bufAppendC(')');

    break;
  case is_EMul:
    bufAppendC('(');

    bufAppendS("EMul");

    bufAppendC(' ');

    shExpr(_p_->u.emul_.expr_1);
  bufAppendC(' ');
    bufAppendC('[');
    shMulOp(_p_->u.emul_.mulop_);
    bufAppendC(']');
  bufAppendC(' ');
    shExpr(_p_->u.emul_.expr_2);

    bufAppendC(')');

    break;
  case is_EAdd:
    bufAppendC('(');

    bufAppendS("EAdd");

    bufAppendC(' ');

    shExpr(_p_->u.eadd_.expr_1);
  bufAppendC(' ');
    bufAppendC('[');
    shAddOp(_p_->u.eadd_.addop_);
    bufAppendC(']');
  bufAppendC(' ');
    shExpr(_p_->u.eadd_.expr_2);

    bufAppendC(')');

    break;
  case is_ERel:
    bufAppendC('(');

    bufAppendS("ERel");

    bufAppendC(' ');

    shExpr(_p_->u.erel_.expr_1);
  bufAppendC(' ');
    bufAppendC('[');
    shRelOp(_p_->u.erel_.relop_);
    bufAppendC(']');
  bufAppendC(' ');
    shExpr(_p_->u.erel_.expr_2);

    bufAppendC(')');

    break;
  case is_EAnd:
    bufAppendC('(');

    bufAppendS("EAnd");

    bufAppendC(' ');

    shExpr(_p_->u.eand_.expr_1);
  bufAppendC(' ');
    shExpr(_p_->u.eand_.expr_2);

    bufAppendC(')');

    break;
  case is_EOr:
    bufAppendC('(');

    bufAppendS("EOr");

    bufAppendC(' ');

    shExpr(_p_->u.eor_.expr_1);
  bufAppendC(' ');
    shExpr(_p_->u.eor_.expr_2);

    bufAppendC(')');

    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing Expr!\n");
    exit(1);
  }
}

void shListExpr(ListExpr listexpr)
{
  while(listexpr != 0)
  {
    if (listexpr->listexpr_)
    {
      shExpr(listexpr->expr_);
      bufAppendS(", ");
      listexpr = listexpr->listexpr_;
    }
    else
    {
      shExpr(listexpr->expr_);
      listexpr = 0;
    }
  }
}

void shAddOp(AddOp _p_)
{
  switch(_p_->kind)
  {
  case is_Plus:
  
    bufAppendS("Plus");

  

  
    break;
  case is_Minus:
  
    bufAppendS("Minus");

  

  
    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing AddOp!\n");
    exit(1);
  }
}

void shMulOp(MulOp _p_)
{
  switch(_p_->kind)
  {
  case is_Times:
  
    bufAppendS("Times");

  

  
    break;
  case is_Div:
  
    bufAppendS("Div");

  

  
    break;
  case is_Mod:
  
    bufAppendS("Mod");

  

  
    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing MulOp!\n");
    exit(1);
  }
}

void shRelOp(RelOp _p_)
{
  switch(_p_->kind)
  {
  case is_LTH:
  
    bufAppendS("LTH");

  

  
    break;
  case is_LE:
  
    bufAppendS("LE");

  

  
    break;
  case is_GTH:
  
    bufAppendS("GTH");

  

  
    break;
  case is_GE:
  
    bufAppendS("GE");

  

  
    break;
  case is_EQU:
  
    bufAppendS("EQU");

  

  
    break;
  case is_NE:
  
    bufAppendS("NE");

  

  
    break;

  default:
    fprintf(stderr, "Error: bad kind field when showing RelOp!\n");
    exit(1);
  }
}

void shInteger(Integer i)
{
  char tmp[16];
  sprintf(tmp, "%d", i);
  bufAppendS(tmp);
}
void shDouble(Double d)
{
  char tmp[16];
  sprintf(tmp, "%g", d);
  bufAppendS(tmp);
}
void shChar(Char c)
{
  bufAppendC('\'');
  bufAppendC(c);
  bufAppendC('\'');
}
void shString(String s)
{
  bufAppendC('\"');
  bufAppendS(s);
  bufAppendC('\"');
}
void shIdent(String s)
{
  bufAppendC('\"');
  bufAppendS(s);
  bufAppendC('\"');
}

void bufAppendS(const char *s)
{
  int len = strlen(s);
  int n;
  while (cur_ + len > buf_size)
  {
    buf_size *= 2; /* Double the buffer size */
    resizeBuffer();
  }
  for(n = 0; n < len; n++)
  {
    buf_[cur_ + n] = s[n];
  }
  cur_ += len;
  buf_[cur_] = 0;
}
void bufAppendC(const char c)
{
  if (cur_ == buf_size)
  {
    buf_size *= 2; /* Double the buffer size */
    resizeBuffer();
  }
  buf_[cur_] = c;
  cur_++;
  buf_[cur_] = 0;
}
void bufReset(void)
{
  cur_ = 0;
  buf_size = BUFFER_INITIAL;
  resizeBuffer();
  memset(buf_, 0, buf_size);
}
void resizeBuffer(void)
{
  char *temp = (char *) malloc(buf_size);
  if (!temp)
  {
    fprintf(stderr, "Error: Out of memory while attempting to grow buffer!\n");
    exit(1);
  }
  if (buf_)
  {
    strncpy(temp, buf_, buf_size); /* peteg: strlcpy is safer, but not POSIX/ISO C. */
    free(buf_);
  }
  buf_ = temp;
}
char *buf_;
int cur_, buf_size;

