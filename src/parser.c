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
extern int
initialize_lexer(FILE* inp);
void
yyerror(const char* str)
{
    extern char* lattetext;
    fprintf(stderr,
            "ERROR\n%s:%d: error: %s at %s\n",
            "TODO.lat",
            yy_mylinenumber + 1,
            str,
            lattetext);
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

#line 443 "src/parser.c" /* yacc.c:337  */
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
#line 368 "src/latte.y" /* yacc.c:352  */

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

#line 556 "src/parser.c" /* yacc.c:352  */
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
#define YYLAST 200

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 31
/* YYNRULES -- Number of rules.  */
#define YYNRULES 83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 150

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
    0,   473, 473, 475, 476, 478, 479, 481, 483, 484, 485, 487, 489, 490,
    492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 506,
    507, 509, 510, 512, 513, 515, 516, 517, 519, 520, 522, 523, 525, 526,
    527, 529, 530, 531, 532, 533, 534, 536, 537, 538, 540, 541, 543, 544,
    546, 547, 549, 550, 552, 553, 555, 556, 558, 559, 560, 562, 563, 565,
    566, 568, 569, 571, 572, 574, 575, 576, 578, 579, 580, 581, 582, 583
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
    "MulOp",    "RelOp",    YY_NULLPTR
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

#define YYPACT_NINF -125

#define yypact_value_is_default(Yystate) (!!((Yystate) == (-125)))

#define YYTABLE_NINF -32

#define yytable_value_is_error(Yytable_value) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    -19,  -33,  3,    21,   -19,  -125, -13,  -10,  -125, -125, -125, 29,
    -7,   39,   12,   -125, 12,   27,   55,   18,   23,   12,   57,   12,
    56,   -125, 7,    -125, -125, -125, -125, -125, 12,   -125, 52,   61,
    126,  -125, -125, 144,  144,  -125, 68,   69,   12,   -125, 108,  -125,
    70,   -125, -125, -8,   -125, -125, 31,   -125, 73,   64,   65,   -125,
    -125, -125, 17,   -3,   24,   59,   19,   56,   77,   78,   155,  -125,
    -125, -125, 40,   155,  82,   -125, 75,   155,  76,   91,   93,   155,
    60,   155,  -125, -125, -125, 155,  -125, -125, 155,  155,  -125, -125,
    -125, -125, -125, -125, 155,  155,  -125, 155,  98,   99,   -125, 144,
    -125, 66,   104,  155,  -125, 105,  155,  31,   -125, 106,  113,  -125,
    96,   -125, 17,   -125, -3,   -125, 110,  -125, -125, -125, 107,  97,
    111,  97,   -125, -125, 155,  -125, -125, -125, 155,  88,   -125, -125,
    -125, 117,  97,   97,   -125, -125
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    0,  0,  31, 0,  5,  2,  0,  71, 32, 1,  6,  0,  0,  0,  8,  72, 0,  9,  0,
    0,  0,  69, 0,  8,  0,  7,  0,  70, 4,  10, 12, 3,  8,  67, 0,  0,  0,  11,
    14, 0,  0,  46, 0,  0,  0,  47, 0,  45, 0,  44, 43, 33, 15, 13, 0,  37, 39,
    42, 48, 51, 53, 55, 57, 59, 61, 63, 0,  0,  33, 0,  0,  33, 49, 50, 0,  0,
    40, 21, 0,  0,  27, 29, 0,  64, 0,  0,  75, 76, 77, 0,  74, 73, 0,  0,  78,
    79, 80, 81, 82, 83, 0,  0,  26, 0,  0,  0,  68, 0,  35, 0,  0,  0,  20, 0,
    0,  0,  16, 65, 0,  36, 0,  54, 56, 60, 58, 62, 0,  18, 19, 52, 0,  0,  0,
    0,  28, 30, 64, 34, 38, 17, 0,  22, 41, 24, 66, 0,  0,  0,  23, 25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = { -125, -125, -125, 119, -125, -18,  -21,
                                        -125, -124, -125, 9,   4,    -125, -125,
                                        -125, -125, -125, -38, 36,   37,   50,
                                        -125, 38,   -36,  20,  -125, 133,  -125,
                                        -125, -125, -125 };

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] = { -1,  3,  4,  5,  17, 18, 52, 34,
                                         53,  81, 82, 54, 55, 56, 57, 58,
                                         59,  60, 61, 62, 63, 64, 65, 66,
                                         118, 21, 22, 13, 92, 89, 100 };

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] = {
    69,  72,  73,  31,  6,   29,  8,   141, 6,   143, 78,  32,  7,   1,   35,
    90,  33,  8,   19,  91,  20,  9,   148, 149, 12,  20,  2,   19,  102, 103,
    104, 105, 11,  14,  69,  23,  19,  -31, 15,  110, 86,  87,  88,  113, 93,
    16,  106, 117, 76,  120, 94,  95,  96,  97,  98,  99,  36,  2,   30,  37,
    24,  38,  30,  25,  28,  125, 67,  126, 26,  129, 39,  40,  74,  75,  79,
    132, 80,  83,  134, 84,  101, 85,  107, 108, 112, 109, 114, 41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  111, 115, 117, 36,  116, 30,  145,
    119, 38,  127, 128, 131, 133, 130, 36,  138, 136, 39,  40,  77,  137, 139,
    140, 146, 147, 10,  135, 121, 39,  40,  142, 122, 36,  123, 41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  41,  39,  40,  44,  45,  70,  47,
    124, 49,  50,  71,  27,  0,   144, 0,   0,   36,  0,   41,  39,  40,  44,
    45,  0,   47,  0,   49,  50,  68,  0,   39,  40,  0,   0,   0,   0,   41,
    0,   0,   44,  45,  0,   47,  0,   49,  50,  71,  41,  0,   0,   44,  45,
    0,   47,  0,   49,  50,  71
};

static const yytype_int16 yycheck[] = {
    36, 39,  40, 24,  0,   23,  14,  131, 4,   133, 46, 4,   45,  32, 32,  18,
    9,  14,  14, 22,  16,  0,   146, 147, 34,  21,  45, 23,  9,   10, 11,  12,
    45, 4,   70, 8,   32,  45,  45,  75,  23,  24,  25, 79,  20,  6,  67,  83,
    44, 85,  26, 27,  28,  29,  30,  31,  4,   45,  6,  7,   5,   9,  6,   45,
    7,  101, 5,  103, 45,  107, 18,  19,  4,   4,   4,  111, 45,  4,  114, 15,
    21, 16,  5,  5,   9,   45,  10,  35,  36,  37,  38, 39,  40,  41, 42,  43,
    44, 45,  16, 8,   136, 4,   9,   6,   140, 45,  9,  9,   9,   5,  5,   45,
    4,  17,  8,  18,  19,  9,   5,   9,   13,  33,  5,  4,   115, 89, 18,  19,
    17, 92,  4,  93,  35,  36,  37,  38,  39,  40,  41, 42,  43,  44, 45,  35,
    18, 19,  38, 39,  4,   41,  100, 43,  44,  45,  21, -1,  136, -1, -1,  4,
    -1, 35,  18, 19,  38,  39,  -1,  41,  -1,  43,  44, 45,  -1,  18, 19,  -1,
    -1, -1,  -1, 35,  -1,  -1,  38,  39,  -1,  41,  -1, 43,  44,  45, 35,  -1,
    -1, 38,  39, -1,  41,  -1,  43,  44,  45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  32, 45, 47, 48, 49, 57, 45, 14, 0,  49, 45, 34, 73, 4,  45, 6,  50, 51,
    57, 57, 71, 72, 8,  5,  45, 45, 72, 7,  51, 6,  52, 4,  9,  53, 51, 4,  7,
    9,  18, 19, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 52, 54, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 5,  45, 69, 4,  45, 63, 63, 4,  4,
    57, 9,  69, 4,  45, 55, 56, 4,  15, 16, 23, 24, 25, 75, 18, 22, 74, 20, 26,
    27, 28, 29, 30, 31, 76, 21, 9,  10, 11, 12, 52, 5,  5,  45, 69, 16, 9,  69,
    10, 8,  9,  69, 70, 45, 69, 64, 65, 68, 66, 69, 69, 9,  9,  63, 45, 5,  69,
    5,  69, 56, 8,  5,  17, 9,  13, 54, 17, 54, 70, 69, 33, 5,  54, 54
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  46, 47, 48, 48, 49, 49, 50, 51, 51, 51, 52, 53, 53, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 55, 55, 56, 56, 57, 57, 58,
    58, 58, 59, 59, 60, 60, 61, 61, 61, 62, 62, 62, 62, 62, 62, 63, 63,
    63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 70, 71,
    71, 72, 72, 73, 73, 74, 74, 75, 75, 75, 76, 76, 76, 76, 76, 76
};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = { 0, 2, 1, 6, 6, 1, 2, 2, 0, 1, 3, 3, 0, 2,
                                     1, 1, 3, 4, 3, 3, 3, 2, 5, 7, 5, 8, 2, 1,
                                     3, 1, 3, 1, 2, 1, 4, 3, 3, 1, 4, 1, 2, 5,
                                     1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 4, 1, 3, 1,
                                     3, 1, 3, 1, 3, 1, 3, 1, 0, 1, 3, 3, 6, 1,
                                     2, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

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
#line 473 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.program_) = make_Prog((yyvsp[0].listtopdef_));
            YY_RESULT_Program_ = (yyval.program_);
        }
#line 1767 "src/parser.c" /* yacc.c:1652  */
        break;

        case 3:
#line 475 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.topdef_) = make_FnDef((yyvsp[-5].type_),
                                         (yyvsp[-4].string_),
                                         (yyvsp[-2].listarg_),
                                         (yyvsp[0].block_));
        }
#line 1773 "src/parser.c" /* yacc.c:1652  */
        break;

        case 4:
#line 476 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.topdef_) = make_ClDef((yyvsp[-4].string_),
                                         (yyvsp[-3].clprops_),
                                         (yyvsp[-1].listclbody_));
        }
#line 1779 "src/parser.c" /* yacc.c:1652  */
        break;

        case 5:
#line 478 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listtopdef_) = make_ListTopDef((yyvsp[0].topdef_), 0);
        }
#line 1785 "src/parser.c" /* yacc.c:1652  */
        break;

        case 6:
#line 479 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listtopdef_) =
              make_ListTopDef((yyvsp[-1].topdef_), (yyvsp[0].listtopdef_));
        }
#line 1791 "src/parser.c" /* yacc.c:1652  */
        break;

        case 7:
#line 481 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.arg_) = make_Ar((yyvsp[-1].type_), (yyvsp[0].string_));
        }
#line 1797 "src/parser.c" /* yacc.c:1652  */
        break;

        case 8:
#line 483 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listarg_) = 0;
        }
#line 1803 "src/parser.c" /* yacc.c:1652  */
        break;

        case 9:
#line 484 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listarg_) = make_ListArg((yyvsp[0].arg_), 0);
        }
#line 1809 "src/parser.c" /* yacc.c:1652  */
        break;

        case 10:
#line 485 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listarg_) =
              make_ListArg((yyvsp[-2].arg_), (yyvsp[0].listarg_));
        }
#line 1815 "src/parser.c" /* yacc.c:1652  */
        break;

        case 11:
#line 487 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.block_) = make_Blk(reverseListStmt((yyvsp[-1].liststmt_)));
        }
#line 1821 "src/parser.c" /* yacc.c:1652  */
        break;

        case 12:
#line 489 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.liststmt_) = 0;
        }
#line 1827 "src/parser.c" /* yacc.c:1652  */
        break;

        case 13:
#line 490 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.liststmt_) =
              make_ListStmt((yyvsp[0].stmt_), (yyvsp[-1].liststmt_));
        }
#line 1833 "src/parser.c" /* yacc.c:1652  */
        break;

        case 14:
#line 492 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Empty();
        }
#line 1839 "src/parser.c" /* yacc.c:1652  */
        break;

        case 15:
#line 493 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_BStmt((yyvsp[0].block_));
        }
#line 1845 "src/parser.c" /* yacc.c:1652  */
        break;

        case 16:
#line 494 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Decl((yyvsp[-2].type_), (yyvsp[-1].listitem_));
        }
#line 1851 "src/parser.c" /* yacc.c:1652  */
        break;

        case 17:
#line 495 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Ass((yyvsp[-3].expr_), (yyvsp[-1].expr_));
        }
#line 1857 "src/parser.c" /* yacc.c:1652  */
        break;

        case 18:
#line 496 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Incr((yyvsp[-2].expr_));
        }
#line 1863 "src/parser.c" /* yacc.c:1652  */
        break;

        case 19:
#line 497 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Decr((yyvsp[-2].expr_));
        }
#line 1869 "src/parser.c" /* yacc.c:1652  */
        break;

        case 20:
#line 498 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Ret((yyvsp[-1].expr_));
        }
#line 1875 "src/parser.c" /* yacc.c:1652  */
        break;

        case 21:
#line 499 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_VRet();
        }
#line 1881 "src/parser.c" /* yacc.c:1652  */
        break;

        case 22:
#line 500 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_Cond((yyvsp[-2].expr_), (yyvsp[0].stmt_));
        }
#line 1887 "src/parser.c" /* yacc.c:1652  */
        break;

        case 23:
#line 501 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_CondElse(
              (yyvsp[-4].expr_), (yyvsp[-2].stmt_), (yyvsp[0].stmt_));
        }
#line 1893 "src/parser.c" /* yacc.c:1652  */
        break;

        case 24:
#line 502 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_While((yyvsp[-2].expr_), (yyvsp[0].stmt_));
        }
#line 1899 "src/parser.c" /* yacc.c:1652  */
        break;

        case 25:
#line 503 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_For((yyvsp[-5].string_),
                                     (yyvsp[-4].string_),
                                     (yyvsp[-2].expr_),
                                     (yyvsp[0].stmt_));
        }
#line 1905 "src/parser.c" /* yacc.c:1652  */
        break;

        case 26:
#line 504 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.stmt_) = make_SExp((yyvsp[-1].expr_));
        }
#line 1911 "src/parser.c" /* yacc.c:1652  */
        break;

        case 27:
#line 506 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.item_) = make_NoInit((yyvsp[0].string_));
        }
#line 1917 "src/parser.c" /* yacc.c:1652  */
        break;

        case 28:
#line 507 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.item_) = make_Init((yyvsp[-2].string_), (yyvsp[0].expr_));
        }
#line 1923 "src/parser.c" /* yacc.c:1652  */
        break;

        case 29:
#line 509 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listitem_) = make_ListItem((yyvsp[0].item_), 0);
        }
#line 1929 "src/parser.c" /* yacc.c:1652  */
        break;

        case 30:
#line 510 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listitem_) =
              make_ListItem((yyvsp[-2].item_), (yyvsp[0].listitem_));
        }
#line 1935 "src/parser.c" /* yacc.c:1652  */
        break;

        case 31:
#line 512 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.type_) = make_TCls((yyvsp[0].string_));
        }
#line 1941 "src/parser.c" /* yacc.c:1652  */
        break;

        case 32:
#line 513 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.type_) = make_TArr((yyvsp[-1].string_));
        }
#line 1947 "src/parser.c" /* yacc.c:1652  */
        break;

        case 33:
#line 515 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EVar((yyvsp[0].string_));
        }
#line 1953 "src/parser.c" /* yacc.c:1652  */
        break;

        case 34:
#line 516 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EApp((yyvsp[-3].expr_), (yyvsp[-1].listexpr_));
        }
#line 1959 "src/parser.c" /* yacc.c:1652  */
        break;

        case 35:
#line 517 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[-1].expr_);
        }
#line 1965 "src/parser.c" /* yacc.c:1652  */
        break;

        case 36:
#line 519 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EClMem((yyvsp[-2].expr_), (yyvsp[0].string_));
        }
#line 1971 "src/parser.c" /* yacc.c:1652  */
        break;

        case 37:
#line 520 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 1977 "src/parser.c" /* yacc.c:1652  */
        break;

        case 38:
#line 522 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EArrApp((yyvsp[-3].expr_), (yyvsp[-1].expr_));
        }
#line 1983 "src/parser.c" /* yacc.c:1652  */
        break;

        case 39:
#line 523 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 1989 "src/parser.c" /* yacc.c:1652  */
        break;

        case 40:
#line 525 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ENew((yyvsp[0].type_));
        }
#line 1995 "src/parser.c" /* yacc.c:1652  */
        break;

        case 41:
#line 526 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ENewArr((yyvsp[-3].type_), (yyvsp[-1].expr_));
        }
#line 2001 "src/parser.c" /* yacc.c:1652  */
        break;

        case 42:
#line 527 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2007 "src/parser.c" /* yacc.c:1652  */
        break;

        case 43:
#line 529 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitInt((yyvsp[0].int_));
        }
#line 2013 "src/parser.c" /* yacc.c:1652  */
        break;

        case 44:
#line 530 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitStr((yyvsp[0].string_));
        }
#line 2019 "src/parser.c" /* yacc.c:1652  */
        break;

        case 45:
#line 531 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitTrue();
        }
#line 2025 "src/parser.c" /* yacc.c:1652  */
        break;

        case 46:
#line 532 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ELitFalse();
        }
#line 2031 "src/parser.c" /* yacc.c:1652  */
        break;

        case 47:
#line 533 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ENull();
        }
#line 2037 "src/parser.c" /* yacc.c:1652  */
        break;

        case 48:
#line 534 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2043 "src/parser.c" /* yacc.c:1652  */
        break;

        case 49:
#line 536 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_Neg((yyvsp[0].expr_));
        }
#line 2049 "src/parser.c" /* yacc.c:1652  */
        break;

        case 50:
#line 537 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_Not((yyvsp[0].expr_));
        }
#line 2055 "src/parser.c" /* yacc.c:1652  */
        break;

        case 51:
#line 538 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2061 "src/parser.c" /* yacc.c:1652  */
        break;

        case 52:
#line 540 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ECast((yyvsp[-2].string_), (yyvsp[0].expr_));
        }
#line 2067 "src/parser.c" /* yacc.c:1652  */
        break;

        case 53:
#line 541 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2073 "src/parser.c" /* yacc.c:1652  */
        break;

        case 54:
#line 543 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EMul(
              (yyvsp[-2].expr_), (yyvsp[-1].mulop_), (yyvsp[0].expr_));
        }
#line 2079 "src/parser.c" /* yacc.c:1652  */
        break;

        case 55:
#line 544 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2085 "src/parser.c" /* yacc.c:1652  */
        break;

        case 56:
#line 546 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EAdd(
              (yyvsp[-2].expr_), (yyvsp[-1].addop_), (yyvsp[0].expr_));
        }
#line 2091 "src/parser.c" /* yacc.c:1652  */
        break;

        case 57:
#line 547 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2097 "src/parser.c" /* yacc.c:1652  */
        break;

        case 58:
#line 549 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_ERel(
              (yyvsp[-2].expr_), (yyvsp[-1].relop_), (yyvsp[0].expr_));
        }
#line 2103 "src/parser.c" /* yacc.c:1652  */
        break;

        case 59:
#line 550 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2109 "src/parser.c" /* yacc.c:1652  */
        break;

        case 60:
#line 552 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EAnd((yyvsp[-2].expr_), (yyvsp[0].expr_));
        }
#line 2115 "src/parser.c" /* yacc.c:1652  */
        break;

        case 61:
#line 553 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2121 "src/parser.c" /* yacc.c:1652  */
        break;

        case 62:
#line 555 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = make_EOr((yyvsp[-2].expr_), (yyvsp[0].expr_));
        }
#line 2127 "src/parser.c" /* yacc.c:1652  */
        break;

        case 63:
#line 556 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.expr_) = (yyvsp[0].expr_);
        }
#line 2133 "src/parser.c" /* yacc.c:1652  */
        break;

        case 64:
#line 558 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listexpr_) = 0;
        }
#line 2139 "src/parser.c" /* yacc.c:1652  */
        break;

        case 65:
#line 559 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listexpr_) = make_ListExpr((yyvsp[0].expr_), 0);
        }
#line 2145 "src/parser.c" /* yacc.c:1652  */
        break;

        case 66:
#line 560 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listexpr_) =
              make_ListExpr((yyvsp[-2].expr_), (yyvsp[0].listexpr_));
        }
#line 2151 "src/parser.c" /* yacc.c:1652  */
        break;

        case 67:
#line 562 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clbody_) =
              make_CBVar((yyvsp[-2].type_), (yyvsp[-1].string_));
        }
#line 2157 "src/parser.c" /* yacc.c:1652  */
        break;

        case 68:
#line 563 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clbody_) = make_CBFnDef((yyvsp[-5].type_),
                                           (yyvsp[-4].string_),
                                           (yyvsp[-2].listarg_),
                                           (yyvsp[0].block_));
        }
#line 2163 "src/parser.c" /* yacc.c:1652  */
        break;

        case 69:
#line 565 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listclbody_) = make_ListClBody((yyvsp[0].clbody_), 0);
        }
#line 2169 "src/parser.c" /* yacc.c:1652  */
        break;

        case 70:
#line 566 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.listclbody_) =
              make_ListClBody((yyvsp[-1].clbody_), (yyvsp[0].listclbody_));
        }
#line 2175 "src/parser.c" /* yacc.c:1652  */
        break;

        case 71:
#line 568 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clprops_) = make_CNone();
        }
#line 2181 "src/parser.c" /* yacc.c:1652  */
        break;

        case 72:
#line 569 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.clprops_) = make_CExtends((yyvsp[0].string_));
        }
#line 2187 "src/parser.c" /* yacc.c:1652  */
        break;

        case 73:
#line 571 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.addop_) = make_Plus();
        }
#line 2193 "src/parser.c" /* yacc.c:1652  */
        break;

        case 74:
#line 572 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.addop_) = make_Minus();
        }
#line 2199 "src/parser.c" /* yacc.c:1652  */
        break;

        case 75:
#line 574 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.mulop_) = make_Times();
        }
#line 2205 "src/parser.c" /* yacc.c:1652  */
        break;

        case 76:
#line 575 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.mulop_) = make_Div();
        }
#line 2211 "src/parser.c" /* yacc.c:1652  */
        break;

        case 77:
#line 576 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.mulop_) = make_Mod();
        }
#line 2217 "src/parser.c" /* yacc.c:1652  */
        break;

        case 78:
#line 578 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_LTH();
        }
#line 2223 "src/parser.c" /* yacc.c:1652  */
        break;

        case 79:
#line 579 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_LE();
        }
#line 2229 "src/parser.c" /* yacc.c:1652  */
        break;

        case 80:
#line 580 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_GTH();
        }
#line 2235 "src/parser.c" /* yacc.c:1652  */
        break;

        case 81:
#line 581 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_GE();
        }
#line 2241 "src/parser.c" /* yacc.c:1652  */
        break;

        case 82:
#line 582 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_EQU();
        }
#line 2247 "src/parser.c" /* yacc.c:1652  */
        break;

        case 83:
#line 583 "src/latte.y" /* yacc.c:1652  */
        {
            (yyval.relop_) = make_NE();
        }
#line 2253 "src/parser.c" /* yacc.c:1652  */
        break;

#line 2257 "src/parser.c" /* yacc.c:1652  */
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
