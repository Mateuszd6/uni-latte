/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.6.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the variable and function names.  */
#define yyparse latteparse
#define yylex lattelex
#define yyerror latteerror
#define yydebug lattedebug
#define yynerrs lattenerrs
#define yylval lattelval
#define yychar lattechar

/* First part of user prologue.  */
#line 2 "src/latte.y"

#include "absyn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define initialize_lexer latte_initialize_lexer
extern int
yyparse(void);
extern int
yylex(void);
int yy_mylinenumber;
extern void
error(long line, char* fmt, ...);
extern int
initialize_lexer(FILE* inp);
void
yyerror(const char* str)
{
    extern char* lattetext;
    error(yy_mylinenumber + 1, "%s at %s", str, lattetext);
}

Program YY_RESULT_Program_ = 0;
Program
pProgram(FILE* inp)
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
TopDef
pTopDef(FILE* inp)
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
ListTopDef
pListTopDef(FILE* inp)
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
Arg
pArg(FILE* inp)
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
ListArg
pListArg(FILE* inp)
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
Block
pBlock(FILE* inp)
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
ListStmt
pListStmt(FILE* inp)
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
Stmt
pStmt(FILE* inp)
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
Item
pItem(FILE* inp)
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
ListItem
pListItem(FILE* inp)
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
Type
pType(FILE* inp)
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

Expr YY_RESULT_Expr_ = 0;
Expr
pExpr(FILE* inp)
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
ListExpr
pListExpr(FILE* inp)
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

ClBody YY_RESULT_ClBody_ = 0;
ClBody
pClBody(FILE* inp)
{
    initialize_lexer(inp);
    if (yyparse())
    { /* Failure */
        return 0;
    }
    else
    { /* Success */
        return YY_RESULT_ClBody_;
    }
}

ListClBody YY_RESULT_ListClBody_ = 0;
ListClBody
pListClBody(FILE* inp)
{
    initialize_lexer(inp);
    if (yyparse())
    { /* Failure */
        return 0;
    }
    else
    { /* Success */
        return YY_RESULT_ListClBody_;
    }
}

ClProps YY_RESULT_ClProps_ = 0;
ClProps
pClProps(FILE* inp)
{
    initialize_lexer(inp);
    if (yyparse())
    { /* Failure */
        return 0;
    }
    else
    { /* Success */
        return YY_RESULT_ClProps_;
    }
}

AddOp YY_RESULT_AddOp_ = 0;
AddOp
pAddOp(FILE* inp)
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
MulOp
pMulOp(FILE* inp)
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
RelOp
pRelOp(FILE* inp)
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

EqOp YY_RESULT_EqOp_ = 0;
EqOp
pEqOp(FILE* inp)
{
    initialize_lexer(inp);
    if (yyparse())
    { /* Failure */
        return 0;
    }
    else
    { /* Success */
        return YY_RESULT_EqOp_;
    }
}

ListTopDef
reverseListTopDef(ListTopDef l)
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
ListArg
reverseListArg(ListArg l)
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
ListStmt
reverseListStmt(ListStmt l)
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
ListItem
reverseListItem(ListItem l)
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
ListExpr
reverseListExpr(ListExpr l)
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
ListClBody
reverseListClBody(ListClBody l)
{
    ListClBody prev = 0;
    ListClBody tmp = 0;
    while (l)
    {
        tmp = l->listclbody_;
        l->listclbody_ = prev;
        prev = l;
        l = tmp;
    }
    return prev;
}

#line 458 "src/parser.c"

#ifndef YY_CAST
#ifdef __cplusplus
#define YY_CAST(Type, Val) static_cast<Type>(Val)
#define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type>(Val)
#else
#define YY_CAST(Type, Val) ((Type)(Val))
#define YY_REINTERPRET_CAST(Type, Val) ((Type)(Val))
#endif
#endif
#ifndef YY_NULLPTR
#if defined __cplusplus
#if 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#else
#define YY_NULLPTR ((void*)0)
#endif
#endif

/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#if YYDEBUG
extern int lattedebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype
{
    YYEMPTY = -2,
    YYEOF = 0,       /* "end of file"  */
    YYerror = 256,   /* error  */
    YYUNDEF = 257,   /* "invalid token"  */
    _ERROR_ = 258,   /* _ERROR_  */
    _SYMB_0 = 259,   /* _SYMB_0  */
    _SYMB_1 = 260,   /* _SYMB_1  */
    _SYMB_2 = 261,   /* _SYMB_2  */
    _SYMB_3 = 262,   /* _SYMB_3  */
    _SYMB_4 = 263,   /* _SYMB_4  */
    _SYMB_5 = 264,   /* _SYMB_5  */
    _SYMB_6 = 265,   /* _SYMB_6  */
    _SYMB_7 = 266,   /* _SYMB_7  */
    _SYMB_8 = 267,   /* _SYMB_8  */
    _SYMB_9 = 268,   /* _SYMB_9  */
    _SYMB_10 = 269,  /* _SYMB_10  */
    _SYMB_11 = 270,  /* _SYMB_11  */
    _SYMB_12 = 271,  /* _SYMB_12  */
    _SYMB_13 = 272,  /* _SYMB_13  */
    _SYMB_14 = 273,  /* _SYMB_14  */
    _SYMB_15 = 274,  /* _SYMB_15  */
    _SYMB_16 = 275,  /* _SYMB_16  */
    _SYMB_17 = 276,  /* _SYMB_17  */
    _SYMB_18 = 277,  /* _SYMB_18  */
    _SYMB_19 = 278,  /* _SYMB_19  */
    _SYMB_20 = 279,  /* _SYMB_20  */
    _SYMB_21 = 280,  /* _SYMB_21  */
    _SYMB_22 = 281,  /* _SYMB_22  */
    _SYMB_23 = 282,  /* _SYMB_23  */
    _SYMB_24 = 283,  /* _SYMB_24  */
    _SYMB_25 = 284,  /* _SYMB_25  */
    _SYMB_26 = 285,  /* _SYMB_26  */
    _SYMB_27 = 286,  /* _SYMB_27  */
    _SYMB_28 = 287,  /* _SYMB_28  */
    _SYMB_29 = 288,  /* _SYMB_29  */
    _SYMB_30 = 289,  /* _SYMB_30  */
    _SYMB_31 = 290,  /* _SYMB_31  */
    _SYMB_32 = 291,  /* _SYMB_32  */
    _SYMB_33 = 292,  /* _SYMB_33  */
    _SYMB_34 = 293,  /* _SYMB_34  */
    _SYMB_35 = 294,  /* _SYMB_35  */
    _SYMB_36 = 295,  /* _SYMB_36  */
    _SYMB_37 = 296,  /* _SYMB_37  */
    _SYMB_38 = 297,  /* _SYMB_38  */
    _STRING_ = 298,  /* _STRING_  */
    _INTEGER_ = 299, /* _INTEGER_  */
    _IDENT_ = 300    /* _IDENT_  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 383 "src/latte.y"

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
    Expr expr_;
    ListExpr listexpr_;
    ClBody clbody_;
    ListClBody listclbody_;
    ClProps clprops_;
    AddOp addop_;
    MulOp mulop_;
    RelOp relop_;
    EqOp eqop_;

#line 578 "src/parser.c"
};
typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE lattelval;

int
latteparse(void);

/* Symbol kind.  */
enum yysymbol_kind_t
{
    YYSYMBOL_YYEMPTY = -2,
    YYSYMBOL_YYEOF = 0,       /* "end of file"  */
    YYSYMBOL_YYerror = 1,     /* error  */
    YYSYMBOL_YYUNDEF = 2,     /* "invalid token"  */
    YYSYMBOL__ERROR_ = 3,     /* _ERROR_  */
    YYSYMBOL__SYMB_0 = 4,     /* _SYMB_0  */
    YYSYMBOL__SYMB_1 = 5,     /* _SYMB_1  */
    YYSYMBOL__SYMB_2 = 6,     /* _SYMB_2  */
    YYSYMBOL__SYMB_3 = 7,     /* _SYMB_3  */
    YYSYMBOL__SYMB_4 = 8,     /* _SYMB_4  */
    YYSYMBOL__SYMB_5 = 9,     /* _SYMB_5  */
    YYSYMBOL__SYMB_6 = 10,    /* _SYMB_6  */
    YYSYMBOL__SYMB_7 = 11,    /* _SYMB_7  */
    YYSYMBOL__SYMB_8 = 12,    /* _SYMB_8  */
    YYSYMBOL__SYMB_9 = 13,    /* _SYMB_9  */
    YYSYMBOL__SYMB_10 = 14,   /* _SYMB_10  */
    YYSYMBOL__SYMB_11 = 15,   /* _SYMB_11  */
    YYSYMBOL__SYMB_12 = 16,   /* _SYMB_12  */
    YYSYMBOL__SYMB_13 = 17,   /* _SYMB_13  */
    YYSYMBOL__SYMB_14 = 18,   /* _SYMB_14  */
    YYSYMBOL__SYMB_15 = 19,   /* _SYMB_15  */
    YYSYMBOL__SYMB_16 = 20,   /* _SYMB_16  */
    YYSYMBOL__SYMB_17 = 21,   /* _SYMB_17  */
    YYSYMBOL__SYMB_18 = 22,   /* _SYMB_18  */
    YYSYMBOL__SYMB_19 = 23,   /* _SYMB_19  */
    YYSYMBOL__SYMB_20 = 24,   /* _SYMB_20  */
    YYSYMBOL__SYMB_21 = 25,   /* _SYMB_21  */
    YYSYMBOL__SYMB_22 = 26,   /* _SYMB_22  */
    YYSYMBOL__SYMB_23 = 27,   /* _SYMB_23  */
    YYSYMBOL__SYMB_24 = 28,   /* _SYMB_24  */
    YYSYMBOL__SYMB_25 = 29,   /* _SYMB_25  */
    YYSYMBOL__SYMB_26 = 30,   /* _SYMB_26  */
    YYSYMBOL__SYMB_27 = 31,   /* _SYMB_27  */
    YYSYMBOL__SYMB_28 = 32,   /* _SYMB_28  */
    YYSYMBOL__SYMB_29 = 33,   /* _SYMB_29  */
    YYSYMBOL__SYMB_30 = 34,   /* _SYMB_30  */
    YYSYMBOL__SYMB_31 = 35,   /* _SYMB_31  */
    YYSYMBOL__SYMB_32 = 36,   /* _SYMB_32  */
    YYSYMBOL__SYMB_33 = 37,   /* _SYMB_33  */
    YYSYMBOL__SYMB_34 = 38,   /* _SYMB_34  */
    YYSYMBOL__SYMB_35 = 39,   /* _SYMB_35  */
    YYSYMBOL__SYMB_36 = 40,   /* _SYMB_36  */
    YYSYMBOL__SYMB_37 = 41,   /* _SYMB_37  */
    YYSYMBOL__SYMB_38 = 42,   /* _SYMB_38  */
    YYSYMBOL__STRING_ = 43,   /* _STRING_  */
    YYSYMBOL__INTEGER_ = 44,  /* _INTEGER_  */
    YYSYMBOL__IDENT_ = 45,    /* _IDENT_  */
    YYSYMBOL_YYACCEPT = 46,   /* $accept  */
    YYSYMBOL_Program = 47,    /* Program  */
    YYSYMBOL_TopDef = 48,     /* TopDef  */
    YYSYMBOL_ListTopDef = 49, /* ListTopDef  */
    YYSYMBOL_Arg = 50,        /* Arg  */
    YYSYMBOL_ListArg = 51,    /* ListArg  */
    YYSYMBOL_Block = 52,      /* Block  */
    YYSYMBOL_ListStmt = 53,   /* ListStmt  */
    YYSYMBOL_Stmt = 54,       /* Stmt  */
    YYSYMBOL_Item = 55,       /* Item  */
    YYSYMBOL_ListItem = 56,   /* ListItem  */
    YYSYMBOL_Type = 57,       /* Type  */
    YYSYMBOL_Expr13 = 58,     /* Expr13  */
    YYSYMBOL_Expr12 = 59,     /* Expr12  */
    YYSYMBOL_Expr11 = 60,     /* Expr11  */
    YYSYMBOL_Expr9 = 61,      /* Expr9  */
    YYSYMBOL_Expr8 = 62,      /* Expr8  */
    YYSYMBOL_Expr7 = 63,      /* Expr7  */
    YYSYMBOL_Expr6 = 64,      /* Expr6  */
    YYSYMBOL_Expr5 = 65,      /* Expr5  */
    YYSYMBOL_Expr4 = 66,      /* Expr4  */
    YYSYMBOL_Expr3 = 67,      /* Expr3  */
    YYSYMBOL_Expr2 = 68,      /* Expr2  */
    YYSYMBOL_Expr1 = 69,      /* Expr1  */
    YYSYMBOL_Expr = 70,       /* Expr  */
    YYSYMBOL_Expr10 = 71,     /* Expr10  */
    YYSYMBOL_ListExpr = 72,   /* ListExpr  */
    YYSYMBOL_ClBody = 73,     /* ClBody  */
    YYSYMBOL_ListClBody = 74, /* ListClBody  */
    YYSYMBOL_ClProps = 75,    /* ClProps  */
    YYSYMBOL_AddOp = 76,      /* AddOp  */
    YYSYMBOL_MulOp = 77,      /* MulOp  */
    YYSYMBOL_RelOp = 78,      /* RelOp  */
    YYSYMBOL_EqOp = 79        /* EqOp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;

#ifdef short
#undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
#include <limits.h> /* INFRINGES ON USER NAME SPACE */
#if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#define YY_STDINT_H
#endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H &&                  \
       UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H &&                 \
       UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
#if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#define YYPTRDIFF_T __PTRDIFF_TYPE__
#define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
#elif defined PTRDIFF_MAX
#ifndef ptrdiff_t
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#endif
#define YYPTRDIFF_T ptrdiff_t
#define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
#else
#define YYPTRDIFF_T long
#define YYPTRDIFF_MAXIMUM LONG_MAX
#endif
#endif

#ifndef YYSIZE_T
#ifdef __SIZE_TYPE__
#define YYSIZE_T __SIZE_TYPE__
#elif defined size_t
#define YYSIZE_T size_t
#elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#define YYSIZE_T size_t
#else
#define YYSIZE_T unsigned
#endif
#endif

#define YYSIZE_MAXIMUM                                                         \
    YY_CAST(YYPTRDIFF_T,                                                       \
            (YYPTRDIFF_MAXIMUM < YY_CAST(YYSIZE_T, -1)                         \
               ? YYPTRDIFF_MAXIMUM                                             \
               : YY_CAST(YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST(YYPTRDIFF_T, sizeof(X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(Msgid) dgettext("bison-runtime", Msgid)
#endif
#endif
#ifndef YY_
#define YY_(Msgid) Msgid
#endif
#endif

#ifndef YY_ATTRIBUTE_PURE
#if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#define YY_ATTRIBUTE_PURE
#endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define YY_ATTRIBUTE_UNUSED
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YYUSE(E) ((void)(E))
#else
#define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && !defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                                    \
    _Pragma("GCC diagnostic push")                                             \
      _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")                    \
        _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
#define YY_IGNORE_MAYBE_UNINITIALIZED_END _Pragma("GCC diagnostic pop")
#else
#define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && !defined __ICC && 6 <= __GNUC__
#define YY_IGNORE_USELESS_CAST_BEGIN                                           \
    _Pragma("GCC diagnostic push")                                             \
      _Pragma("GCC diagnostic ignored \"-Wuseless-cast\"")
#define YY_IGNORE_USELESS_CAST_END _Pragma("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_END
#endif

#define YY_ASSERT(E) ((void)(0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

#ifdef YYSTACK_USE_ALLOCA
#if YYSTACK_USE_ALLOCA
#ifdef __GNUC__
#define YYSTACK_ALLOC __builtin_alloca
#elif defined __BUILTIN_VA_ARG_INCR
#include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#elif defined _AIX
#define YYSTACK_ALLOC __alloca
#elif defined _MSC_VER
#include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#define alloca _alloca
#else
#define YYSTACK_ALLOC alloca
#if !defined _ALLOCA_H && !defined EXIT_SUCCESS
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#endif
#endif
#endif

#ifdef YYSTACK_ALLOC
/* Pacify GCC's 'empty if-body' warning.  */
#define YYSTACK_FREE(Ptr)                                                      \
    do                                                                         \
    { /* empty */                                                              \
        ;                                                                      \
    } while (0)
#ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#endif
#else
#define YYSTACK_ALLOC YYMALLOC
#define YYSTACK_FREE YYFREE
#ifndef YYSTACK_ALLOC_MAXIMUM
#define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#endif
#if (defined __cplusplus && !defined EXIT_SUCCESS &&                           \
     !((defined YYMALLOC || defined malloc) &&                                 \
       (defined YYFREE || defined free)))
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#if !defined malloc && !defined EXIT_SUCCESS
void* malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#ifndef YYFREE
#define YYFREE free
#if !defined free && !defined EXIT_SUCCESS
void
free(void*); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#endif
#endif /* !defined yyoverflow */

#if (!defined yyoverflow &&                                                    \
     (!defined __cplusplus ||                                                  \
      (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
    yy_state_t yyss_alloc;
    YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#define YYSTACK_GAP_MAXIMUM (YYSIZEOF(union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#define YYSTACK_BYTES(N)                                                       \
    ((N) * (YYSIZEOF(yy_state_t) + YYSIZEOF(YYSTYPE)) + YYSTACK_GAP_MAXIMUM)

#define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#define YYSTACK_RELOCATE(Stack_alloc, Stack)                                   \
    do                                                                         \
    {                                                                          \
        YYPTRDIFF_T yynewbytes;                                                \
        YYCOPY(&yyptr->Stack_alloc, Stack, yysize);                            \
        Stack = &yyptr->Stack_alloc;                                           \
        yynewbytes = yystacksize * YYSIZEOF(*Stack) + YYSTACK_GAP_MAXIMUM;     \
        yyptr += yynewbytes / YYSIZEOF(*yyptr);                                \
    } while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(Dst, Src, Count)                                                \
    __builtin_memcpy(Dst, Src, YY_CAST(YYSIZE_T, (Count)) * sizeof(*(Src)))
#else
#define YYCOPY(Dst, Src, Count)                                                \
    do                                                                         \
    {                                                                          \
        YYPTRDIFF_T yyi;                                                       \
        for (yyi = 0; yyi < (Count); yyi++)                                    \
            (Dst)[yyi] = (Src)[yyi];                                           \
    } while (0)
#endif
#endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 193

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 34
/* YYNRULES -- Number of rules.  */
#define YYNRULES 87
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 156

#define YYMAXUTOK 300

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                       \
    (0 <= (YYX) && (YYX) <= YYMAXUTOK                                          \
       ? YY_CAST(yysymbol_kind_t, yytranslate[YYX])                            \
       : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] = {
    0,   492, 492, 494, 495, 497, 498, 500, 502, 503, 504, 506, 508, 509, 511,
    512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 525, 526, 528,
    529, 531, 532, 534, 535, 537, 538, 540, 541, 542, 544, 545, 547, 548, 549,
    551, 552, 553, 554, 555, 556, 558, 559, 560, 562, 563, 565, 566, 568, 569,
    571, 572, 573, 575, 576, 578, 579, 581, 583, 584, 585, 587, 588, 590, 591,
    593, 594, 596, 597, 599, 600, 601, 603, 604, 605, 606, 608, 609
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST(yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char*
yysymbol_name(yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char* const yytname[] = { "\"end of file\"",
                                       "error",
                                       "\"invalid token\"",
                                       "_ERROR_",
                                       "_SYMB_0",
                                       "_SYMB_1",
                                       "_SYMB_2",
                                       "_SYMB_3",
                                       "_SYMB_4",
                                       "_SYMB_5",
                                       "_SYMB_6",
                                       "_SYMB_7",
                                       "_SYMB_8",
                                       "_SYMB_9",
                                       "_SYMB_10",
                                       "_SYMB_11",
                                       "_SYMB_12",
                                       "_SYMB_13",
                                       "_SYMB_14",
                                       "_SYMB_15",
                                       "_SYMB_16",
                                       "_SYMB_17",
                                       "_SYMB_18",
                                       "_SYMB_19",
                                       "_SYMB_20",
                                       "_SYMB_21",
                                       "_SYMB_22",
                                       "_SYMB_23",
                                       "_SYMB_24",
                                       "_SYMB_25",
                                       "_SYMB_26",
                                       "_SYMB_27",
                                       "_SYMB_28",
                                       "_SYMB_29",
                                       "_SYMB_30",
                                       "_SYMB_31",
                                       "_SYMB_32",
                                       "_SYMB_33",
                                       "_SYMB_34",
                                       "_SYMB_35",
                                       "_SYMB_36",
                                       "_SYMB_37",
                                       "_SYMB_38",
                                       "_STRING_",
                                       "_INTEGER_",
                                       "_IDENT_",
                                       "$accept",
                                       "Program",
                                       "TopDef",
                                       "ListTopDef",
                                       "Arg",
                                       "ListArg",
                                       "Block",
                                       "ListStmt",
                                       "Stmt",
                                       "Item",
                                       "ListItem",
                                       "Type",
                                       "Expr13",
                                       "Expr12",
                                       "Expr11",
                                       "Expr9",
                                       "Expr8",
                                       "Expr7",
                                       "Expr6",
                                       "Expr5",
                                       "Expr4",
                                       "Expr3",
                                       "Expr2",
                                       "Expr1",
                                       "Expr",
                                       "Expr10",
                                       "ListExpr",
                                       "ClBody",
                                       "ListClBody",
                                       "ClProps",
                                       "AddOp",
                                       "MulOp",
                                       "RelOp",
                                       "EqOp",
                                       YY_NULLPTR };

static const char*
yysymbol_name(yysymbol_kind_t yysymbol)
{
    return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] = { 0,   256, 257, 258, 259, 260, 261, 262,
                                         263, 264, 265, 266, 267, 268, 269, 270,
                                         271, 272, 273, 274, 275, 276, 277, 278,
                                         279, 280, 281, 282, 283, 284, 285, 286,
                                         287, 288, 289, 290, 291, 292, 293, 294,
                                         295, 296, 297, 298, 299, 300 };
#endif

#define YYPACT_NINF (-129)

#define yypact_value_is_default(Yyn) ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-32)

#define yytable_value_is_error(Yyn) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    -27,  -19,  10,   30,   -27,  -129, -12,  4,    -129, -129, -129, 36,
    21,   62,   29,   -129, -129, 67,   71,   32,   -3,   29,   73,   -129,
    -129, 42,   -129, -129, -129, -129, 28,   16,   29,   -129, 119,  -129,
    -129, 130,  130,  -129, 77,   80,   43,   -129, 87,   -129, 85,   -129,
    -129, 2,    -129, -129, 45,   -129, -129, -129, -129, 81,   -129, -129,
    -129, 47,   -5,   113,  72,   53,   83,   94,   39,   95,   148,  88,
    -129, -129, 56,   148,  86,   -129, 98,   148,  148,  114,  96,   99,
    148,  -129, -129, -129, 148,  -129, -129, 148,  148,  -129, -129, -129,
    -129, -129, -129, 148,  148,  148,  -129, 148,  118,  120,  90,   73,
    130,  -129, 91,   140,  148,  -129, 141,  139,  145,  148,  45,   -129,
    134,  -129, 47,   -129, -5,   -5,   -129, 144,  -129, -129, 151,  -129,
    -129, 143,  76,   142,  76,   148,  -129, -129, -129, -129, -129, 148,
    148,  128,  -129, -129, -129, 165,  167,  76,   -129, 76,   -129, -129
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] = {
    0,  0,  31, 0,  5,  2,  0,  75, 32, 1,  6,  0,  0,  0,  8,  76, 73, 9,
    0,  0,  0,  8,  0,  7,  4,  0,  74, 10, 12, 3,  0,  0,  8,  71, 0,  11,
    14, 0,  0,  48, 0,  0,  0,  49, 0,  47, 0,  46, 45, 33, 15, 13, 0,  36,
    39, 67, 44, 50, 53, 55, 57, 59, 62, 64, 66, 0,  41, 0,  33, 0,  0,  33,
    51, 52, 0,  0,  42, 21, 0,  0,  68, 27, 29, 0,  0,  79, 80, 81, 0,  78,
    77, 0,  0,  82, 83, 84, 85, 86, 87, 0,  0,  0,  26, 0,  0,  0,  0,  0,
    0,  34, 0,  0,  0,  20, 0,  69, 0,  0,  0,  16, 0,  56, 58, 63, 60, 61,
    65, 0,  18, 19, 38, 72, 54, 0,  0,  0,  0,  68, 35, 28, 30, 40, 17, 68,
    0,  22, 43, 24, 70, 0,  0,  0,  37, 0,  23, 25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -129, -129, -129, 172,  -129, -13,  -21,  -129, -122, -129, 59,   7,
    -129, -129, -129, -129, -129, -129, -35,  92,   93,   -51,  -129, 89,
    -34,  -129, -128, -129, -129, -129, -129, -129, -129, -129
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] = { -1, 3,  4,  5,  17, 18, 50, 31, 51,
                                         82, 83, 52, 53, 54, 55, 56, 57, 58,
                                         59, 60, 61, 62, 63, 64, 65, 66, 116,
                                         26, 20, 13, 91, 88, 99, 100 };

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] = {
    69,  29,  72,  73,  24,  1,   80,  6,   27,  148, 78,  6,   145, 89,  147,
    149, 8,   90,  2,   67,  34,  19,  28,  35,  8,   36,  7,   25,  19,  154,
    9,   155, 32,  11,  37,  38,  69,  33,  12,  19,  14,  111, 2,   80,  108,
    114, 115, -31, 124, 125, 120, 39,  40,  41,  42,  43,  44,  45,  46,  47,
    48,  49,  102, 103, 104, 105, 15,  126, 16,  127, 85,  86,  87,  132, 2,
    21,  22,  23,  135, 28,  34,  74,  28,  139, 75,  36,  131, 30,  76,  79,
    81,  34,  80,  101, 37,  38,  77,  84,  106, 107, 109, 110, 112, 115, 118,
    37,  38,  113, 119, 115, 150, 39,  40,  41,  42,  43,  44,  45,  46,  47,
    48,  49,  39,  34,  117, 42,  43,  128, 45,  129, 47,  48,  71,  92,  70,
    130, 133, 37,  38,  93,  94,  95,  96,  97,  98,  134, 136, 137, 37,  38,
    138, 141, 34,  142, 39,  143, 144, 42,  43,  146, 45,  151, 47,  48,  68,
    39,  37,  38,  42,  43,  152, 45,  153, 47,  48,  71,  10,  140, 0,   0,
    121, 123, 0,   39,  122, 0,   42,  43,  0,   45,  0,   47,  48,  71
};

static const yytype_int16 yycheck[] = {
    34,  22,  37, 38,  7,   32,  4,  0,   21,  137, 44, 4,   134, 18,  136,
    143, 14,  22, 45,  32,  4,   14, 6,   7,   14,  9,  45,  20,  21,  151,
    0,   153, 4,  45,  18,  19,  70, 9,   34,  32,  4,  75,  45,  4,   5,
    79,  80,  45, 99,  100, 84,  35, 36,  37,  38,  39, 40,  41,  42,  43,
    44,  45,  9,  10,  11,  12,  45, 101, 6,   103, 23, 24,  25,  108, 45,
    8,   5,   45, 112, 6,   4,   4,  6,   117, 4,   9,  107, 45,  45,  4,
    45,  4,   4,  21,  18,  19,  9,  16,  15,  5,   5,  45,  16,  137, 8,
    18,  19,  9,  9,   143, 144, 35, 36,  37,  38,  39, 40,  41,  42,  43,
    44,  45,  35, 4,   10,  38,  39, 9,   41,  9,   43, 44,  45,  20,  4,
    45,  45,  18, 19,  26,  27,  28, 29,  30,  31,  5,  5,   8,   18,  19,
    5,   17,  4,  9,   35,  4,   13, 38,  39,  17,  41, 33,  43,  44,  45,
    35,  18,  19, 38,  39,  5,   41, 5,   43,  44,  45, 4,   118, -1,  -1,
    88,  92,  -1, 35,  91,  -1,  38, 39,  -1,  41,  -1, 43,  44,  45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] = {
    0,  32, 45, 47, 48, 49, 57, 45, 14, 0,  49, 45, 34, 75, 4,  45, 6,  50,
    51, 57, 74, 8,  5,  45, 7,  57, 73, 51, 6,  52, 45, 53, 4,  9,  4,  7,
    9,  18, 19, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 52, 54, 57, 58,
    59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 51, 45, 70, 4,  45,
    64, 64, 4,  4,  45, 9,  70, 4,  4,  45, 55, 56, 16, 23, 24, 25, 77, 18,
    22, 76, 20, 26, 27, 28, 29, 30, 31, 78, 79, 21, 9,  10, 11, 12, 15, 5,
    5,  5,  45, 70, 16, 9,  70, 70, 72, 10, 8,  9,  70, 65, 66, 69, 67, 67,
    70, 70, 9,  9,  45, 52, 64, 45, 5,  70, 5,  8,  5,  70, 56, 17, 9,  4,
    13, 54, 17, 54, 72, 72, 70, 33, 5,  5,  54, 54
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] = {
    0,  46, 47, 48, 48, 49, 49, 50, 51, 51, 51, 52, 53, 53, 54, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 54, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59,
    59, 60, 60, 60, 61, 61, 62, 62, 62, 63, 63, 63, 63, 63, 63, 64, 64, 64,
    65, 65, 66, 66, 67, 67, 68, 68, 68, 69, 69, 70, 70, 71, 72, 72, 72, 73,
    73, 74, 74, 75, 75, 76, 76, 77, 77, 77, 78, 78, 78, 78, 79, 79
};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] = { 0, 2, 1, 6, 6, 1, 2, 2, 0, 1, 3, 3, 0, 2, 1,
                                    1, 3, 4, 3, 3, 3, 2, 5, 7, 5, 8, 2, 1, 3, 1,
                                    3, 1, 2, 1, 3, 4, 1, 6, 3, 1, 4, 1, 2, 5, 1,
                                    1, 1, 1, 1, 1, 1, 2, 2, 1, 4, 1, 3, 1, 3, 1,
                                    3, 3, 1, 3, 1, 3, 1, 1, 0, 1, 3, 3, 6, 0, 2,
                                    0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

enum
{
    YYENOMEM = -2
};

#define yyerrok (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab

#define YYRECOVERING() (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                                 \
    do                                                                         \
        if (yychar == YYEMPTY)                                                 \
        {                                                                      \
            yychar = (Token);                                                  \
            yylval = (Value);                                                  \
            YYPOPSTACK(yylen);                                                 \
            yystate = *yyssp;                                                  \
            goto yybackup;                                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            yyerror(YY_("syntax error: cannot back up"));                      \
            YYERROR;                                                           \
        }                                                                      \
    while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* Enable debugging if requested.  */
#if YYDEBUG

#ifndef YYFPRINTF
#include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#define YYFPRINTF fprintf
#endif

#define YYDPRINTF(Args)                                                        \
    do                                                                         \
    {                                                                          \
        if (yydebug)                                                           \
            YYFPRINTF Args;                                                    \
    } while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
#define YY_LOCATION_PRINT(File, Loc) ((void)0)
#endif

#define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                          \
    do                                                                         \
    {                                                                          \
        if (yydebug)                                                           \
        {                                                                      \
            YYFPRINTF(stderr, "%s ", Title);                                   \
            yy_symbol_print(stderr, Kind, Value);                              \
            YYFPRINTF(stderr, "\n");                                           \
        }                                                                      \
    } while (0)

/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print(FILE* yyo,
                      yysymbol_kind_t yykind,
                      YYSTYPE const* const yyvaluep)
{
    FILE* yyoutput = yyo;
    YYUSE(yyoutput);
    if (!yyvaluep)
        return;
#ifdef YYPRINT
    if (yykind < YYNTOKENS)
        YYPRINT(yyo, yytoknum[yykind], *yyvaluep);
#endif
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YYUSE(yykind);
    YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print(FILE* yyo,
                yysymbol_kind_t yykind,
                YYSTYPE const* const yyvaluep)
{
    YYFPRINTF(yyo,
              "%s %s (",
              yykind < YYNTOKENS ? "token" : "nterm",
              yysymbol_name(yykind));

    yy_symbol_value_print(yyo, yykind, yyvaluep);
    YYFPRINTF(yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print(yy_state_t* yybottom, yy_state_t* yytop)
{
    YYFPRINTF(stderr, "Stack now");
    for (; yybottom <= yytop; yybottom++)
    {
        int yybot = *yybottom;
        YYFPRINTF(stderr, " %d", yybot);
    }
    YYFPRINTF(stderr, "\n");
}

#define YY_STACK_PRINT(Bottom, Top)                                            \
    do                                                                         \
    {                                                                          \
        if (yydebug)                                                           \
            yy_stack_print((Bottom), (Top));                                   \
    } while (0)

/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print(yy_state_t* yyssp, YYSTYPE* yyvsp, int yyrule)
{
    int yylno = yyrline[yyrule];
    int yynrhs = yyr2[yyrule];
    int yyi;
    YYFPRINTF(
      stderr, "Reducing stack by rule %d (line %d):\n", yyrule - 1, yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++)
    {
        YYFPRINTF(stderr, "   $%d = ", yyi + 1);
        yy_symbol_print(stderr,
                        YY_ACCESSING_SYMBOL(+yyssp[yyi + 1 - yynrhs]),
                        &yyvsp[(yyi + 1) - (yynrhs)]);
        YYFPRINTF(stderr, "\n");
    }
}

#define YY_REDUCE_PRINT(Rule)                                                  \
    do                                                                         \
    {                                                                          \
        if (yydebug)                                                           \
            yy_reduce_print(yyssp, yyvsp, Rule);                               \
    } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
#define YYDPRINTF(Args) ((void)0)
#define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
#define YY_STACK_PRINT(Bottom, Top)
#define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct(const char* yymsg, yysymbol_kind_t yykind, YYSTYPE* yyvaluep)
{
    YYUSE(yyvaluep);
    if (!yymsg)
        yymsg = "Deleting";
    YY_SYMBOL_PRINT(yymsg, yykind, yyvaluep, yylocationp);

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YYUSE(yykind);
    YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;

/*----------.
| yyparse.  |
`----------*/

int
yyparse(void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t* yyss;
    yy_state_t* yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE* yyvs;
    YYSTYPE* yyvsp;

    int yyn;
    /* The return value of yyparse.  */
    int yyresult;
    /* Lookahead token as an internal (translated) token number.  */
    yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
    /* The variables used to return semantic value and location from the
       action routines.  */
    YYSTYPE yyval;

#define YYPOPSTACK(N) (yyvsp -= (N), yyssp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
       Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    yynerrs = 0;
    yystate = 0;
    yyerrstatus = 0;

    yystacksize = YYINITDEPTH;
    yyssp = yyss = yyssa;
    yyvsp = yyvs = yyvsa;

    YYDPRINTF((stderr, "Starting parse\n"));

    yychar = YYEMPTY; /* Cause a token to be read.  */
    goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
    /* In all cases, when you get here, the value and location stacks
       have just been pushed.  So pushing a state here evens the stacks.  */
    yyssp++;

/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
    YYDPRINTF((stderr, "Entering state %d\n", yystate));
    YY_ASSERT(0 <= yystate && yystate < YYNSTATES);
    YY_IGNORE_USELESS_CAST_BEGIN
    *yyssp = YY_CAST(yy_state_t, yystate);
    YY_IGNORE_USELESS_CAST_END
    YY_STACK_PRINT(yyss, yyssp);

    if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
        goto yyexhaustedlab;
#else
    {
        /* Get the current used size of the three stacks, in elements.  */
        YYPTRDIFF_T yysize = yyssp - yyss + 1;

#if defined yyoverflow
        {
            /* Give user a chance to reallocate the stack.  Use copies of
               these so that the &'s don't force the real ones into
               memory.  */
            yy_state_t* yyss1 = yyss;
            YYSTYPE* yyvs1 = yyvs;

            /* Each stack pointer address is followed by the size of the
               data in use in that stack, in bytes.  This used to be a
               conditional around just the two extra args, but that might
               be undefined if yyoverflow is a macro.  */
            yyoverflow(YY_("memory exhausted"),
                       &yyss1,
                       yysize * YYSIZEOF(*yyssp),
                       &yyvs1,
                       yysize * YYSIZEOF(*yyvsp),
                       &yystacksize);
            yyss = yyss1;
            yyvs = yyvs1;
        }
#else /* defined YYSTACK_RELOCATE */
        /* Extend the stack our own way.  */
        if (YYMAXDEPTH <= yystacksize)
            goto yyexhaustedlab;
        yystacksize *= 2;
        if (YYMAXDEPTH < yystacksize)
            yystacksize = YYMAXDEPTH;

        {
            yy_state_t* yyss1 = yyss;
            union yyalloc* yyptr = YY_CAST(
              union yyalloc*,
              YYSTACK_ALLOC(YY_CAST(YYSIZE_T, YYSTACK_BYTES(yystacksize))));
            if (!yyptr)
                goto yyexhaustedlab;
            YYSTACK_RELOCATE(yyss_alloc, yyss);
            YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#undef YYSTACK_RELOCATE
            if (yyss1 != yyssa)
                YYSTACK_FREE(yyss1);
        }
#endif

        yyssp = yyss + yysize - 1;
        yyvsp = yyvs + yysize - 1;

        YY_IGNORE_USELESS_CAST_BEGIN
        YYDPRINTF((
          stderr, "Stack size increased to %ld\n", YY_CAST(long, yystacksize)));
        YY_IGNORE_USELESS_CAST_END

        if (yyss + yystacksize - 1 <= yyssp)
            YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

    if (yystate == YYFINAL)
        YYACCEPT;

    goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:
    /* Do appropriate processing given the current state.  Read a
       lookahead token if we need one and don't already have one.  */

    /* First try to decide what to do without reference to lookahead token.  */
    yyn = yypact[yystate];
    if (yypact_value_is_default(yyn))
        goto yydefault;

    /* Not known => get a lookahead token if don't already have one.  */

    /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
    if (yychar == YYEMPTY)
    {
        YYDPRINTF((stderr, "Reading a token\n"));
        yychar = yylex();
    }

    if (yychar <= YYEOF)
    {
        yychar = YYEOF;
        yytoken = YYSYMBOL_YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
    }
    else if (yychar == YYerror)
    {
        /* The scanner already issued an error message, process directly
           to error recovery.  But do not keep the error token as
           lookahead, it is too special and may lead us to an endless
           loop in error recovery. */
        yychar = YYUNDEF;
        yytoken = YYSYMBOL_YYerror;
        goto yyerrlab1;
    }
    else
    {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
    }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
        goto yydefault;
    yyn = yytable[yyn];
    if (yyn <= 0)
    {
        if (yytable_value_is_error(yyn))
            goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
    }

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus)
        yyerrstatus--;

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);
    yystate = yyn;
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    /* Discard the shifted token.  */
    yychar = YYEMPTY;
    goto yynewstate;

/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
    yyn = yydefact[yystate];
    if (yyn == 0)
        goto yyerrlab;
    goto yyreduce;

/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
    /* yyn is the number of a rule to reduce with.  */
    yylen = yyr2[yyn];

    /* If YYLEN is nonzero, implement the default value of the action:
       '$$ = $1'.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  Assigning to YYVAL
       unconditionally makes the parser a bit smaller, and it avoids a
       GCC warning that YYVAL may be used uninitialized.  */
    yyval = yyvsp[1 - yylen];

    YY_REDUCE_PRINT(yyn);
    switch (yyn)
    {
        case 2:
#line 492 "src/latte.y"
        {
            (yyval.program_) = make_Prog((yyvsp[0].listtopdef_));
            YY_RESULT_Program_ = (yyval.program_);
        }
#line 1760 "src/parser.c"
        break;

        case 3:
#line 494 "src/latte.y"
        {
            (yyval.topdef_) = make_FnDef((yyvsp[-5].type_),
                                         (yyvsp[-4].string_),
                                         (yyvsp[-2].listarg_),
                                         (yyvsp[0].block_));
        }
#line 1766 "src/parser.c"
        break;

        case 4:
#line 495 "src/latte.y"
        {
            (yyval.topdef_) =
              make_ClDef((yyvsp[-4].string_),
                         (yyvsp[-3].clprops_),
                         reverseListClBody((yyvsp[-1].listclbody_)));
        }
#line 1772 "src/parser.c"
        break;

        case 5:
#line 497 "src/latte.y"
        {
            (yyval.listtopdef_) = make_ListTopDef((yyvsp[0].topdef_), 0);
        }
#line 1778 "src/parser.c"
        break;

        case 6:
#line 498 "src/latte.y"
        {
            (yyval.listtopdef_) =
              make_ListTopDef((yyvsp[-1].topdef_), (yyvsp[0].listtopdef_));
        }
#line 1784 "src/parser.c"
        break;

        case 7:
#line 500 "src/latte.y"
        {
            (yyval.arg_) = make_Ar((yyvsp[-1].type_), (yyvsp[0].string_));
        }
#line 1790 "src/parser.c"
        break;

        case 8:
#line 502 "src/latte.y"
        {
            (yyval.listarg_) = 0;
        }
#line 1796 "src/parser.c"
        break;

        case 9:
#line 503 "src/latte.y"
        {
            (yyval.listarg_) = make_ListArg((yyvsp[0].arg_), 0);
        }
#line 1802 "src/parser.c"
        break;

        case 10:
#line 504 "src/latte.y"
        {
            (yyval.listarg_) =
              make_ListArg((yyvsp[-2].arg_), (yyvsp[0].listarg_));
        }
#line 1808 "src/parser.c"
        break;

        case 11:
#line 506 "src/latte.y"
        {
            (yyval.block_) = make_Blk(reverseListStmt((yyvsp[-1].liststmt_)));
        }
#line 1814 "src/parser.c"
        break;

        case 12:
#line 508 "src/latte.y"
        {
            (yyval.liststmt_) = 0;
        }
#line 1820 "src/parser.c"
        break;

        case 13:
#line 509 "src/latte.y"
        {
            (yyval.liststmt_) =
              make_ListStmt((yyvsp[0].stmt_), (yyvsp[-1].liststmt_));
        }
#line 1826 "src/parser.c"
        break;

        case 14:
#line 511 "src/latte.y"
        {
            (yyval.stmt_) = make_Empty();
        }
#line 1832 "src/parser.c"
        break;

        case 15:
#line 512 "src/latte.y"
        {
            (yyval.stmt_) = make_BStmt((yyvsp[0].block_));
        }
#line 1838 "src/parser.c"
        break;

        case 16:
#line 513 "src/latte.y"
        {
            (yyval.stmt_) = make_Decl((yyvsp[-2].type_), (yyvsp[-1].listitem_));
        }
#line 1844 "src/parser.c"
        break;

        case 17:
#line 514 "src/latte.y"
        {
            (yyval.stmt_) = make_Ass((yyvsp[-3].expr_), (yyvsp[-1].expr_));
        }
#line 1850 "src/parser.c"
        break;

        case 18:
#line 515 "src/latte.y"
        {
            (yyval.stmt_) = make_Incr((yyvsp[-2].expr_));
        }
#line 1856 "src/parser.c"
        break;

        case 19:
#line 516 "src/latte.y"
        {
            (yyval.stmt_) = make_Decr((yyvsp[-2].expr_));
        }
#line 1862 "src/parser.c"
        break;

        case 20:
#line 517 "src/latte.y"
        {
            (yyval.stmt_) = make_Ret((yyvsp[-1].expr_));
        }
#line 1868 "src/parser.c"
        break;

        case 21:
#line 518 "src/latte.y"
        {
            (yyval.stmt_) = make_VRet();
        }
#line 1874 "src/parser.c"
        break;

        case 22:
#line 519 "src/latte.y"
        {
            (yyval.stmt_) = make_Cond((yyvsp[-2].expr_), (yyvsp[0].stmt_));
        }
#line 1880 "src/parser.c"
        break;

        case 23:
#line 520 "src/latte.y"
        {
            (yyval.stmt_) = make_CondElse(
              (yyvsp[-4].expr_), (yyvsp[-2].stmt_), (yyvsp[0].stmt_));
        }
#line 1886 "src/parser.c"
        break;

        case 24:
#line 521 "src/latte.y"
        {
            (yyval.stmt_) = make_While((yyvsp[-2].expr_), (yyvsp[0].stmt_));
        }
#line 1892 "src/parser.c"
        break;

        case 25:
#line 522 "src/latte.y"
        {
            (yyval.stmt_) = make_For((yyvsp[-5].string_),
                                     (yyvsp[-4].string_),
                                     (yyvsp[-2].expr_),
                                     (yyvsp[0].stmt_));
        }
#line 1898 "src/parser.c"
        break;

        case 26:
#line 523 "src/latte.y"
        {
            (yyval.stmt_) = make_SExp((yyvsp[-1].expr_));
        }
#line 1904 "src/parser.c"
        break;

        case 27:
#line 525 "src/latte.y"
        {
            (yyval.item_) = make_NoInit((yyvsp[0].string_));
        }
#line 1910 "src/parser.c"
        break;

        case 28:
#line 526 "src/latte.y"
        {
            (yyval.item_) = make_Init((yyvsp[-2].string_), (yyvsp[0].expr_));
        }
#line 1916 "src/parser.c"
        break;

        case 29:
#line 528 "src/latte.y"
        {
            (yyval.listitem_) = make_ListItem((yyvsp[0].item_), 0);
        }
#line 1922 "src/parser.c"
        break;

        case 30:
#line 529 "src/latte.y"
        {
            (yyval.listitem_) =
              make_ListItem((yyvsp[-2].item_), (yyvsp[0].listitem_));
        }
#line 1928 "src/parser.c"
        break;

        case 31:
#line 531 "src/latte.y"
        {
            (yyval.type_) = make_TCls((yyvsp[0].string_));
        }
#line 1934 "src/parser.c"
        break;

        case 32:
#line 532 "src/latte.y"
        {
            (yyval.type_) = make_TArr((yyvsp[-1].string_));
        }
#line 1940 "src/parser.c"
        break;

        case 33:
#line 534 "src/latte.y"
        {
            (yyval.expr_) = make_EVar((yyvsp[0].string_));
        }
#line 1946 "src/parser.c"
        break;

        case 34:
#line 535 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[-1].expr_);
        }
#line 1952 "src/parser.c"
        break;

        case 35:
#line 537 "src/latte.y"
        {
            (yyval.expr_) =
              make_EApp((yyvsp[-3].string_), (yyvsp[-1].listexpr_));
        }
#line 1958 "src/parser.c"
        break;

        case 36:
#line 538 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 1964 "src/parser.c"
        break;

        case 37:
#line 540 "src/latte.y"
        {
            (yyval.expr_) = make_EClApp(
              (yyvsp[-5].expr_), (yyvsp[-3].string_), (yyvsp[-1].listexpr_));
        }
#line 1970 "src/parser.c"
        break;

        case 38:
#line 541 "src/latte.y"
        {
            (yyval.expr_) = make_EClMem((yyvsp[-2].expr_), (yyvsp[0].string_));
        }
#line 1976 "src/parser.c"
        break;

        case 39:
#line 542 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 1982 "src/parser.c"
        break;

        case 40:
#line 544 "src/latte.y"
        {
            (yyval.expr_) = make_EArrApp((yyvsp[-3].expr_), (yyvsp[-1].expr_));
        }
#line 1988 "src/parser.c"
        break;

        case 41:
#line 545 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 1994 "src/parser.c"
        break;

        case 42:
#line 547 "src/latte.y"
        {
            (yyval.expr_) = make_ENew((yyvsp[0].string_));
        }
#line 2000 "src/parser.c"
        break;

        case 43:
#line 548 "src/latte.y"
        {
            (yyval.expr_) =
              make_ENewArr((yyvsp[-3].string_), (yyvsp[-1].expr_));
        }
#line 2006 "src/parser.c"
        break;

        case 44:
#line 549 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2012 "src/parser.c"
        break;

        case 45:
#line 551 "src/latte.y"
        {
            (yyval.expr_) = make_ELitInt((yyvsp[0].int_));
        }
#line 2018 "src/parser.c"
        break;

        case 46:
#line 552 "src/latte.y"
        {
            (yyval.expr_) = make_ELitStr((yyvsp[0].string_));
        }
#line 2024 "src/parser.c"
        break;

        case 47:
#line 553 "src/latte.y"
        {
            (yyval.expr_) = make_ELitTrue();
        }
#line 2030 "src/parser.c"
        break;

        case 48:
#line 554 "src/latte.y"
        {
            (yyval.expr_) = make_ELitFalse();
        }
#line 2036 "src/parser.c"
        break;

        case 49:
#line 555 "src/latte.y"
        {
            (yyval.expr_) = make_ENull();
        }
#line 2042 "src/parser.c"
        break;

        case 50:
#line 556 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2048 "src/parser.c"
        break;

        case 51:
#line 558 "src/latte.y"
        {
            (yyval.expr_) = make_Neg((yyvsp[0].expr_));
        }
#line 2054 "src/parser.c"
        break;

        case 52:
#line 559 "src/latte.y"
        {
            (yyval.expr_) = make_Not((yyvsp[0].expr_));
        }
#line 2060 "src/parser.c"
        break;

        case 53:
#line 560 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2066 "src/parser.c"
        break;

        case 54:
#line 562 "src/latte.y"
        {
            (yyval.expr_) = make_ECast((yyvsp[-2].string_), (yyvsp[0].expr_));
        }
#line 2072 "src/parser.c"
        break;

        case 55:
#line 563 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2078 "src/parser.c"
        break;

        case 56:
#line 565 "src/latte.y"
        {
            (yyval.expr_) = make_EMul(
              (yyvsp[-2].expr_), (yyvsp[-1].mulop_), (yyvsp[0].expr_));
        }
#line 2084 "src/parser.c"
        break;

        case 57:
#line 566 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2090 "src/parser.c"
        break;

        case 58:
#line 568 "src/latte.y"
        {
            (yyval.expr_) = make_EAdd(
              (yyvsp[-2].expr_), (yyvsp[-1].addop_), (yyvsp[0].expr_));
        }
#line 2096 "src/parser.c"
        break;

        case 59:
#line 569 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2102 "src/parser.c"
        break;

        case 60:
#line 571 "src/latte.y"
        {
            (yyval.expr_) = make_ERel(
              (yyvsp[-2].expr_), (yyvsp[-1].relop_), (yyvsp[0].expr_));
        }
#line 2108 "src/parser.c"
        break;

        case 61:
#line 572 "src/latte.y"
        {
            (yyval.expr_) =
              make_EEq((yyvsp[-2].expr_), (yyvsp[-1].eqop_), (yyvsp[0].expr_));
        }
#line 2114 "src/parser.c"
        break;

        case 62:
#line 573 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2120 "src/parser.c"
        break;

        case 63:
#line 575 "src/latte.y"
        {
            (yyval.expr_) = make_EAnd((yyvsp[-2].expr_), (yyvsp[0].expr_));
        }
#line 2126 "src/parser.c"
        break;

        case 64:
#line 576 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2132 "src/parser.c"
        break;

        case 65:
#line 578 "src/latte.y"
        {
            (yyval.expr_) = make_EOr((yyvsp[-2].expr_), (yyvsp[0].expr_));
        }
#line 2138 "src/parser.c"
        break;

        case 66:
#line 579 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2144 "src/parser.c"
        break;

        case 67:
#line 581 "src/latte.y"
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2150 "src/parser.c"
        break;

        case 68:
#line 583 "src/latte.y"
        {
            (yyval.listexpr_) = 0;
        }
#line 2156 "src/parser.c"
        break;

        case 69:
#line 584 "src/latte.y"
        {
            (yyval.listexpr_) = make_ListExpr((yyvsp[0].expr_), 0);
        }
#line 2162 "src/parser.c"
        break;

        case 70:
#line 585 "src/latte.y"
        {
            (yyval.listexpr_) =
              make_ListExpr((yyvsp[-2].expr_), (yyvsp[0].listexpr_));
        }
#line 2168 "src/parser.c"
        break;

        case 71:
#line 587 "src/latte.y"
        {
            (yyval.clbody_) =
              make_CBVar((yyvsp[-2].type_), (yyvsp[-1].string_));
        }
#line 2174 "src/parser.c"
        break;

        case 72:
#line 588 "src/latte.y"
        {
            (yyval.clbody_) = make_CBFnDef((yyvsp[-5].type_),
                                           (yyvsp[-4].string_),
                                           (yyvsp[-2].listarg_),
                                           (yyvsp[0].block_));
        }
#line 2180 "src/parser.c"
        break;

        case 73:
#line 590 "src/latte.y"
        {
            (yyval.listclbody_) = 0;
        }
#line 2186 "src/parser.c"
        break;

        case 74:
#line 591 "src/latte.y"
        {
            (yyval.listclbody_) =
              make_ListClBody((yyvsp[0].clbody_), (yyvsp[-1].listclbody_));
        }
#line 2192 "src/parser.c"
        break;

        case 75:
#line 593 "src/latte.y"
        {
            (yyval.clprops_) = make_CNone();
        }
#line 2198 "src/parser.c"
        break;

        case 76:
#line 594 "src/latte.y"
        {
            (yyval.clprops_) = make_CExtends((yyvsp[0].string_));
        }
#line 2204 "src/parser.c"
        break;

        case 77:
#line 596 "src/latte.y"
        {
            (yyval.addop_) = make_Plus();
        }
#line 2210 "src/parser.c"
        break;

        case 78:
#line 597 "src/latte.y"
        {
            (yyval.addop_) = make_Minus();
        }
#line 2216 "src/parser.c"
        break;

        case 79:
#line 599 "src/latte.y"
        {
            (yyval.mulop_) = make_Times();
        }
#line 2222 "src/parser.c"
        break;

        case 80:
#line 600 "src/latte.y"
        {
            (yyval.mulop_) = make_Div();
        }
#line 2228 "src/parser.c"
        break;

        case 81:
#line 601 "src/latte.y"
        {
            (yyval.mulop_) = make_Mod();
        }
#line 2234 "src/parser.c"
        break;

        case 82:
#line 603 "src/latte.y"
        {
            (yyval.relop_) = make_LTH();
        }
#line 2240 "src/parser.c"
        break;

        case 83:
#line 604 "src/latte.y"
        {
            (yyval.relop_) = make_LE();
        }
#line 2246 "src/parser.c"
        break;

        case 84:
#line 605 "src/latte.y"
        {
            (yyval.relop_) = make_GTH();
        }
#line 2252 "src/parser.c"
        break;

        case 85:
#line 606 "src/latte.y"
        {
            (yyval.relop_) = make_GE();
        }
#line 2258 "src/parser.c"
        break;

        case 86:
#line 608 "src/latte.y"
        {
            (yyval.eqop_) = make_NE();
        }
#line 2264 "src/parser.c"
        break;

        case 87:
#line 609 "src/latte.y"
        {
            (yyval.eqop_) = make_EQU();
        }
#line 2270 "src/parser.c"
        break;

#line 2274 "src/parser.c"

        default:
            break;
    }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action invokes
       YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
       if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
       incorrect destructor might then be invoked immediately.  In the
       case of YYERROR or YYBACKUP, subsequent parser actions might lead
       to an incorrect destructor call or verbose syntax error message
       before the lookahead is translated.  */
    YY_SYMBOL_PRINT(
      "-> $$ =", YY_CAST(yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

    YYPOPSTACK(yylen);
    yylen = 0;

    *++yyvsp = yyval;

    /* Now 'shift' the result of the reduction.  Determine what state
       that goes to, based on the state we popped back to and the rule
       number reduced by.  */
    {
        const int yylhs = yyr1[yyn] - YYNTOKENS;
        const int yyi = yypgoto[yylhs] + *yyssp;
        yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
    }

    goto yynewstate;

/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE(yychar);
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus)
    {
        ++yynerrs;
        yyerror(YY_("syntax error"));
    }

    if (yyerrstatus == 3)
    {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        if (yychar <= YYEOF)
        {
            /* Return failure if at end of input.  */
            if (yychar == YYEOF)
                YYABORT;
        }
        else
        {
            yydestruct("Error: discarding", yytoken, &yylval);
            yychar = YYEMPTY;
        }
    }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;

/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and the
       label yyerrorlab therefore never appears in user code.  */
    if (0)
        YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);
    yystate = *yyssp;
    goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
    yyerrstatus = 3; /* Each real token shifted decrements this.  */

    /* Pop stack until we find a state that shifts the error token.  */
    for (;;)
    {
        yyn = yypact[yystate];
        if (!yypact_value_is_default(yyn))
        {
            yyn += YYSYMBOL_YYerror;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
                yyn = yytable[yyn];
                if (0 < yyn)
                    break;
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
            YYABORT;

        yydestruct("Error: popping", YY_ACCESSING_SYMBOL(yystate), yyvsp);
        YYPOPSTACK(1);
        yystate = *yyssp;
        YY_STACK_PRINT(yyss, yyssp);
    }

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    /* Shift the error token.  */
    YY_SYMBOL_PRINT("Shifting", YY_ACCESSING_SYMBOL(yyn), yyvsp, yylsp);

    yystate = yyn;
    goto yynewstate;

/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
    yyresult = 0;
    goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
    yyresult = 1;
    goto yyreturn;

#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
    yyerror(YY_("memory exhausted"));
    yyresult = 2;
    /* Fall through.  */
#endif

/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
    if (yychar != YYEMPTY)
    {
        /* Make sure we have latest lookahead translation.  See comments at
           user semantic actions for why this is necessary.  */
        yytoken = YYTRANSLATE(yychar);
        yydestruct("Cleanup: discarding lookahead", yytoken, &yylval);
    }
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    YYPOPSTACK(yylen);
    YY_STACK_PRINT(yyss, yyssp);
    while (yyssp != yyss)
    {
        yydestruct("Cleanup: popping", YY_ACCESSING_SYMBOL(+*yyssp), yyvsp);
        YYPOPSTACK(1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
        YYSTACK_FREE(yyss);
#endif

    return yyresult;
}
