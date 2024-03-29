/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

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
#line 2 "src/latte.y" /* yacc.c:337  */

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

#line 458 "src/parser.c" /* yacc.c:337  */
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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
#undef YYERROR_VERBOSE
#define YYERROR_VERBOSE 1
#else
#define YYERROR_VERBOSE 0
#endif

/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#if YYDEBUG
extern int lattedebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype
{
    _ERROR_ = 258,
    _SYMB_0 = 259,
    _SYMB_1 = 260,
    _SYMB_2 = 261,
    _SYMB_3 = 262,
    _SYMB_4 = 263,
    _SYMB_5 = 264,
    _SYMB_6 = 265,
    _SYMB_7 = 266,
    _SYMB_8 = 267,
    _SYMB_9 = 268,
    _SYMB_10 = 269,
    _SYMB_11 = 270,
    _SYMB_12 = 271,
    _SYMB_13 = 272,
    _SYMB_14 = 273,
    _SYMB_15 = 274,
    _SYMB_16 = 275,
    _SYMB_17 = 276,
    _SYMB_18 = 277,
    _SYMB_19 = 278,
    _SYMB_20 = 279,
    _SYMB_21 = 280,
    _SYMB_22 = 281,
    _SYMB_23 = 282,
    _SYMB_24 = 283,
    _SYMB_25 = 284,
    _SYMB_26 = 285,
    _SYMB_27 = 286,
    _SYMB_28 = 287,
    _SYMB_29 = 288,
    _SYMB_30 = 289,
    _SYMB_31 = 290,
    _SYMB_32 = 291,
    _SYMB_33 = 292,
    _SYMB_34 = 293,
    _SYMB_35 = 294,
    _SYMB_36 = 295,
    _SYMB_37 = 296,
    _SYMB_38 = 297,
    _STRING_ = 298,
    _INTEGER_ = 299,
    _IDENT_ = 300
};
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED

union YYSTYPE {
#line 383 "src/latte.y" /* yacc.c:352  */

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

#line 572 "src/parser.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE lattelval;

int
latteparse(void);

#ifdef short
#undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
#ifdef __SIZE_TYPE__
#define YYSIZE_T __SIZE_TYPE__
#elif defined size_t
#define YYSIZE_T size_t
#elif !defined YYSIZE_T
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#define YYSIZE_T size_t
#else
#define YYSIZE_T unsigned
#endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T)-1)

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

#ifndef YY_ATTRIBUTE
#if (defined __GNUC__ &&                                                       \
     (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__))) ||             \
  defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#define YY_ATTRIBUTE(Spec) __attribute__(Spec)
#else
#define YY_ATTRIBUTE(Spec) /* empty */
#endif
#endif

#ifndef YY_ATTRIBUTE_PURE
#define YY_ATTRIBUTE_PURE YY_ATTRIBUTE((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE((__unused__))
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

#if !defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */

#if (!defined yyoverflow &&                                                    \
     (!defined __cplusplus ||                                                  \
      (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
    yytype_int16 yyss_alloc;
    YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#define YYSTACK_GAP_MAXIMUM (sizeof(union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#define YYSTACK_BYTES(N)                                                       \
    ((N) * (sizeof(yytype_int16) + sizeof(YYSTYPE)) + YYSTACK_GAP_MAXIMUM)

#define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#define YYSTACK_RELOCATE(Stack_alloc, Stack)                                   \
    do                                                                         \
    {                                                                          \
        YYSIZE_T yynewbytes;                                                   \
        YYCOPY(&yyptr->Stack_alloc, Stack, yysize);                            \
        Stack = &yyptr->Stack_alloc;                                           \
        yynewbytes = yystacksize * sizeof(*Stack) + YYSTACK_GAP_MAXIMUM;       \
        yyptr += yynewbytes / sizeof(*yyptr);                                  \
    } while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(Dst, Src, Count)                                                \
    __builtin_memcpy(Dst, Src, (Count) * sizeof(*(Src)))
#else
#define YYCOPY(Dst, Src, Count)                                                \
    do                                                                         \
    {                                                                          \
        YYSIZE_T yyi;                                                          \
        for (yyi = 0; yyi < (Count); yyi++)                                    \
            (Dst)[yyi] = (Src)[yyi];                                           \
    } while (0)
#endif
#endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 188

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 32
/* YYNRULES -- Number of rules.  */
#define YYNRULES 85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 154

#define YYUNDEFTOK 2
#define YYMAXUTOK 300

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                       \
    ((unsigned)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] = {
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
static const yytype_uint16 yyrline[] = {
    0,   490, 490, 492, 493, 495, 496, 498, 500, 501, 502, 504, 506, 507, 509,
    510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 523, 524, 526,
    527, 529, 530, 532, 533, 535, 536, 538, 539, 540, 541, 543, 544, 545, 547,
    548, 549, 550, 551, 552, 554, 555, 556, 558, 559, 561, 562, 564, 565, 567,
    568, 569, 571, 572, 574, 575, 577, 578, 579, 581, 582, 584, 585, 587, 588,
    590, 591, 593, 594, 595, 597, 598, 599, 600, 602, 603
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char* const yytname[] = {
    "$end",     "error",    "$undefined", "_ERROR_",  "_SYMB_0",
    "_SYMB_1",  "_SYMB_2",  "_SYMB_3",    "_SYMB_4",  "_SYMB_5",
    "_SYMB_6",  "_SYMB_7",  "_SYMB_8",    "_SYMB_9",  "_SYMB_10",
    "_SYMB_11", "_SYMB_12", "_SYMB_13",   "_SYMB_14", "_SYMB_15",
    "_SYMB_16", "_SYMB_17", "_SYMB_18",   "_SYMB_19", "_SYMB_20",
    "_SYMB_21", "_SYMB_22", "_SYMB_23",   "_SYMB_24", "_SYMB_25",
    "_SYMB_26", "_SYMB_27", "_SYMB_28",   "_SYMB_29", "_SYMB_30",
    "_SYMB_31", "_SYMB_32", "_SYMB_33",   "_SYMB_34", "_SYMB_35",
    "_SYMB_36", "_SYMB_37", "_SYMB_38",   "_STRING_", "_INTEGER_",
    "_IDENT_",  "$accept",  "Program",    "TopDef",   "ListTopDef",
    "Arg",      "ListArg",  "Block",      "ListStmt", "Stmt",
    "Item",     "ListItem", "Type",       "Expr11",   "Expr10",
    "Expr9",    "Expr8",    "Expr7",      "Expr6",    "Expr5",
    "Expr4",    "Expr3",    "Expr2",      "Expr1",    "Expr",
    "ListExpr", "ClBody",   "ListClBody", "ClProps",  "AddOp",
    "MulOp",    "RelOp",    "EqOp",       YY_NULLPTR
};
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] = {
    0,   256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266,
    267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278,
    279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290,
    291, 292, 293, 294, 295, 296, 297, 298, 299, 300
};
#endif

#define YYPACT_NINF -122

#define yypact_value_is_default(Yystate) (!!((Yystate) == (-122)))

#define YYTABLE_NINF -32

#define yytable_value_is_error(Yytable_value) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    -27,  -36,  11,   27,   -27,  -122, -23,  -5,   -122, -122, -122, 38,
    5,    45,   18,   -122, -122, 56,   62,   25,   1,    18,   71,   -122,
    -122, 37,   -122, -122, -122, -122, 3,    17,   18,   -122, 103,  -122,
    -122, 121,  121,  -122, 83,   84,   46,   -122, 85,   -122, 86,   -122,
    -122, 0,    -122, -122, 50,   -122, -122, -122, 22,   -122, -122, -122,
    8,    -3,   152,  75,   63,   93,   36,   94,   132,  96,   -122, -122,
    57,   132,  90,   -122, 118,  132,  132,  122,  123,  124,  89,   132,
    -122, -122, -122, 132,  -122, -122, 132,  132,  -122, -122, -122, -122,
    -122, -122, 132,  132,  132,  -122, 132,  126,  128,  71,   121,  -122,
    98,   140,  132,  -122, 144,  145,  147,  132,  50,   -122, 150,  138,
    -122, 8,    -122, -3,   -3,   -122, 148,  -122, -122, -122, -122, 155,
    74,   141,  74,   132,  -122, -122, -122, 132,  -122, -122, 132,  130,
    -122, -122, -122, 156,  164,  74,   -122, 74,   -122, -122
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    0,  0,  31, 0,  5,  2,  0,  73, 32, 1,  6,  0,  0,  0,  8,  74, 71, 9,
    0,  0,  0,  8,  0,  7,  4,  0,  72, 10, 12, 3,  0,  0,  8,  69, 0,  11,
    14, 0,  0,  47, 0,  0,  0,  48, 0,  46, 0,  45, 44, 33, 15, 13, 0,  36,
    40, 43, 49, 52, 54, 56, 58, 61, 63, 65, 0,  0,  33, 0,  0,  33, 50, 51,
    0,  0,  41, 21, 0,  0,  66, 27, 29, 0,  0,  0,  77, 78, 79, 0,  76, 75,
    0,  0,  80, 81, 82, 83, 84, 85, 0,  0,  0,  26, 0,  0,  0,  0,  0,  34,
    0,  0,  0,  20, 0,  67, 0,  0,  0,  16, 38, 0,  55, 57, 62, 59, 60, 64,
    0,  18, 19, 70, 53, 0,  0,  0,  0,  66, 35, 28, 30, 66, 39, 17, 0,  22,
    42, 24, 68, 0,  0,  0,  37, 0,  23, 25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -122, -122, -122, 170,  -122, -15,  -21,  -122, -121, -122, 68,
    65,   -122, -122, -122, -122, -122, -35,  99,   95,   -51,  -122,
    97,   -34,  -119, -122, -122, -122, -122, -122, -122, -122
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] = { -1,  3,  4,  5,  17, 18, 50, 31,
                                         51,  80, 81, 52, 53, 54, 55, 56,
                                         57,  58, 59, 60, 61, 62, 63, 64,
                                         114, 26, 20, 13, 90, 87, 98, 99 };

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] = {
    67,  29,  70,  71,  78,  1,   27,  32,  24,  7,   76,  143, 33,  145, 8,
    88,  146, 65,  2,   89,  147, 34,  11,  28,  35,  8,   36,  9,   152, 12,
    153, 84,  85,  86,  67,  37,  38,  82,  83,  109, 78,  106, 14,  112, 113,
    -31, 2,   123, 124, 119, 15,  16,  39,  40,  41,  42,  43,  44,  45,  46,
    47,  48,  49,  2,   21,  6,   125, 22,  126, 6,   23,  130, 101, 102, 103,
    104, 133, 28,  34,  19,  28,  137, 30,  36,  129, 25,  19,  72,  73,  34,
    77,  74,  37,  38,  75,  79,  100, 19,  105, 107, 78,  113, 108, 37,  38,
    113, 110, 34,  148, 39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
    39,  37,  38,  42,  43,  68,  45,  111, 47,  48,  69,  116, 115, 117, 118,
    127, 34,  128, 39,  37,  38,  42,  43,  131, 45,  132, 47,  48,  66,  134,
    37,  38,  136, 135, 139, 140, 39,  141, 144, 42,  43,  150, 45,  149, 47,
    48,  69,  39,  142, 151, 42,  43,  91,  45,  10,  47,  48,  69,  92,  93,
    94,  95,  96,  97,  138, 121, 120, 0,   122
};

static const yytype_int16 yycheck[] = {
    34,  22,  37,  38, 4,   32,  21, 4,   7,   45,  44, 132, 9,   134, 14,  18,
    135, 32,  45,  22, 139, 4,   45, 6,   7,   14,  9,  0,   149, 34,  151, 23,
    24,  25,  68,  18, 19,  15,  16, 73,  4,   5,   4,  77,  78,  45,  45,  98,
    99,  83,  45,  6,  35,  36,  37, 38,  39,  40,  41, 42,  43,  44,  45,  45,
    8,   0,   100, 5,  102, 4,   45, 106, 9,   10,  11, 12,  110, 6,   4,   14,
    6,   115, 45,  9,  105, 20,  21, 4,   4,   4,   4,  45,  18,  19,  9,   45,
    21,  32,  5,   5,  4,   135, 45, 18,  19,  139, 16, 4,   142, 35,  36,  37,
    38,  39,  40,  41, 42,  43,  44, 45,  35,  18,  19, 38,  39,  4,   41,  9,
    43,  44,  45,  8,  10,  9,   45, 9,   4,   9,   35, 18,  19,  38,  39,  45,
    41,  5,   43,  44, 45,  5,   18, 19,  5,   8,   4,  17,  35,  9,   17,  38,
    39,  5,   41,  33, 43,  44,  45, 35,  13,  5,   38, 39,  20,  41,  4,   43,
    44,  45,  26,  27, 28,  29,  30, 31,  116, 90,  87, -1,  91
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  32, 45, 47, 48, 49, 57, 45, 14, 0,  49, 45, 34, 73, 4,  45, 6,  50,
    51, 57, 72, 8,  5,  45, 7,  57, 71, 51, 6,  52, 45, 53, 4,  9,  4,  7,
    9,  18, 19, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 52, 54, 57, 58,
    59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 51, 45, 69, 4,  45, 63, 63,
    4,  4,  45, 9,  69, 4,  4,  45, 55, 56, 15, 16, 23, 24, 25, 75, 18, 22,
    74, 20, 26, 27, 28, 29, 30, 31, 76, 77, 21, 9,  10, 11, 12, 5,  5,  5,
    45, 69, 16, 9,  69, 69, 70, 10, 8,  9,  45, 69, 64, 65, 68, 66, 66, 69,
    69, 9,  9,  52, 63, 45, 5,  69, 5,  8,  5,  69, 56, 4,  17, 9,  13, 54,
    17, 54, 70, 70, 69, 33, 5,  5,  54, 54
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  46, 47, 48, 48, 49, 49, 50, 51, 51, 51, 52, 53, 53, 54, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 54, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59,
    59, 60, 60, 60, 60, 61, 61, 61, 62, 62, 62, 62, 62, 62, 63, 63, 63, 64,
    64, 65, 65, 66, 66, 67, 67, 67, 68, 68, 69, 69, 70, 70, 70, 71, 71, 72,
    72, 73, 73, 74, 74, 75, 75, 75, 76, 76, 76, 76, 77, 77
};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0, 2, 1, 6, 6, 1, 2, 2, 0, 1, 3, 3, 0, 2, 1, 1, 3, 4, 3, 3, 3, 2,
    5, 7, 5, 8, 2, 1, 3, 1, 3, 1, 2, 1, 3, 4, 1, 6, 3, 4, 1, 2, 5, 1,
    1, 1, 1, 1, 1, 1, 2, 2, 1, 4, 1, 3, 1, 3, 1, 3, 3, 1, 3, 1, 3, 1,
    0, 1, 3, 3, 6, 0, 2, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

#define yyerrok (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)
#define YYEMPTY (-2)
#define YYEOF 0

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

/* Error token number */
#define YYTERROR 1
#define YYERRCODE 256

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

#define YY_SYMBOL_PRINT(Title, Type, Value, Location)                          \
    do                                                                         \
    {                                                                          \
        if (yydebug)                                                           \
        {                                                                      \
            YYFPRINTF(stderr, "%s ", Title);                                   \
            yy_symbol_print(stderr, Type, Value);                              \
            YYFPRINTF(stderr, "\n");                                           \
        }                                                                      \
    } while (0)

/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print(FILE* yyo, int yytype, YYSTYPE const* const yyvaluep)
{
    FILE* yyoutput = yyo;
    YYUSE(yyoutput);
    if (!yyvaluep)
        return;
#ifdef YYPRINT
    if (yytype < YYNTOKENS)
        YYPRINT(yyo, yytoknum[yytype], *yyvaluep);
#endif
    YYUSE(yytype);
}

/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print(FILE* yyo, int yytype, YYSTYPE const* const yyvaluep)
{
    YYFPRINTF(
      yyo, "%s %s (", yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

    yy_symbol_value_print(yyo, yytype, yyvaluep);
    YYFPRINTF(yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print(yytype_int16* yybottom, yytype_int16* yytop)
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
yy_reduce_print(yytype_int16* yyssp, YYSTYPE* yyvsp, int yyrule)
{
    unsigned long yylno = yyrline[yyrule];
    int yynrhs = yyr2[yyrule];
    int yyi;
    YYFPRINTF(
      stderr, "Reducing stack by rule %d (line %lu):\n", yyrule - 1, yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++)
    {
        YYFPRINTF(stderr, "   $%d = ", yyi + 1);
        yy_symbol_print(stderr,
                        yystos[yyssp[yyi + 1 - yynrhs]],
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
#define YYDPRINTF(Args)
#define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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

#if YYERROR_VERBOSE

#ifndef yystrlen
#if defined __GLIBC__ && defined _STRING_H
#define yystrlen strlen
#else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen(const char* yystr)
{
    YYSIZE_T yylen;
    for (yylen = 0; yystr[yylen]; yylen++)
        continue;
    return yylen;
}
#endif
#endif

#ifndef yystpcpy
#if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#define yystpcpy stpcpy
#else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char*
yystpcpy(char* yydest, const char* yysrc)
{
    char* yyd = yydest;
    const char* yys = yysrc;

    while ((*yyd++ = *yys++) != '\0')
        continue;

    return yyd - 1;
}
#endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr(char* yyres, const char* yystr)
{
    if (*yystr == '"')
    {
        YYSIZE_T yyn = 0;
        char const* yyp = yystr;

        for (;;)
            switch (*++yyp)
            {
                case '\'':
                case ',':
                    goto do_not_strip_quotes;

                case '\\':
                    if (*++yyp != '\\')
                        goto do_not_strip_quotes;
                    else
                        goto append;

                append:
                default:
                    if (yyres)
                        yyres[yyn] = *yyp;
                    yyn++;
                    break;

                case '"':
                    if (yyres)
                        yyres[yyn] = '\0';
                    return yyn;
            }
    do_not_strip_quotes:;
    }

    if (!yyres)
        return yystrlen(yystr);

    return (YYSIZE_T)(yystpcpy(yyres, yystr) - yyres);
}
#endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error(YYSIZE_T* yymsg_alloc,
               char** yymsg,
               yytype_int16* yyssp,
               int yytoken)
{
    YYSIZE_T yysize0 = yytnamerr(YY_NULLPTR, yytname[yytoken]);
    YYSIZE_T yysize = yysize0;
    enum
    {
        YYERROR_VERBOSE_ARGS_MAXIMUM = 5
    };
    /* Internationalized format string. */
    const char* yyformat = YY_NULLPTR;
    /* Arguments of yyformat. */
    char const* yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
    /* Number of reported tokens (one for the "unexpected", one per
       "expected"). */
    int yycount = 0;

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yychar) is if
         this state is a consistent state with a default action.  Thus,
         detecting the absence of a lookahead is sufficient to determine
         that there is no unexpected or expected token to report.  In that
         case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is a
         consistent state with a default action.  There might have been a
         previous inconsistent state, consistent state with a non-default
         action, or user semantic action that manipulated yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (yytoken != YYEMPTY)
    {
        int yyn = yypact[*yyssp];
        yyarg[yycount++] = yytname[yytoken];
        if (!yypact_value_is_default(yyn))
        {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = YYLAST - yyn + 1;
            int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
            int yyx;

            for (yyx = yyxbegin; yyx < yyxend; ++yyx)
                if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR &&
                    !yytable_value_is_error(yytable[yyx + yyn]))
                {
                    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                        yycount = 1;
                        yysize = yysize0;
                        break;
                    }
                    yyarg[yycount++] = yytname[yyx];
                    {
                        YYSIZE_T yysize1 =
                          yysize + yytnamerr(YY_NULLPTR, yytname[yyx]);
                        if (yysize <= yysize1 &&
                            yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                            yysize = yysize1;
                        else
                            return 2;
                    }
                }
        }
    }

    switch (yycount)
    {
#define YYCASE_(N, S)                                                          \
    case N:                                                                    \
        yyformat = S;                                                          \
        break
        default: /* Avoid compiler warnings. */
            YYCASE_(0, YY_("syntax error"));
            YYCASE_(1, YY_("syntax error, unexpected %s"));
            YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
            YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
            YYCASE_(
              4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
            YYCASE_(
              5,
              YY_(
                "syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

    {
        YYSIZE_T yysize1 = yysize + yystrlen(yyformat);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
            yysize = yysize1;
        else
            return 2;
    }

    if (*yymsg_alloc < yysize)
    {
        *yymsg_alloc = 2 * yysize;
        if (!(yysize <= *yymsg_alloc && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
            *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
        return 1;
    }

    /* Avoid sprintf, as that infringes on the user's name space.
       Don't have undefined behavior even if the translation
       produced a string with the wrong number of "%s"s.  */
    {
        char* yyp = *yymsg;
        int yyi = 0;
        while ((*yyp = *yyformat) != '\0')
            if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
            {
                yyp += yytnamerr(yyp, yyarg[yyi++]);
                yyformat += 2;
            }
            else
            {
                yyp++;
                yyformat++;
            }
    }
    return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct(const char* yymsg, int yytype, YYSTYPE* yyvaluep)
{
    YYUSE(yyvaluep);
    if (!yymsg)
        yymsg = "Deleting";
    YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YYUSE(yytype);
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16* yyss;
    yytype_int16* yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE* yyvs;
    YYSTYPE* yyvsp;

    YYSIZE_T yystacksize;

    int yyn;
    int yyresult;
    /* Lookahead token as an internal (translated) token number.  */
    int yytoken = 0;
    /* The variables used to return semantic value and location from the
       action routines.  */
    YYSTYPE yyval;

#if YYERROR_VERBOSE
    /* Buffer for error messages, and its allocated size.  */
    char yymsgbuf[128];
    char* yymsg = yymsgbuf;
    YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N) (yyvsp -= (N), yyssp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
       Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    yyssp = yyss = yyssa;
    yyvsp = yyvs = yyvsa;
    yystacksize = YYINITDEPTH;

    YYDPRINTF((stderr, "Starting parse\n"));

    yystate = 0;
    yyerrstatus = 0;
    yynerrs = 0;
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
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
    *yyssp = (yytype_int16)yystate;

    if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
        goto yyexhaustedlab;
#else
    {
        /* Get the current used size of the three stacks, in elements.  */
        YYSIZE_T yysize = (YYSIZE_T)(yyssp - yyss + 1);

#if defined yyoverflow
        {
            /* Give user a chance to reallocate the stack.  Use copies of
               these so that the &'s don't force the real ones into
               memory.  */
            YYSTYPE* yyvs1 = yyvs;
            yytype_int16* yyss1 = yyss;

            /* Each stack pointer address is followed by the size of the
               data in use in that stack, in bytes.  This used to be a
               conditional around just the two extra args, but that might
               be undefined if yyoverflow is a macro.  */
            yyoverflow(YY_("memory exhausted"),
                       &yyss1,
                       yysize * sizeof(*yyssp),
                       &yyvs1,
                       yysize * sizeof(*yyvsp),
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
            yytype_int16* yyss1 = yyss;
            union yyalloc* yyptr =
              (union yyalloc*)YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
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

        YYDPRINTF((
          stderr, "Stack size increased to %lu\n", (unsigned long)yystacksize));

        if (yyss + yystacksize - 1 <= yyssp)
            YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

    YYDPRINTF((stderr, "Entering state %d\n", yystate));

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

    /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
    if (yychar == YYEMPTY)
    {
        YYDPRINTF((stderr, "Reading a token: "));
        yychar = yylex();
    }

    if (yychar <= YYEOF)
    {
        yychar = yytoken = YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
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

    /* Discard the shifted token.  */
    yychar = YYEMPTY;

    yystate = yyn;
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

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
#line 490 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.program_) = make_Prog((yyvsp[0].listtopdef_));
            YY_RESULT_Program_ = (yyval.program_);
        }
#line 1783 "src/parser.c" /* yacc.c:1652  */
        break;

        case 3:
#line 492 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.topdef_) = make_FnDef((yyvsp[-5].type_),
                                         (yyvsp[-4].string_),
                                         (yyvsp[-2].listarg_),
                                         (yyvsp[0].block_));
        }
#line 1789 "src/parser.c" /* yacc.c:1652  */
        break;

        case 4:
#line 493 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.topdef_) =
              make_ClDef((yyvsp[-4].string_),
                         (yyvsp[-3].clprops_),
                         reverseListClBody((yyvsp[-1].listclbody_)));
        }
#line 1795 "src/parser.c" /* yacc.c:1652  */
        break;

        case 5:
#line 495 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listtopdef_) = make_ListTopDef((yyvsp[0].topdef_), 0);
        }
#line 1801 "src/parser.c" /* yacc.c:1652  */
        break;

        case 6:
#line 496 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listtopdef_) =
              make_ListTopDef((yyvsp[-1].topdef_), (yyvsp[0].listtopdef_));
        }
#line 1807 "src/parser.c" /* yacc.c:1652  */
        break;

        case 7:
#line 498 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.arg_) = make_Ar((yyvsp[-1].type_), (yyvsp[0].string_));
        }
#line 1813 "src/parser.c" /* yacc.c:1652  */
        break;

        case 8:
#line 500 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listarg_) = 0;
        }
#line 1819 "src/parser.c" /* yacc.c:1652  */
        break;

        case 9:
#line 501 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listarg_) = make_ListArg((yyvsp[0].arg_), 0);
        }
#line 1825 "src/parser.c" /* yacc.c:1652  */
        break;

        case 10:
#line 502 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listarg_) =
              make_ListArg((yyvsp[-2].arg_), (yyvsp[0].listarg_));
        }
#line 1831 "src/parser.c" /* yacc.c:1652  */
        break;

        case 11:
#line 504 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.block_) = make_Blk(reverseListStmt((yyvsp[-1].liststmt_)));
        }
#line 1837 "src/parser.c" /* yacc.c:1652  */
        break;

        case 12:
#line 506 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.liststmt_) = 0;
        }
#line 1843 "src/parser.c" /* yacc.c:1652  */
        break;

        case 13:
#line 507 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.liststmt_) =
              make_ListStmt((yyvsp[0].stmt_), (yyvsp[-1].liststmt_));
        }
#line 1849 "src/parser.c" /* yacc.c:1652  */
        break;

        case 14:
#line 509 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Empty();
        }
#line 1855 "src/parser.c" /* yacc.c:1652  */
        break;

        case 15:
#line 510 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_BStmt((yyvsp[0].block_));
        }
#line 1861 "src/parser.c" /* yacc.c:1652  */
        break;

        case 16:
#line 511 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Decl((yyvsp[-2].type_), (yyvsp[-1].listitem_));
        }
#line 1867 "src/parser.c" /* yacc.c:1652  */
        break;

        case 17:
#line 512 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Ass((yyvsp[-3].expr_), (yyvsp[-1].expr_));
        }
#line 1873 "src/parser.c" /* yacc.c:1652  */
        break;

        case 18:
#line 513 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Incr((yyvsp[-2].expr_));
        }
#line 1879 "src/parser.c" /* yacc.c:1652  */
        break;

        case 19:
#line 514 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Decr((yyvsp[-2].expr_));
        }
#line 1885 "src/parser.c" /* yacc.c:1652  */
        break;

        case 20:
#line 515 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Ret((yyvsp[-1].expr_));
        }
#line 1891 "src/parser.c" /* yacc.c:1652  */
        break;

        case 21:
#line 516 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_VRet();
        }
#line 1897 "src/parser.c" /* yacc.c:1652  */
        break;

        case 22:
#line 517 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Cond((yyvsp[-2].expr_), (yyvsp[0].stmt_));
        }
#line 1903 "src/parser.c" /* yacc.c:1652  */
        break;

        case 23:
#line 518 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_CondElse(
              (yyvsp[-4].expr_), (yyvsp[-2].stmt_), (yyvsp[0].stmt_));
        }
#line 1909 "src/parser.c" /* yacc.c:1652  */
        break;

        case 24:
#line 519 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_While((yyvsp[-2].expr_), (yyvsp[0].stmt_));
        }
#line 1915 "src/parser.c" /* yacc.c:1652  */
        break;

        case 25:
#line 520 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_For((yyvsp[-5].string_),
                                     (yyvsp[-4].string_),
                                     (yyvsp[-2].expr_),
                                     (yyvsp[0].stmt_));
        }
#line 1921 "src/parser.c" /* yacc.c:1652  */
        break;

        case 26:
#line 521 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_SExp((yyvsp[-1].expr_));
        }
#line 1927 "src/parser.c" /* yacc.c:1652  */
        break;

        case 27:
#line 523 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.item_) = make_NoInit((yyvsp[0].string_));
        }
#line 1933 "src/parser.c" /* yacc.c:1652  */
        break;

        case 28:
#line 524 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.item_) = make_Init((yyvsp[-2].string_), (yyvsp[0].expr_));
        }
#line 1939 "src/parser.c" /* yacc.c:1652  */
        break;

        case 29:
#line 526 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listitem_) = make_ListItem((yyvsp[0].item_), 0);
        }
#line 1945 "src/parser.c" /* yacc.c:1652  */
        break;

        case 30:
#line 527 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listitem_) =
              make_ListItem((yyvsp[-2].item_), (yyvsp[0].listitem_));
        }
#line 1951 "src/parser.c" /* yacc.c:1652  */
        break;

        case 31:
#line 529 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.type_) = make_TCls((yyvsp[0].string_));
        }
#line 1957 "src/parser.c" /* yacc.c:1652  */
        break;

        case 32:
#line 530 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.type_) = make_TArr((yyvsp[-1].string_));
        }
#line 1963 "src/parser.c" /* yacc.c:1652  */
        break;

        case 33:
#line 532 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EVar((yyvsp[0].string_));
        }
#line 1969 "src/parser.c" /* yacc.c:1652  */
        break;

        case 34:
#line 533 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[-1].expr_);
        }
#line 1975 "src/parser.c" /* yacc.c:1652  */
        break;

        case 35:
#line 535 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) =
              make_EApp((yyvsp[-3].string_), (yyvsp[-1].listexpr_));
        }
#line 1981 "src/parser.c" /* yacc.c:1652  */
        break;

        case 36:
#line 536 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 1987 "src/parser.c" /* yacc.c:1652  */
        break;

        case 37:
#line 538 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EClApp(
              (yyvsp[-5].expr_), (yyvsp[-3].string_), (yyvsp[-1].listexpr_));
        }
#line 1993 "src/parser.c" /* yacc.c:1652  */
        break;

        case 38:
#line 539 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EClMem((yyvsp[-2].expr_), (yyvsp[0].string_));
        }
#line 1999 "src/parser.c" /* yacc.c:1652  */
        break;

        case 39:
#line 540 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EArrApp((yyvsp[-3].expr_), (yyvsp[-1].expr_));
        }
#line 2005 "src/parser.c" /* yacc.c:1652  */
        break;

        case 40:
#line 541 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2011 "src/parser.c" /* yacc.c:1652  */
        break;

        case 41:
#line 543 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ENew((yyvsp[0].string_));
        }
#line 2017 "src/parser.c" /* yacc.c:1652  */
        break;

        case 42:
#line 544 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) =
              make_ENewArr((yyvsp[-3].string_), (yyvsp[-1].expr_));
        }
#line 2023 "src/parser.c" /* yacc.c:1652  */
        break;

        case 43:
#line 545 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2029 "src/parser.c" /* yacc.c:1652  */
        break;

        case 44:
#line 547 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitInt((yyvsp[0].int_));
        }
#line 2035 "src/parser.c" /* yacc.c:1652  */
        break;

        case 45:
#line 548 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitStr((yyvsp[0].string_));
        }
#line 2041 "src/parser.c" /* yacc.c:1652  */
        break;

        case 46:
#line 549 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitTrue();
        }
#line 2047 "src/parser.c" /* yacc.c:1652  */
        break;

        case 47:
#line 550 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitFalse();
        }
#line 2053 "src/parser.c" /* yacc.c:1652  */
        break;

        case 48:
#line 551 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ENull();
        }
#line 2059 "src/parser.c" /* yacc.c:1652  */
        break;

        case 49:
#line 552 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2065 "src/parser.c" /* yacc.c:1652  */
        break;

        case 50:
#line 554 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_Neg((yyvsp[0].expr_));
        }
#line 2071 "src/parser.c" /* yacc.c:1652  */
        break;

        case 51:
#line 555 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_Not((yyvsp[0].expr_));
        }
#line 2077 "src/parser.c" /* yacc.c:1652  */
        break;

        case 52:
#line 556 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2083 "src/parser.c" /* yacc.c:1652  */
        break;

        case 53:
#line 558 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ECast((yyvsp[-2].string_), (yyvsp[0].expr_));
        }
#line 2089 "src/parser.c" /* yacc.c:1652  */
        break;

        case 54:
#line 559 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2095 "src/parser.c" /* yacc.c:1652  */
        break;

        case 55:
#line 561 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EMul(
              (yyvsp[-2].expr_), (yyvsp[-1].mulop_), (yyvsp[0].expr_));
        }
#line 2101 "src/parser.c" /* yacc.c:1652  */
        break;

        case 56:
#line 562 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2107 "src/parser.c" /* yacc.c:1652  */
        break;

        case 57:
#line 564 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EAdd(
              (yyvsp[-2].expr_), (yyvsp[-1].addop_), (yyvsp[0].expr_));
        }
#line 2113 "src/parser.c" /* yacc.c:1652  */
        break;

        case 58:
#line 565 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2119 "src/parser.c" /* yacc.c:1652  */
        break;

        case 59:
#line 567 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ERel(
              (yyvsp[-2].expr_), (yyvsp[-1].relop_), (yyvsp[0].expr_));
        }
#line 2125 "src/parser.c" /* yacc.c:1652  */
        break;

        case 60:
#line 568 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) =
              make_EEq((yyvsp[-2].expr_), (yyvsp[-1].eqop_), (yyvsp[0].expr_));
        }
#line 2131 "src/parser.c" /* yacc.c:1652  */
        break;

        case 61:
#line 569 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2137 "src/parser.c" /* yacc.c:1652  */
        break;

        case 62:
#line 571 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EAnd((yyvsp[-2].expr_), (yyvsp[0].expr_));
        }
#line 2143 "src/parser.c" /* yacc.c:1652  */
        break;

        case 63:
#line 572 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2149 "src/parser.c" /* yacc.c:1652  */
        break;

        case 64:
#line 574 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EOr((yyvsp[-2].expr_), (yyvsp[0].expr_));
        }
#line 2155 "src/parser.c" /* yacc.c:1652  */
        break;

        case 65:
#line 575 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2161 "src/parser.c" /* yacc.c:1652  */
        break;

        case 66:
#line 577 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listexpr_) = 0;
        }
#line 2167 "src/parser.c" /* yacc.c:1652  */
        break;

        case 67:
#line 578 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listexpr_) = make_ListExpr((yyvsp[0].expr_), 0);
        }
#line 2173 "src/parser.c" /* yacc.c:1652  */
        break;

        case 68:
#line 579 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listexpr_) =
              make_ListExpr((yyvsp[-2].expr_), (yyvsp[0].listexpr_));
        }
#line 2179 "src/parser.c" /* yacc.c:1652  */
        break;

        case 69:
#line 581 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clbody_) =
              make_CBVar((yyvsp[-2].type_), (yyvsp[-1].string_));
        }
#line 2185 "src/parser.c" /* yacc.c:1652  */
        break;

        case 70:
#line 582 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clbody_) = make_CBFnDef((yyvsp[-5].type_),
                                           (yyvsp[-4].string_),
                                           (yyvsp[-2].listarg_),
                                           (yyvsp[0].block_));
        }
#line 2191 "src/parser.c" /* yacc.c:1652  */
        break;

        case 71:
#line 584 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listclbody_) = 0;
        }
#line 2197 "src/parser.c" /* yacc.c:1652  */
        break;

        case 72:
#line 585 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listclbody_) =
              make_ListClBody((yyvsp[0].clbody_), (yyvsp[-1].listclbody_));
        }
#line 2203 "src/parser.c" /* yacc.c:1652  */
        break;

        case 73:
#line 587 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clprops_) = make_CNone();
        }
#line 2209 "src/parser.c" /* yacc.c:1652  */
        break;

        case 74:
#line 588 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clprops_) = make_CExtends((yyvsp[0].string_));
        }
#line 2215 "src/parser.c" /* yacc.c:1652  */
        break;

        case 75:
#line 590 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.addop_) = make_Plus();
        }
#line 2221 "src/parser.c" /* yacc.c:1652  */
        break;

        case 76:
#line 591 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.addop_) = make_Minus();
        }
#line 2227 "src/parser.c" /* yacc.c:1652  */
        break;

        case 77:
#line 593 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.mulop_) = make_Times();
        }
#line 2233 "src/parser.c" /* yacc.c:1652  */
        break;

        case 78:
#line 594 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.mulop_) = make_Div();
        }
#line 2239 "src/parser.c" /* yacc.c:1652  */
        break;

        case 79:
#line 595 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.mulop_) = make_Mod();
        }
#line 2245 "src/parser.c" /* yacc.c:1652  */
        break;

        case 80:
#line 597 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_LTH();
        }
#line 2251 "src/parser.c" /* yacc.c:1652  */
        break;

        case 81:
#line 598 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_LE();
        }
#line 2257 "src/parser.c" /* yacc.c:1652  */
        break;

        case 82:
#line 599 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_GTH();
        }
#line 2263 "src/parser.c" /* yacc.c:1652  */
        break;

        case 83:
#line 600 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_GE();
        }
#line 2269 "src/parser.c" /* yacc.c:1652  */
        break;

        case 84:
#line 602 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.eqop_) = make_NE();
        }
#line 2275 "src/parser.c" /* yacc.c:1652  */
        break;

        case 85:
#line 603 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.eqop_) = make_EQU();
        }
#line 2281 "src/parser.c" /* yacc.c:1652  */
        break;

#line 2285 "src/parser.c" /* yacc.c:1652  */
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
    YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);

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
    yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus)
    {
        ++yynerrs;
#if !YYERROR_VERBOSE
        yyerror(YY_("syntax error"));
#else
#define YYSYNTAX_ERROR yysyntax_error(&yymsg_alloc, &yymsg, yyssp, yytoken)
        {
            char const* yymsgp = YY_("syntax error");
            int yysyntax_error_status;
            yysyntax_error_status = YYSYNTAX_ERROR;
            if (yysyntax_error_status == 0)
                yymsgp = yymsg;
            else if (yysyntax_error_status == 1)
            {
                if (yymsg != yymsgbuf)
                    YYSTACK_FREE(yymsg);
                yymsg = (char*)YYSTACK_ALLOC(yymsg_alloc);
                if (!yymsg)
                {
                    yymsg = yymsgbuf;
                    yymsg_alloc = sizeof yymsgbuf;
                    yysyntax_error_status = 2;
                }
                else
                {
                    yysyntax_error_status = YYSYNTAX_ERROR;
                    yymsgp = yymsg;
                }
            }
            yyerror(yymsgp);
            if (yysyntax_error_status == 2)
                goto yyexhaustedlab;
        }
#undef YYSYNTAX_ERROR
#endif
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

    for (;;)
    {
        yyn = yypact[yystate];
        if (!yypact_value_is_default(yyn))
        {
            yyn += YYTERROR;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
                yyn = yytable[yyn];
                if (0 < yyn)
                    break;
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
            YYABORT;

        yydestruct("Error: popping", yystos[yystate], yyvsp);
        YYPOPSTACK(1);
        yystate = *yyssp;
        YY_STACK_PRINT(yyss, yyssp);
    }

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    /* Shift the error token.  */
    YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined yyoverflow || YYERROR_VERBOSE
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
        yydestruct("Cleanup: popping", yystos[*yyssp], yyvsp);
        YYPOPSTACK(1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
        YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
    if (yymsg != yymsgbuf)
        YYSTACK_FREE(yymsg);
#endif
    return yyresult;
}
