/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C

      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         __reginaparse
#define yylex           __reginalex
#define yyerror         __reginaerror
#define yylval          __reginalval
#define yychar          __reginachar
#define yydebug         __reginadebug
#define yynerrs         __reginanerrs


/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "./yaccsrc.y"


#ifndef lint
static char *RCSid = "$Id: yaccsrc.c,v 1.45 2013/07/19 10:13:38 mark Exp $";
#endif

/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992-1994  Anders Christensen <anders@pvv.unit.no>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rexx.h"
#include <time.h>

#if defined(HAVE_MALLOC_H)
# include <malloc.h>
#endif

#if defined(HAVE_ALLOCA_H)
# include <alloca.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#if defined(_MSC_VER) || defined(MAC)
# define __STDC__ 1 /* Hack to allow const since it is not defined */
#endif

#define YYSTYPE nodeptr

/* locals, they are protected by regina_parser (see lexsrc.l) */
static int tmplno,           /* lineno of current instruction             */
           tmpchr,           /* character position of current instruction */
           level,            /* nested do/if/select depth                 */
           start_parendepth; /* see below at parendepth                   */

/*
 * parendepth regulates the action which happens detecting a comma or an
 * empty expression. A negative values indicates an error; both a comma
 * and an empty expression raise an error.
 * We regulate the enumeration of arguments with this semantical flag.
 * Look at "call subroutine args" and "function args". Function itself
 * contains a parentheses pair, so starting with a depth of just allows
 * the enumeration. subroutine starts with either 0 or 1. The latter one
 * is allowed for the support request of "call subroutine(a,b,c)" which
 * isn't allowed by ANSI but can be enabled for backward compatibility.
 */
static int parendepth;

static nodeptr current, with = NULL;

static char *nullptr = NULL; /* for C++ compilation */

typedef enum { IS_UNKNOWN,
               IS_A_NUMBER,
               IS_NO_NUMBER,
               IS_SIM_SYMBOL,
               IS_COMP_SYMBOL } node_type;

typedef enum { REDUCE_CALL,
               REDUCE_EXPR,
               REDUCE_RIGHT,
               REDUCE_SUBEXPR } reduce_mode;

static node_type gettypeof( nodeptr thisptr ) ;
static void checkconst( nodeptr thisptr ) ;
static nodeptr reduce_expr_list( nodeptr thisptr, reduce_mode mode );
static void transform( nodeptr thisptr ) ;
static nodeptr create_head( const char *name ) ;
static nodeptr makenode( int type, int numb, ... ) ;
static void checkdosyntax( cnodeptr thisptr ) ;
void newlabel( const tsd_t *TSD, internal_parser_type *ipt, nodeptr thisptr ) ;
static nodeptr optgluelast( nodeptr p1, nodeptr p2 );
static void move_labels( nodeptr front, nodeptr end, int level );

#define IS_EXPRLIST(x) ( ( (x) != NULL )                   \
                      && ( ( (x)->type == X_CEXPRLIST )    \
                        || ( (x)->type == X_EXPRLIST ) ) )

#define IS_FUNCTION(x) ( ( (x) != NULL )                  \
                      && ( ( (x)->type == X_EX_FUNC )     \
                        || ( (x)->type == X_IN_FUNC ) ) )

#define AUTO_REDUCE(x,y) { if ( parendepth == 1 )                             \
                           {                                                  \
                             x = reduce_expr_list( x, REDUCE_EXPR );          \
                             /* detect "call s (a,b)<op>" and      */         \
                             /* "call s ()<op>"                    */         \
                             if ( IS_EXPRLIST( x ) )                          \
                             {                                                \
                                if ( (y) != NULL )                            \
                                   exiterror( ERR_INVALID_EXPRESSION, 1, y ); \
                                else if ( (x)->p[0] == NULL )                 \
                                   exiterror( ERR_UNEXPECTED_PARAN, 0 );      \
                                else                                          \
                                   exiterror( ERR_UNEXPECTED_PARAN, 1 );      \
                             }                                                \
                           }                                                  \
                         }

/* Line 268 of yacc.c  */
#line 149 "./yaccsrc.y"

#ifdef NDEBUG
# define YYDEBUG 0
#else
# define YYDEBUG 1
#endif


/* Line 268 of yacc.c  */
#line 207 "./yaccsrc.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ADDRESS = 258,
     ARG = 259,
     CALL = 260,
     DO = 261,
     TO = 262,
     BY = 263,
     FOR = 264,
     WHILE = 265,
     UNTIL = 266,
     EXIT = 267,
     IF = 268,
     THEN = 269,
     ELSE = 270,
     ITERATE = 271,
     INTERPRET = 272,
     LEAVE = 273,
     NOP = 274,
     NUMERIC = 275,
     PARSE = 276,
     EXTERNAL = 277,
     SOURCE = 278,
     VAR = 279,
     VALUE = 280,
     WITH = 281,
     PROCEDURE = 282,
     EXPOSE = 283,
     PULL = 284,
     PUSH = 285,
     QUEUE = 286,
     SAY = 287,
     RETURN = 288,
     SELECT = 289,
     WHEN = 290,
     DROP = 291,
     OTHERWISE = 292,
     SIGNAL = 293,
     ON = 294,
     OFF = 295,
     SYNTAX = 296,
     HALT = 297,
     NOVALUE = 298,
     TRACE = 299,
     END = 300,
     UPPER = 301,
     ASSIGNMENTVARIABLE = 302,
     STATSEP = 303,
     FOREVER = 304,
     DIGITS = 305,
     FORM = 306,
     FUZZ = 307,
     SCIENTIFIC = 308,
     ENGINEERING = 309,
     NOT = 310,
     CONCATENATE = 311,
     MODULUS = 312,
     GTE = 313,
     GT = 314,
     LTE = 315,
     LT = 316,
     DIFFERENT = 317,
     EQUALEQUAL = 318,
     NOTEQUALEQUAL = 319,
     OFFSET = 320,
     SPACE = 321,
     EXP = 322,
     XOR = 323,
     PLACEHOLDER = 324,
     NOTREADY = 325,
     CONSYMBOL = 326,
     SIMSYMBOL = 327,
     EXFUNCNAME = 328,
     INFUNCNAME = 329,
     LABEL = 330,
     DOVARIABLE = 331,
     HEXSTRING = 332,
     STRING = 333,
     VERSION = 334,
     LINEIN = 335,
     WHATEVER = 336,
     NAME = 337,
     FAILURE = 338,
     BINSTRING = 339,
     OPTIONS = 340,
     ENVIRONMENT = 341,
     LOSTDIGITS = 342,
     GTGT = 343,
     LTLT = 344,
     NOTGTGT = 345,
     NOTLTLT = 346,
     GTGTE = 347,
     LTLTE = 348,
     INPUT = 349,
     OUTPUT = 350,
     ERROR = 351,
     NORMAL = 352,
     APPEND = 353,
     REPLACE = 354,
     STREAM = 355,
     STEM = 356,
     LIFO = 357,
     FIFO = 358,
     LOWER = 359,
     CASELESS = 360,
     CCAT = 361,
     UPLUS = 362,
     UMINUS = 363
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 357 "./yaccsrc.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
        && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
    || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)            \
    do                           \
      {                          \
   YYSIZE_T yynewbytes;                \
   YYCOPY (&yyptr->Stack_alloc, Stack, yysize);       \
   Stack = &yyptr->Stack_alloc;              \
   yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
   yyptr += yynewbytes / sizeof (*yyptr);          \
      }                          \
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)     \
      do             \
   {              \
     YYSIZE_T yyi;            \
     for (yyi = 0; yyi < (Count); yyi++)  \
       (To)[yyi] = (From)[yyi];     \
   }              \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2188

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  120
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  176
/* YYNRULES -- Number of rules.  */
#define YYNRULES  447
/* YYNRULES -- Number of states.  */
#define YYNSTATES  611

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   363

#define YYTRANSLATE(YYX)                  \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   114,   107,     2,
     117,   118,   112,   110,   119,   111,     2,   113,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   108,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   106,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   109,   115,   116
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    12,    15,    17,    20,
      23,    25,    27,    31,    33,    35,    39,    42,    45,    47,
      50,    51,    53,    55,    56,    59,    62,    64,    66,    68,
      70,    72,    74,    76,    78,    80,    82,    84,    86,    88,
      90,    92,    94,    96,    98,   100,   102,   104,   106,   108,
     110,   112,   114,   116,   118,   120,   122,   124,   126,   128,
     130,   132,   134,   136,   138,   140,   142,   144,   146,   148,
     150,   152,   154,   156,   158,   160,   162,   164,   166,   168,
     170,   172,   173,   177,   181,   183,   184,   185,   189,   193,
     199,   202,   203,   208,   209,   214,   218,   222,   227,   233,
     238,   242,   247,   251,   253,   255,   257,   260,   262,   264,
     267,   269,   273,   280,   288,   295,   301,   306,   309,   312,
     315,   316,   318,   320,   322,   323,   326,   327,   330,   334,
     337,   339,   341,   343,   345,   348,   351,   354,   357,   361,
     364,   368,   370,   373,   377,   380,   384,   386,   389,   393,
     396,   400,   402,   407,   411,   416,   420,   425,   429,   431,
     433,   435,   438,   441,   444,   447,   449,   452,   455,   458,
     461,   464,   467,   470,   473,   474,   478,   479,   483,   485,
     486,   488,   490,   492,   494,   496,   500,   504,   508,   512,
     516,   517,   521,   524,   528,   531,   534,   541,   551,   561,
     568,   571,   576,   579,   582,   585,   588,   592,   594,   596,
     598,   601,   605,   607,   609,   612,   616,   619,   624,   628,
     631,   636,   640,   643,   647,   650,   652,   654,   657,   662,
     666,   670,   673,   675,   678,   681,   683,   686,   689,   691,
     695,   697,   699,   701,   703,   705,   707,   709,   712,   716,
     719,   721,   724,   728,   733,   737,   740,   743,   746,   749,
     752,   755,   759,   761,   767,   772,   777,   780,   784,   790,
     793,   795,   797,   804,   812,   815,   818,   821,   824,   828,
     829,   833,   837,   840,   844,   848,   853,   859,   864,   868,
     873,   877,   879,   881,   883,   886,   889,   892,   894,   896,
     898,   900,   902,   904,   906,   908,   910,   912,   914,   916,
     920,   923,   927,   930,   932,   935,   937,   938,   942,   943,
     948,   949,   954,   955,   960,   962,   963,   968,   970,   971,
     976,   978,   979,   984,   986,   987,   992,   994,   995,  1000,
    1002,  1003,  1008,  1010,  1011,  1016,  1018,  1019,  1024,  1026,
    1027,  1032,  1034,  1037,  1039,  1040,  1045,  1047,  1048,  1053,
    1055,  1056,  1061,  1063,  1064,  1069,  1071,  1072,  1077,  1079,
    1080,  1085,  1087,  1088,  1093,  1095,  1096,  1101,  1103,  1104,
    1109,  1111,  1112,  1117,  1119,  1120,  1125,  1127,  1128,  1133,
    1135,  1136,  1141,  1143,  1144,  1149,  1151,  1153,  1155,  1157,
    1159,  1161,  1163,  1166,  1169,  1172,  1175,  1178,  1181,  1183,
    1185,  1188,  1191,  1192,  1195,  1197,  1199,  1203,  1205,  1207,
    1210,  1213,  1215,  1218,  1222,  1227,  1232,  1237,  1239,  1241,
    1243,  1245,  1247,  1249,  1252,  1255,  1256,  1257,  1262,  1264,
    1266,  1267,  1270,  1272,  1277,  1281,  1283,  1285
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     121,     0,    -1,    -1,   122,   123,    -1,   130,   124,    -1,
     130,    -1,   124,   126,    -1,   126,    -1,   125,   132,    -1,
     132,   133,    -1,   132,    -1,   127,    -1,   133,   176,   129,
      -1,   125,    -1,   133,    -1,   129,    48,   131,    -1,    48,
     131,    -1,   131,   129,    -1,   131,    -1,   131,   215,    -1,
      -1,   134,    -1,   236,    -1,    -1,   133,   135,    -1,   136,
     129,    -1,   210,    -1,   211,    -1,   212,    -1,   164,    -1,
     174,    -1,   168,    -1,   169,    -1,   177,    -1,   207,    -1,
     209,    -1,   213,    -1,   214,    -1,   217,    -1,   218,    -1,
     219,    -1,   221,    -1,   222,    -1,   226,    -1,   227,    -1,
     228,    -1,   229,    -1,   231,    -1,   230,    -1,   233,    -1,
     238,    -1,   246,    -1,   208,    -1,   248,    -1,     5,    -1,
       6,    -1,    12,    -1,    13,    -1,    16,    -1,    18,    -1,
      32,    -1,    33,    -1,     3,    -1,     4,    -1,    36,    -1,
      17,    -1,    75,    -1,    19,    -1,    20,    -1,    85,    -1,
      21,    -1,    27,    -1,    29,    -1,    30,    -1,    31,    -1,
      34,    -1,    38,    -1,    35,    -1,    37,    -1,    44,    -1,
      46,    -1,    -1,   145,   165,   166,    -1,    25,   250,   180,
      -1,   182,    -1,    -1,    -1,     1,   167,   180,    -1,   179,
     292,   180,    -1,   117,   250,   118,   200,   180,    -1,   146,
     224,    -1,    -1,   137,   172,   170,   173,    -1,    -1,   137,
     288,   171,   173,    -1,   137,   242,     1,    -1,   137,   243,
       1,    -1,   137,   242,   244,     1,    -1,   137,   242,   244,
     240,     1,    -1,   137,   242,   244,   240,    -1,   137,   242,
     244,    -1,   137,   243,   244,     1,    -1,   137,   243,   244,
      -1,   241,    -1,     1,    -1,   290,    -1,   290,   118,    -1,
     250,    -1,    45,    -1,   175,   295,    -1,   175,    -1,   175,
     295,     1,    -1,   138,   178,   206,   129,   128,   176,    -1,
     204,   108,   250,   200,   205,   205,   205,    -1,   204,   108,
     250,   200,   205,   205,    -1,   204,   108,   250,   200,   205,
      -1,   204,   108,   250,   200,    -1,    49,   200,    -1,    49,
       1,    -1,   250,   200,    -1,    -1,    71,    -1,    72,    -1,
      78,    -1,    -1,   181,   182,    -1,    -1,    26,   183,    -1,
      26,   183,     1,    -1,    26,   200,    -1,   184,    -1,   185,
      -1,   186,    -1,     1,    -1,   190,   187,    -1,   191,   188,
      -1,   192,   189,    -1,   191,   200,    -1,   191,   192,   200,
      -1,   192,   200,    -1,   192,   191,   200,    -1,   200,    -1,
     190,   200,    -1,   190,   192,   200,    -1,   192,   200,    -1,
     192,   190,   200,    -1,   200,    -1,   190,   200,    -1,   190,
     191,   200,    -1,   191,   200,    -1,   191,   190,   200,    -1,
     200,    -1,   200,    94,   200,   193,    -1,   200,    94,     1,
      -1,   200,    95,   200,   194,    -1,   200,    95,     1,    -1,
     200,    96,   200,   194,    -1,   200,    96,     1,    -1,   195,
      -1,    97,    -1,   195,    -1,    98,   195,    -1,    98,     1,
      -1,    99,   195,    -1,    99,     1,    -1,    97,    -1,   100,
     198,    -1,   100,     1,    -1,   101,   196,    -1,   101,     1,
      -1,   102,   198,    -1,   102,     1,    -1,   103,   198,    -1,
     103,     1,    -1,    -1,   197,   200,   202,    -1,    -1,   199,
     200,   201,    -1,    66,    -1,    -1,   202,    -1,   203,    -1,
     294,    -1,   288,    -1,    76,    -1,     7,   250,   200,    -1,
       9,   250,   200,    -1,     8,   250,   200,    -1,    10,   250,
     200,    -1,    11,   250,   200,    -1,    -1,   147,   293,     1,
      -1,   147,   293,    -1,   163,   293,     1,    -1,   163,   293,
      -1,   139,   292,    -1,   140,   250,   130,    14,   130,   126,
      -1,   140,   250,   130,    14,   130,   126,    15,   130,   126,
      -1,   140,   250,   130,    14,   130,   126,    15,   130,     1,
      -1,   140,   250,   130,    14,   130,     1,    -1,   140,   129,
      -1,   140,   250,   130,     1,    -1,   133,    14,    -1,   133,
      15,    -1,   148,   250,    -1,   141,   295,    -1,   141,   295,
       1,    -1,   141,    -1,   216,    -1,   149,    -1,   142,   295,
      -1,   142,   295,     1,    -1,   142,    -1,   150,    -1,   150,
       1,    -1,   151,    50,   250,    -1,   151,    50,    -1,   151,
      51,   220,     1,    -1,   151,    51,   220,    -1,   151,    51,
      -1,   151,    51,    25,   250,    -1,   151,    51,     1,    -1,
     151,    52,    -1,   151,    52,   250,    -1,   151,     1,    -1,
      53,    -1,    54,    -1,   152,   292,    -1,   153,   223,   225,
     224,    -1,   153,   225,   224,    -1,   153,   223,     1,    -1,
     153,     1,    -1,    46,    -1,    46,   105,    -1,   105,    46,
      -1,   104,    -1,   104,   105,    -1,   105,   104,    -1,   105,
      -1,   224,   119,   285,    -1,   285,    -1,     4,    -1,    80,
      -1,    22,    -1,    79,    -1,    29,    -1,    23,    -1,    24,
     295,    -1,    25,   292,    26,    -1,    25,     1,    -1,   154,
      -1,   154,     1,    -1,   154,    28,     1,    -1,   154,    28,
     293,     1,    -1,   154,    28,   293,    -1,   155,   285,    -1,
     156,   292,    -1,   157,   292,    -1,   143,   292,    -1,   144,
     292,    -1,    45,   295,    -1,    45,   295,     1,    -1,    45,
      -1,   158,   129,   234,   237,   232,    -1,   158,   129,    45,
       1,    -1,   158,   129,   161,     1,    -1,   158,     1,    -1,
     158,   129,    14,    -1,   158,   129,   234,   161,     1,    -1,
     234,   235,    -1,   235,    -1,     1,    -1,   160,   250,   130,
      14,   130,   132,    -1,   160,   250,   130,    14,   130,   132,
      14,    -1,   160,   250,    -1,   160,     1,    -1,   133,   160,
      -1,   133,   161,    -1,   161,   130,   128,    -1,    -1,   159,
      25,   250,    -1,   159,   239,     1,    -1,   159,   239,    -1,
     159,   242,     1,    -1,   159,   243,     1,    -1,   159,   242,
     245,     1,    -1,   159,   242,   245,   240,     1,    -1,   159,
     242,   245,   240,    -1,   159,   242,   245,    -1,   159,   243,
     245,     1,    -1,   159,   243,   245,    -1,   241,    -1,    78,
      -1,     1,    -1,    82,    72,    -1,    82,    78,    -1,    82,
       1,    -1,    71,    -1,    72,    -1,    39,    -1,    40,    -1,
      96,    -1,    42,    -1,    70,    -1,    83,    -1,   244,    -1,
      43,    -1,    41,    -1,    87,    -1,   162,    25,   250,    -1,
     162,   292,    -1,   162,   247,     1,    -1,   162,   247,    -1,
      81,    -1,   249,   292,    -1,    47,    -1,    -1,   117,   251,
     278,    -1,    -1,   250,   110,   252,   250,    -1,    -1,   250,
     111,   253,   250,    -1,    -1,   250,   112,   254,   250,    -1,
     112,    -1,    -1,   250,   113,   255,   250,    -1,   113,    -1,
      -1,   250,    57,   256,   250,    -1,    57,    -1,    -1,   250,
     114,   257,   250,    -1,   114,    -1,    -1,   250,   106,   258,
     250,    -1,   106,    -1,    -1,   250,   107,   259,   250,    -1,
     107,    -1,    -1,   250,    68,   260,   250,    -1,    68,    -1,
      -1,   250,    67,   261,   250,    -1,    67,    -1,    -1,   250,
      66,   262,   250,    -1,    66,    -1,    -1,   250,    56,   263,
     250,    -1,    56,    -1,    55,   250,    -1,    55,    -1,    -1,
     250,   108,   264,   250,    -1,   108,    -1,    -1,   250,    58,
     265,   250,    -1,    58,    -1,    -1,   250,    60,   266,   250,
      -1,    60,    -1,    -1,   250,    59,   267,   250,    -1,    59,
      -1,    -1,   250,    61,   268,   250,    -1,    61,    -1,    -1,
     250,    62,   269,   250,    -1,    62,    -1,    -1,   250,    63,
     270,   250,    -1,    63,    -1,    -1,   250,    64,   271,   250,
      -1,    64,    -1,    -1,   250,    88,   272,   250,    -1,    88,
      -1,    -1,   250,    89,   273,   250,    -1,    89,    -1,    -1,
     250,    90,   274,   250,    -1,    90,    -1,    -1,   250,    91,
     275,   250,    -1,    91,    -1,    -1,   250,    92,   276,   250,
      -1,    92,    -1,    -1,   250,    93,   277,   250,    -1,    93,
      -1,   279,    -1,    71,    -1,    77,    -1,    84,    -1,    78,
      -1,   280,    -1,   110,   250,    -1,   111,   250,    -1,   110,
       1,    -1,   111,     1,    -1,   290,   118,    -1,   290,     1,
      -1,    48,    -1,    72,    -1,   284,   281,    -1,   283,   281,
      -1,    -1,   282,   278,    -1,    74,    -1,    73,    -1,   289,
     286,   285,    -1,   289,    -1,     1,    -1,   111,   287,    -1,
     110,   287,    -1,   287,    -1,   108,   287,    -1,   117,   279,
     118,    -1,   111,   117,   279,   118,    -1,   110,   117,   279,
     118,    -1,   108,   117,   279,   118,    -1,   288,    -1,    65,
      -1,    71,    -1,    78,    -1,    77,    -1,    84,    -1,    69,
     289,    -1,   279,   289,    -1,    -1,    -1,   292,   119,   291,
     290,    -1,   292,    -1,   250,    -1,    -1,   294,   293,    -1,
     294,    -1,   117,   294,   118,   293,    -1,   117,   294,   118,
      -1,    72,    -1,    72,    -1,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   159,   159,   159,   169,   172,   178,   180,   183,   185,
     188,   189,   192,   197,   198,   201,   202,   205,   206,   209,
     210,   213,   214,   217,   221,   224,   225,   226,   227,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   257,   261,   266,   270,   275,   279,
     283,   287,   291,   295,   299,   303,   307,   311,   315,   319,
     323,   327,   331,   335,   339,   343,   348,   352,   356,   360,
     364,   368,   368,   372,   375,   376,   377,   377,   379,   383,
     389,   395,   395,   400,   400,   406,   407,   408,   409,   410,
     416,   421,   422,   428,   429,   432,   440,   443,   449,   455,
     457,   458,   461,   482,   486,   490,   494,   497,   498,   499,
     501,   504,   505,   506,   509,   509,   515,   518,   519,   520,
     523,   524,   525,   526,   529,   533,   537,   541,   542,   543,
     544,   545,   548,   549,   550,   551,   552,   555,   556,   557,
     558,   559,   562,   563,   566,   567,   570,   571,   574,   575,
     580,   581,   583,   584,   585,   586,   591,   599,   600,   613,
     614,   618,   619,   623,   626,   626,   630,   630,   634,   635,
     638,   639,   642,   650,   658,   661,   662,   663,   666,   667,
     668,   671,   672,   676,   677,   681,   685,   693,   703,   705,
     707,   708,   711,   714,   717,   722,   724,   725,   728,   736,
     740,   742,   743,   746,   747,   750,   753,   754,   755,   758,
     760,   762,   763,   764,   767,   770,   771,   774,   777,   783,
     789,   790,   793,   795,   797,   799,   801,   803,   805,   809,
     811,   814,   815,   816,   817,   818,   819,   820,   822,   823,
     826,   827,   828,   829,   830,   834,   838,   842,   846,   850,
     854,   855,   856,   859,   864,   865,   867,   868,   869,   873,
     874,   875,   878,   884,   886,   887,   897,   898,   901,   905,
     911,   914,   915,   917,   918,   919,   920,   921,   927,   931,
     932,   938,   939,   940,   943,   944,   945,   948,   949,   952,
     955,   958,   959,   960,   961,   964,   965,   966,   967,   970,
     972,   974,   975,   979,   983,   999,  1006,  1006,  1039,  1039,
    1042,  1042,  1045,  1045,  1048,  1049,  1049,  1052,  1053,  1053,
    1056,  1057,  1057,  1060,  1061,  1061,  1064,  1065,  1065,  1068,
    1069,  1069,  1072,  1073,  1073,  1076,  1077,  1077,  1080,  1081,
    1081,  1084,  1085,  1087,  1088,  1088,  1092,  1093,  1093,  1097,
    1098,  1098,  1102,  1103,  1103,  1107,  1108,  1108,  1112,  1113,
    1113,  1117,  1118,  1118,  1121,  1122,  1122,  1125,  1126,  1126,
    1129,  1130,  1130,  1133,  1134,  1134,  1137,  1138,  1138,  1141,
    1142,  1142,  1145,  1146,  1146,  1149,  1150,  1151,  1153,  1157,
    1161,  1163,  1164,  1166,  1168,  1169,  1172,  1173,  1174,  1177,
    1181,  1183,  1187,  1187,  1194,  1197,  1200,  1201,  1202,  1205,
    1207,  1209,  1211,  1213,  1215,  1217,  1219,  1221,  1225,  1226,
    1235,  1236,  1240,  1246,  1247,  1248,  1258,  1258,  1338,  1382,
    1383,  1386,  1388,  1390,  1392,  1396,  1399,  1400
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ADDRESS", "ARG", "CALL", "DO", "TO",
  "BY", "FOR", "WHILE", "UNTIL", "EXIT", "IF", "THEN", "ELSE", "ITERATE",
  "INTERPRET", "LEAVE", "NOP", "NUMERIC", "PARSE", "EXTERNAL", "SOURCE",
  "VAR", "VALUE", "WITH", "PROCEDURE", "EXPOSE", "PULL", "PUSH", "QUEUE",
  "SAY", "RETURN", "SELECT", "WHEN", "DROP", "OTHERWISE", "SIGNAL", "ON",
  "OFF", "SYNTAX", "HALT", "NOVALUE", "TRACE", "END", "UPPER",
  "ASSIGNMENTVARIABLE", "STATSEP", "FOREVER", "DIGITS", "FORM", "FUZZ",
  "SCIENTIFIC", "ENGINEERING", "NOT", "CONCATENATE", "MODULUS", "GTE",
  "GT", "LTE", "LT", "DIFFERENT", "EQUALEQUAL", "NOTEQUALEQUAL", "OFFSET",
  "SPACE", "EXP", "XOR", "PLACEHOLDER", "NOTREADY", "CONSYMBOL",
  "SIMSYMBOL", "EXFUNCNAME", "INFUNCNAME", "LABEL", "DOVARIABLE",
  "HEXSTRING", "STRING", "VERSION", "LINEIN", "WHATEVER", "NAME",
  "FAILURE", "BINSTRING", "OPTIONS", "ENVIRONMENT", "LOSTDIGITS", "GTGT",
  "LTLT", "NOTGTGT", "NOTLTLT", "GTGTE", "LTLTE", "INPUT", "OUTPUT",
  "ERROR", "NORMAL", "APPEND", "REPLACE", "STREAM", "STEM", "LIFO", "FIFO",
  "LOWER", "CASELESS", "'|'", "'&'", "'='", "CCAT", "'+'", "'-'", "'*'",
  "'/'", "'%'", "UPLUS", "UMINUS", "'('", "')'", "','", "$accept", "start",
  "$@1", "prog", "stats", "xstats", "ystatement", "lonely_end", "nxstats",
  "ncl", "nlncl", "optLabels", "statement", "gruff", "mttstatement",
  "mtstatement", "nclstatement", "call", "do", "exit", "if", "iterate",
  "leave", "say", "return", "address", "arg", "drop", "interpret", "label",
  "nop", "numeric", "options", "parse", "proc", "pull", "push", "queue",
  "select", "signal", "when", "otherwise", "trace", "upper",
  "address_stat", "@2", "address_stat2", "$@3", "arg_stat", "call_stat",
  "$@4", "$@5", "call_name", "call_args", "expr_stat", "end_stat", "end",
  "do_stat", "repetitor", "nvir", "naddr_with", "@6", "addr_with",
  "connection", "inputstmts", "outputstmts", "errorstmts", "adeo", "adei",
  "adio", "inputstmt", "outputstmt", "errorstmt", "resourcei", "resourceo",
  "resources", "nsimsymb", "$@7", "nnvir", "$@8", "nspace", "addrAll",
  "addrSim", "addrString", "dovar", "tobyfor", "conditional", "drop_stat",
  "upper_stat", "exit_stat", "if_stat", "unexp_then", "unexp_else",
  "ipret_stat", "iterate_stat", "label_stat", "labelname", "leave_stat",
  "nop_stat", "numeric_stat", "form_expr", "options_stat", "parse_stat",
  "parse_flags", "templs", "parse_param", "proc_stat", "pull_stat",
  "push_stat", "queue_stat", "say_stat", "return_stat", "sel_end",
  "select_stat", "when_stats", "when_stat", "ex_when_stat",
  "otherwise_stat", "signal_stat", "signal_name", "namespec", "asymbol",
  "on", "off", "c_action", "s_action", "trace_stat", "whatever",
  "assignment", "ass_part", "expr", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "$@15", "$@16", "$@17", "$@18", "$@19", "$@20", "$@21", "$@22",
  "$@23", "$@24", "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31",
  "$@32", "$@33", "$@34", "$@35", "exprs_sub", "symbtree", "function",
  "func_args", "@36", "intfunc", "extfunc", "template", "solid", "offset",
  "string", "pv", "exprs", "$@37", "nexpr", "anyvars", "xsimsymb",
  "simsymb", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   124,    38,    61,   361,
      43,    45,    42,    47,    37,   362,   363,    40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   120,   122,   121,   123,   123,   124,   124,   125,   125,
     126,   126,   127,   128,   128,   129,   129,   130,   130,   131,
     131,   132,   132,   133,   134,   135,   135,   135,   135,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   165,   164,   166,   166,   166,   167,   166,   166,   166,
     168,   170,   169,   171,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   172,   172,   173,   173,   174,   175,   176,
     176,   176,   177,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   179,   181,   180,   180,   182,   182,   182,
     183,   183,   183,   183,   184,   185,   186,   187,   187,   187,
     187,   187,   188,   188,   188,   188,   188,   189,   189,   189,
     189,   189,   190,   190,   191,   191,   192,   192,   193,   193,
     194,   194,   194,   194,   194,   194,   195,   195,   195,   195,
     195,   195,   195,   195,   197,   196,   199,   198,   200,   200,
     201,   201,   202,   203,   204,   205,   205,   205,   206,   206,
     206,   207,   207,   208,   208,   209,   210,   210,   210,   210,
     210,   210,   211,   212,   213,   214,   214,   214,   215,   216,
     217,   217,   217,   218,   218,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   220,   220,   221,   222,   222,
     222,   222,   223,   223,   223,   223,   223,   223,   223,   224,
     224,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     226,   226,   226,   226,   226,   227,   228,   229,   230,   231,
     232,   232,   232,   233,   233,   233,   233,   233,   233,   234,
     234,   234,   235,   235,   235,   235,   236,   236,   237,   237,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   239,   239,   239,   240,   240,   240,   241,   241,   242,
     243,   244,   244,   244,   244,   245,   245,   245,   245,   246,
     246,   246,   246,   247,   248,   249,   251,   250,   252,   250,
     253,   250,   254,   250,   250,   255,   250,   250,   256,   250,
     250,   257,   250,   250,   258,   250,   250,   259,   250,   250,
     260,   250,   250,   261,   250,   250,   262,   250,   250,   263,
     250,   250,   250,   250,   264,   250,   250,   265,   250,   250,
     266,   250,   250,   267,   250,   250,   268,   250,   250,   269,
     250,   250,   270,   250,   250,   271,   250,   250,   272,   250,
     250,   273,   250,   250,   274,   250,   250,   275,   250,   250,
     276,   250,   250,   277,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   278,   278,   278,   279,
     280,   280,   282,   281,   283,   284,   285,   285,   285,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   287,   287,
     288,   288,   288,   289,   289,   289,   291,   290,   290,   292,
     292,   293,   293,   293,   293,   294,   295,   295
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     2,     1,     2,     2,
       1,     1,     3,     1,     1,     3,     2,     2,     1,     2,
       0,     1,     1,     0,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     1,     0,     0,     3,     3,     5,
       2,     0,     4,     0,     4,     3,     3,     4,     5,     4,
       3,     4,     3,     1,     1,     1,     2,     1,     1,     2,
       1,     3,     6,     7,     6,     5,     4,     2,     2,     2,
       0,     1,     1,     1,     0,     2,     0,     2,     3,     2,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     2,
       3,     1,     2,     3,     2,     3,     1,     2,     3,     2,
       3,     1,     4,     3,     4,     3,     4,     3,     1,     1,
       1,     2,     2,     2,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     3,     0,     3,     1,     0,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       0,     3,     2,     3,     2,     2,     6,     9,     9,     6,
       2,     4,     2,     2,     2,     2,     3,     1,     1,     1,
       2,     3,     1,     1,     2,     3,     2,     4,     3,     2,
       4,     3,     2,     3,     2,     1,     1,     2,     4,     3,
       3,     2,     1,     2,     2,     1,     2,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     2,
       1,     2,     3,     4,     3,     2,     2,     2,     2,     2,
       2,     3,     1,     5,     4,     4,     2,     3,     5,     2,
       1,     1,     6,     7,     2,     2,     2,     2,     3,     0,
       3,     3,     2,     3,     3,     4,     5,     4,     3,     4,
       3,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     3,     2,     1,     2,     1,     0,     3,     0,     4,
       0,     4,     0,     4,     1,     0,     4,     1,     0,     4,
       1,     0,     4,     1,     0,     4,     1,     0,     4,     1,
       0,     4,     1,     0,     4,     1,     0,     4,     1,     0,
       4,     1,     2,     1,     0,     4,     1,     0,     4,     1,
       0,     4,     1,     0,     4,     1,     0,     4,     1,     0,
       4,     1,     0,     4,     1,     0,     4,     1,     0,     4,
       1,     0,     4,     1,     0,     4,     1,     0,     4,     1,
       0,     4,     1,     0,     4,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       2,     2,     0,     2,     1,     1,     3,     1,     1,     2,
       2,     1,     2,     3,     4,     4,     4,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     0,     4,     1,     1,
       0,     2,     1,     4,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,    20,     1,     3,    23,    18,    23,     7,    11,
      10,    23,    21,    22,    20,    66,    17,   209,    19,   208,
       6,    62,    63,    54,    55,    56,    57,    58,    65,    59,
      67,    68,    70,    71,    72,    73,    74,    60,    61,    75,
      77,    64,    78,    76,    79,   108,    80,   315,   353,   351,
     330,   359,   365,   362,   368,   371,   374,   377,   348,   345,
     342,   397,   409,   415,   414,   398,   400,   399,    69,   380,
     383,   386,   389,   392,   395,   336,   339,   356,     0,     0,
     324,   327,   333,   316,     0,    24,     0,     0,   120,   440,
       0,     0,     0,   440,   440,    81,     0,     0,     0,     0,
       0,   440,     0,     0,     0,   440,   440,     0,     0,   276,
     277,   440,     0,    29,    31,    32,    30,     0,     0,    33,
      34,    52,    35,    26,    27,    28,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    48,    47,    49,
      50,    51,    53,   440,   107,   396,   401,   412,   412,    16,
      20,   352,   404,   402,   405,   403,   440,   202,   203,    25,
     104,   299,   300,   297,   298,   431,   430,   432,    91,   103,
       0,     0,    93,     0,   184,   190,     0,   179,   439,   195,
     200,    20,   447,   446,     0,     0,   258,   259,     0,   418,
     435,    90,   435,   240,   417,   445,     0,     0,   442,   204,
     214,   224,   216,     0,   222,   227,   231,   241,   243,   246,
       0,     0,   245,   232,   244,   242,   235,   238,     0,     0,
     251,     0,   255,   256,   257,   266,     0,   293,     0,   292,
       0,   291,     0,     0,     0,   313,     0,   310,     0,     0,
      12,   314,   349,   328,   357,   363,   360,   366,   369,   372,
     375,   346,   343,   340,   378,   381,   384,   387,   390,   393,
     334,   337,   354,   318,   320,   322,   325,   331,   411,   440,
     410,    15,   408,   317,     0,   438,   440,    95,   302,   303,
     304,   301,     0,    96,     0,   440,   118,   178,   117,     0,
       0,     0,     0,   346,   119,     0,   206,   211,    86,     0,
       0,   121,   122,   123,     0,    82,   440,    84,   433,     0,
     434,   428,   429,     0,     0,     0,     0,     0,   421,   427,
       0,   191,   441,   215,   221,     0,   225,   226,     0,   223,
     247,   249,     0,   233,   236,   234,   237,   230,     0,   229,
     252,     0,   271,   267,     0,     0,     0,   279,   270,   280,
     281,   283,   307,   306,   308,   305,     0,   284,     0,   309,
     311,   193,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   413,
     407,   406,   436,    92,   105,    97,     0,     0,   101,    94,
     179,   179,    23,   179,   201,    20,   124,   124,   133,     0,
     130,   131,   132,   179,   179,   179,   129,     0,   124,   239,
       0,   422,     0,   420,     0,   419,     0,   416,   444,   220,
     217,   248,   228,   253,   264,   275,    20,   265,     0,   269,
       0,   285,     0,   289,   350,   329,   358,   364,   361,   367,
     370,   373,   376,   347,   344,   341,   379,   382,   385,   388,
     391,   394,   335,   338,   355,   319,   321,   323,   326,   332,
     440,   106,   296,   294,   295,    98,   188,   189,    23,     0,
      23,    23,   116,     0,    87,     0,    83,   128,   134,   179,
     179,   141,   135,   179,   179,   146,   136,   179,   179,   151,
       0,     0,     0,   179,    88,     0,     0,     0,   423,   443,
       0,   268,    23,     0,   263,   286,   437,     8,    23,   112,
       9,     0,     0,     0,   115,   199,   196,   125,   179,   137,
     179,   139,   179,   142,   179,   144,   179,   147,   179,   149,
     153,     0,   155,     0,   157,     0,   124,   426,   425,   424,
      20,   278,     0,   179,   179,   179,   114,    20,   138,   140,
     143,   145,   148,   150,   159,     0,     0,     0,     0,   152,
     158,   165,     0,     0,   154,   160,   156,    89,    23,   261,
     185,   187,   186,   113,     0,   167,   166,   179,   169,   168,
     179,   171,   170,   173,   172,   162,   161,   164,   163,   272,
     198,   197,     0,     0,   273,   177,   180,   181,   183,   182,
     175
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     4,     7,   478,     8,     9,   479,    16,
       5,     6,    10,    11,    12,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    17,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   188,   305,   406,   114,   115,
     276,   285,   168,   393,   116,   117,   118,   119,   175,   306,
     484,   485,   307,   409,   410,   411,   412,   488,   492,   496,
     413,   414,   415,   569,   574,   575,   589,   590,   586,   587,
     288,   605,   606,   607,   176,   524,   291,   120,   121,   122,
     123,   124,   125,   126,   127,    18,    19,   128,   129,   130,
     328,   131,   132,   218,   191,   219,   133,   134,   135,   136,
     137,   138,   514,   139,   347,   348,    13,   440,   140,   230,
     397,   169,   170,   171,   355,   356,   141,   236,   142,   143,
     178,   156,   384,   385,   386,   387,   364,   388,   381,   382,
     374,   373,   372,   363,   383,   365,   367,   366,   368,   369,
     370,   371,   375,   376,   377,   378,   379,   380,   273,   145,
     146,   268,   269,   147,   148,   193,   317,   318,   172,   194,
     274,   470,   275,   197,   198,   184
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -497
static const yytype_int16 yypact[] =
{
    -497,    34,  -497,  -497,  -497,    63,    -4,    67,  -497,  -497,
    -497,  1130,  -497,  -497,  -497,  -497,     5,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  1579,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,   887,   951,
    -497,  -497,  -497,  -497,    58,  -497,    30,   402,  1687,  1814,
    1554,    48,    61,  1814,  1814,  -497,    39,   101,  1814,    47,
     405,  1814,   158,   206,    53,  1814,  1814,    51,   207,  -497,
    -497,  1429,   101,  -497,  -497,  -497,  -497,   185,    30,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  1814,  1939,  -497,  -497,  -497,  -497,    27,
    -497,  -497,  -497,  -497,  -497,  -497,  1621,  -497,  -497,     5,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
     170,   223,  -497,    40,  -497,   217,    33,  1998,  1939,  -497,
       5,  1939,  -497,  -497,    59,   193,  -497,  -497,   128,  -497,
      -8,    29,    -8,  -497,   236,  -497,    85,   208,   101,  1939,
    -497,  -497,  1814,    32,  1814,  -497,  -497,  -497,  -497,  -497,
      74,   823,  -497,    55,  -497,  -497,    86,   -14,   495,    39,
    -497,    28,  -497,  -497,  -497,  -497,  1530,  -497,  1814,  -497,
     241,  -497,   379,   395,  1814,  -497,   260,  -497,   281,   283,
       5,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  1621,
    -497,    27,  -497,  -497,    22,    79,  1814,  -497,  -497,  -497,
    -497,  -497,    45,  -497,   286,  1814,  -497,  -497,  -497,  1814,
    1814,    30,  1814,   341,  -497,   171,  -497,  -497,  -497,  1814,
     154,  -497,  -497,  -497,  1814,  -497,  1814,  -497,  -497,    39,
    -497,  -497,  -497,   267,   336,   337,   129,    39,  -497,  -497,
      92,  -497,  -497,  1939,  -497,  1814,  -497,  -497,   291,  1939,
    -497,  -497,   226,  -497,  -497,  -497,  -497,  -497,    39,    29,
    -497,   382,  -497,  -497,   254,  1015,   263,   142,  -497,  1939,
    -497,  -497,  -497,  -497,  -497,  -497,    46,  -497,   384,  1939,
    -497,  -497,  -497,  1814,  1814,  1814,  1814,  1814,  1814,  1814,
    1814,  1814,  1814,  1814,  1814,  1814,  1814,  1814,  1814,  1814,
    1814,  1814,  1814,  1814,  1814,  1814,  1814,  1814,  1814,  -497,
    -497,  -497,  -497,  -497,   159,  -497,    97,   392,  -497,  -497,
    1998,  1998,     5,  1998,  -497,  -497,   261,  1754,  -497,   433,
    -497,  -497,  -497,   252,   252,   252,   -12,  1876,   261,  -497,
     129,  -497,   129,  -497,   129,  -497,   212,  -497,   101,  1939,
    -497,  -497,    29,  -497,  -497,  -497,  1493,  -497,   525,  -497,
     300,  -497,   442,  -497,   509,   315,   624,   624,   624,   624,
     624,   624,   624,   509,  -497,  2036,   624,   624,   624,   624,
     624,   624,  2036,  2074,   624,   229,   229,   315,   315,   315,
    1814,  -497,  -497,  -497,  -497,  -497,  -497,  -497,   359,   370,
    -497,  1245,   439,   640,  -497,   393,  -497,  -497,  -497,   252,
     252,   -53,  -497,   252,   252,   164,  -497,   252,   252,   149,
     202,   173,   225,   252,  -497,   308,   310,   321,  -497,  -497,
     430,  -497,  -497,   187,  -497,  -497,  -497,  -497,  1360,  -497,
    -497,  1814,  1814,  1814,   439,  -497,   446,  -497,   252,   371,
     252,   374,   252,   371,   252,   383,   252,   374,   252,   383,
    -497,   504,  -497,   527,  -497,   527,   261,  -497,  -497,  -497,
    -497,  -497,   444,  1998,  1998,  1998,   439,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,   112,   222,   339,   417,  -497,
    -497,  -497,    65,   114,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,   755,  -497,  -497,   252,  -497,  -497,
     252,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,   471,
    -497,  -497,   426,    85,  -497,  -497,  -497,  -497,  -497,  -497,
    -497
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -497,  -497,  -497,  -497,  -497,  -497,    -7,  -497,   -24,   -80,
    -169,     1,  -386,   -10,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -205,  -204,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,   215,  -497,  -497,    14,  -497,  -497,  -497,
    -394,  -497,    12,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -359,  -378,  -355,  -497,   -40,  -496,  -497,  -497,  -158,  -497,
    -103,  -497,   -97,  -497,  -497,  -459,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -194,   289,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,   168,  -497,  -497,  -497,  -497,
     155,   408,   413,   414,   293,   290,  -497,  -497,  -497,  -497,
      -9,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
    -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,   256,   -87,
    -497,   385,  -497,  -497,  -497,   -96,  -497,   145,  -190,   125,
    -265,  -497,   -75,  -107,  -189,   -89
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -441
static const yytype_int16 yytable[] =
{
      20,    84,   144,   185,   319,   238,   159,   320,   222,   192,
     180,   394,   295,   486,   179,   149,   480,   192,   186,   187,
     394,   345,   346,   390,   504,   339,   205,   226,   239,   340,
     223,   224,   335,   324,     3,   489,   237,   498,   240,   151,
     189,   286,   501,   502,    14,   570,   395,   441,   200,   182,
    -179,  -179,   225,   150,   189,   493,   497,   325,   490,   494,
     296,   190,   182,    -5,    62,   556,   595,    -4,   241,   153,
     155,    15,   157,   158,   294,   182,   596,   598,    14,   177,
    -219,   181,   500,   501,   502,   326,   327,  -435,  -179,   199,
     336,   322,   517,  -100,  -288,  -213,  -207,   583,   472,    14,
     195,  -435,    15,   192,  -435,   192,   287,  -205,   190,  -212,
    -435,    62,   530,   585,   341,   597,  -435,  -435,  -435,   536,
     183,   330,   190,  -435,  -435,    62,   480,   396,   396,   298,
    -435,  -435,   192,   183,   528,   534,   332,  -435,   532,   538,
     391,   292,   345,   438,   432,   196,   183,  -435,   309,  -435,
    -435,   271,   577,   299,   300,   408,  -435,   195,  -435,   206,
     333,  -435,   207,  -435,  -435,   565,   566,   567,   568,   473,
    -435,   277,   404,   195,   542,   474,   -85,    40,  -176,    42,
     208,   209,   210,   211,  -176,   405,   182,   212,   182,  -176,
    -176,   334,   599,   323,   297,   329,  -176,   416,   392,   301,
     302,    62,  -179,   540,   213,   516,   303,   220,   227,   321,
     428,   402,   278,   419,   565,   566,   567,   568,   196,   349,
     287,   427,   192,   588,   283,   359,   544,   289,   290,   426,
     192,   418,   228,  -110,   221,  -262,   483,   214,   215,   287,
     279,  -210,   350,   500,   501,   304,   161,   162,  -179,  -179,
    -179,   192,   431,   280,  -250,   434,  -192,   183,   500,   183,
     502,   360,   216,   217,   437,   278,   281,   510,   287,   512,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,   471,   163,   164,
     400,   401,   361,   403,   362,   229,   243,   398,  -174,  -282,
     407,   287,   430,   279,  -174,   417,   252,   476,   477,  -179,
     482,   311,  -179,  -179,  -179,  -179,   280,   312,  -312,  -126,
     491,   495,   499,   165,   166,   308,   429,   310,   287,   281,
     167,   509,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -194,
     508,  -109,   311,   505,  -102,   506,   436,   507,   312,  -218,
     591,   265,   266,   267,   313,   513,   314,   315,  -178,  -178,
    -178,  -178,  -178,   316,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     351,   578,   252,   433,   420,   443,   529,   531,   584,  -178,
     533,   535,   481,   475,   537,   539,   357,   541,   543,   545,
     546,   311,   311,   160,   -13,  -176,   201,   312,   312,   592,
     594,  -176,   608,   609,   609,    45,  -176,  -176,   593,   300,
     352,   278,   353,  -176,   552,   558,   547,   559,   548,   560,
    -254,   561,  -290,   562,   487,   563,   352,   278,   353,   549,
     -99,   161,   162,   515,   550,   579,   521,   522,   523,   279,
     580,   581,   582,   422,   424,   202,   203,   204,   421,   423,
     425,   557,   280,   282,   284,   279,   354,   502,   518,   501,
     520,    84,   144,   163,   164,   281,   526,   500,   280,   165,
     166,  -127,   354,  -176,   602,   604,   167,   603,   551,  -176,
    -287,   281,  -260,   519,  -176,  -176,   337,   527,   195,   207,
     399,  -176,   481,   165,   166,   576,   610,   338,    84,   144,
     167,   442,   553,   554,   555,   439,   231,   208,   209,   210,
     211,   232,   233,   358,   212,   389,   511,     0,   -20,   -20,
     -20,   -20,     0,   270,     0,     0,     0,   -20,   -20,   -20,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,     0,     0,     0,
       0,     0,   -20,     0,   -20,   -20,   -20,   -20,   -20,   -20,
     -20,   -20,   -20,   -20,     0,     0,   243,     0,   518,   -20,
     -20,   -20,   -20,   -20,   214,   215,   252,   601,     0,     0,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
       0,   -20,   -20,   -20,     0,     0,   -20,   -20,   -20,   -20,
     -20,   564,   -20,   -20,   565,   566,   567,   568,     0,   -20,
     -20,     0,     0,   -20,   -20,   -20,   -20,   -20,   -20,   263,
     264,   265,   266,   267,   571,   572,   573,   565,   566,   567,
     568,   -20,   -20,   -20,     0,   -20,   -20,   -20,   -20,   -20,
       0,   525,   -20,   -23,   -23,   -23,   -23,     0,     0,     0,
       0,     0,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,     0,     0,     0,     0,     0,   -23,     0,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,     0,
     242,   243,     0,     0,   -23,   -23,   -23,   -23,     0,     0,
     251,   252,     0,     0,     0,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,     0,   -23,   -23,   -23,     0,
       0,   -23,   -23,   -23,   -23,     0,     0,   -23,   -23,     0,
       0,     0,     0,     0,   -23,   -23,     0,     0,   -23,   -23,
     -23,   -23,   -23,   -23,   263,   264,   265,   266,   267,     0,
       0,     0,     0,     0,     0,     0,   -23,   -23,   -23,     0,
     -23,   -23,   -23,   -23,   -23,     0,   600,   -23,   -23,   -23,
     -23,   -23,     0,     0,     0,     0,     0,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,     0,     0,     0,
       0,     0,   -23,     0,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,     0,     0,     0,     0,     0,   -23,
     -23,   -23,   -23,     0,     0,     0,     0,     0,     0,     0,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
       0,   -23,   -23,   -23,   331,     0,   -23,   -23,   -23,   -23,
       0,     0,   -23,   -23,     0,     0,     0,     0,     0,   -23,
     -23,     0,     0,   -23,   -23,   -23,   -23,   -23,   -23,  -440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -23,   -23,   -23,     0,   -23,   -23,   -23,   -23,   -23,
       0,     0,   -23,     0,     0,     0,     0,     0,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,   152,    58,
      59,    60,     0,     0,    61,    62,    63,    64,     0,     0,
      65,    66,     0,     0,     0,     0,     0,    67,     0,     0,
       0,    69,    70,    71,    72,    73,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,     0,    78,    79,    80,    81,    82,     0,     0,
      83,     0,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,   154,    58,    59,    60,     0,     0,    61,    62,
      63,    64,     0,     0,    65,    66,     0,     0,     0,     0,
       0,    67,     0,     0,     0,    69,    70,    71,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,   435,    58,    59,    60,
       0,     0,    61,    62,    63,    64,     0,     0,    65,    66,
       0,     0,     0,     0,     0,    67,     0,     0,     0,    69,
      70,    71,    72,    73,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
       0,    58,    59,    60,     0,     0,    61,    62,    63,    64,
       0,     0,    65,    66,     0,     0,     0,     0,     0,    67,
       0,     0,     0,    69,    70,    71,    72,    73,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,    21,    22,    23,    24,     0,     0,     0,
       0,     0,    25,    26,     0,     0,    27,    28,    29,    30,
      31,    32,     0,     0,     0,     0,     0,    33,     0,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     0,
       0,     0,     0,     0,    44,    45,    46,    47,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,    58,    59,    60,     0,
       0,    61,    62,    63,    64,     0,     0,    65,    66,     0,
       0,     0,     0,     0,    67,    68,     0,     0,    69,    70,
      71,    72,    73,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,    21,    22,
      23,    24,     0,     0,     0,     0,     0,    25,    26,     0,
       0,    27,    28,    29,    30,    31,    32,     0,     0,     0,
       0,     0,    33,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     0,     0,     0,     0,     0,    44,
     -14,    46,    47,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
       0,    58,    59,    60,     0,     0,    61,    62,    63,    64,
       0,     0,    65,    66,     0,     0,     0,     0,     0,    67,
      68,     0,     0,    69,    70,    71,    72,    73,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,    21,    22,    23,    24,     0,     0,     0,
       0,     0,    25,    26,     0,     0,    27,    28,    29,    30,
      31,    32,     0,     0,     0,     0,     0,    33,     0,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     0,
       0,     0,     0,     0,    44,     0,    46,    47,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,    58,    59,    60,     0,
       0,    61,    62,    63,    64,     0,     0,    65,    66,     0,
       0,     0,     0,     0,    67,    68,     0,     0,    69,    70,
      71,    72,    73,    74,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,     0,
       0,     0,     0,     0,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     0,    58,    59,    60,     0,     0,
      61,    62,    63,    64,     0,     0,    65,    66,     0,     0,
     235,     0,     0,    67,     0,     0,     0,    69,    70,    71,
      72,    73,    74,     0,     0,     0,     0,     0,  -274,     0,
    -274,   342,     0,     0,     0,    75,    76,    77,  -274,    78,
      79,    80,    81,    82,   343,     0,    83,     0,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,   150,     0,
       0,   254,   255,   256,   257,   258,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,    14,   263,   264,   265,   266,   267,     0,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
      58,    59,    60,     0,     0,    61,    62,    63,    64,     0,
       0,    65,    66,     0,    48,     0,     0,     0,    67,     0,
       0,     0,    69,    70,    71,    72,    73,    74,     0,     0,
      61,    62,    63,    64,     0,     0,    65,    66,     0,     0,
      75,    76,    77,    67,    78,    79,    80,    81,    82,   272,
       0,    83,     0,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,     0,    58,    59,    60,
       0,     0,    61,    62,    63,    64,    83,     0,    65,    66,
       0,     0,     0,     0,     0,    67,     0,     0,     0,    69,
      70,    71,    72,    73,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
       0,    78,    79,    80,    81,    82,   173,     0,    83,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,    58,    59,    60,     0,     0,    61,    62,
      63,    64,     0,   174,    65,    66,     0,     0,     0,     0,
       0,    67,     0,     0,     0,    69,    70,    71,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,     0,    78,    79,    80,
      81,    82,  -126,     0,    83,     0,     0,     0,     0,     0,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,   253,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   254,   255,   256,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
      58,    59,    60,     0,     0,    61,    62,    63,    64,     0,
       0,    65,    66,     0,     0,     0,     0,     0,    67,     0,
       0,     0,    69,    70,    71,    72,    73,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,    83,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   253,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   254,   255,   256,   257,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,     0,     0,     0,   503,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   254,   255,   256,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   293,   252,   253,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   254,   255,   256,   257,
     258,   259,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   260,   261,   262,     0,   263,   264,
     265,   266,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   254,   255,   256,   257,   258,   259,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,   261,   262,     0,   263,   264,   265,   266,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   254,   255,   256,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,   263,   264,   265,   266,   267
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-497))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,    11,    11,    92,   194,   112,    86,   196,   104,    96,
      90,   276,   181,   407,    89,    14,   402,   104,    93,    94,
     285,   226,   226,     1,   418,   219,   101,   107,   117,     1,
     105,   106,    46,     1,     0,   413,   111,   415,   118,    48,
       1,     1,    95,    96,    48,   541,     1,     1,     1,     1,
      10,    11,     1,    48,     1,   414,   415,    25,   413,   414,
       1,    69,     1,     0,    72,   524,     1,     0,   143,    78,
      79,    75,    14,    15,   177,     1,   572,   573,    48,    88,
      48,    90,    94,    95,    96,    53,    54,    48,    48,    98,
     104,   198,   478,    48,    48,    48,    48,   556,     1,    48,
      72,    48,    75,   190,    65,   192,    66,    48,    69,    48,
      71,    72,   490,     1,   221,     1,    77,    78,    65,   497,
      72,   210,    69,    84,    71,    72,   512,    82,    82,     1,
      77,    78,   219,    72,   489,   494,   211,    84,   493,   498,
     118,   108,   347,   347,   338,   117,    72,   108,   119,   110,
     111,   150,   546,    25,    26,     1,   117,    72,   119,     1,
     105,   108,     4,   110,   111,   100,   101,   102,   103,    72,
     117,     1,     1,    72,     1,    78,    48,    35,    66,    37,
      22,    23,    24,    25,    72,    14,     1,    29,     1,    77,
      78,   105,   578,   202,     1,   204,    84,   300,   119,    71,
      72,    72,    48,     1,    46,   470,    78,     1,     1,     1,
     118,   291,    42,   309,   100,   101,   102,   103,   117,   228,
      66,   317,   309,     1,     1,   234,     1,    10,    11,   316,
     317,   306,    25,    48,    28,    48,   405,    79,    80,    66,
      70,    48,     1,    94,    95,   117,    39,    40,    94,    95,
      96,   338,    26,    83,    48,     1,    48,    72,    94,    72,
      96,     1,   104,   105,     1,    42,    96,   436,    66,   438,
      97,    98,    99,   100,   101,   102,   103,   118,    71,    72,
     289,   290,     1,   292,     1,    78,    57,     1,    66,    48,
     299,    66,     1,    70,    72,   304,    67,   400,   401,    97,
     403,    65,   100,   101,   102,   103,    83,    71,    48,    48,
     413,   414,   415,    77,    78,   190,   325,   192,    66,    96,
      84,   428,    97,    98,    99,   100,   101,   102,   103,    48,
     118,    48,    65,   420,    48,   422,   345,   424,    71,    48,
       1,   112,   113,   114,   108,    45,   110,   111,     7,     8,
       9,    10,    11,   117,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
       1,   550,    67,     1,   117,     1,   489,   490,   557,    48,
     493,   494,   402,     1,   497,   498,     1,   500,   501,   502,
     503,    65,    65,     1,    45,    66,     1,    71,    71,   567,
     568,    72,   602,   602,   603,    45,    77,    78,     1,    26,
      41,    42,    43,    84,   513,   528,   118,   530,   118,   532,
      48,   534,    48,   536,     1,   538,    41,    42,    43,   118,
      48,    39,    40,     1,    14,     1,     7,     8,     9,    70,
     553,   554,   555,   117,   117,    50,    51,    52,   313,   314,
     315,    15,    83,   170,   171,    70,    87,    96,   478,    95,
     480,   481,   481,    71,    72,    96,   483,    94,    83,    77,
      78,    48,    87,    66,   587,    14,    84,   590,   512,    72,
      48,    96,    48,   479,    77,    78,     1,   485,    72,     4,
     285,    84,   512,    77,    78,   545,   603,   218,   518,   518,
      84,   356,   521,   522,   523,   347,   108,    22,    23,    24,
      25,   108,   108,   233,    29,   269,     1,    -1,     3,     4,
       5,     6,    -1,   148,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    57,    -1,   578,    44,
      45,    46,    47,    48,    79,    80,    67,   584,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    -1,    71,    72,    73,    74,
      75,    97,    77,    78,   100,   101,   102,   103,    -1,    84,
      85,    -1,    -1,    88,    89,    90,    91,    92,    93,   110,
     111,   112,   113,   114,    97,    98,    99,   100,   101,   102,
     103,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,     1,   117,     3,     4,     5,     6,    -1,    -1,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      56,    57,    -1,    -1,    44,    45,    46,    47,    -1,    -1,
      66,    67,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    88,    89,
      90,    91,    92,    93,   110,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,     1,   117,     3,     4,
       5,     6,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    -1,    -1,    -1,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,     1,    -1,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    88,    89,    90,    91,    92,    93,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     1,    66,
      67,    68,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      77,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,     1,    66,    67,    68,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,     1,    66,    67,    68,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,     3,     4,     5,     6,    -1,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    88,    89,
      90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,     3,     4,
       5,     6,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    -1,    -1,    -1,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    88,    89,    90,    91,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,     3,     4,     5,     6,    -1,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    88,    89,
      90,    91,    92,    93,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    -1,    66,    67,    68,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    77,    78,    -1,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,     1,    -1,    -1,    -1,   106,   107,   108,    45,   110,
     111,   112,   113,   114,    14,    -1,   117,    -1,    -1,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    -1,    -1,    35,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    88,    89,    90,    91,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,    48,   110,   111,   112,   113,   114,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    77,    78,    -1,    55,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    77,    78,    -1,    -1,
     106,   107,   108,    84,   110,   111,   112,   113,   114,    48,
      -1,   117,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    -1,    71,    72,    73,    74,   117,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    49,    -1,   117,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    -1,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    48,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    -1,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,   110,   111,   112,   113,   114
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   121,   122,     0,   123,   130,   131,   124,   126,   127,
     132,   133,   134,   236,    48,    75,   129,   149,   215,   216,
     126,     3,     4,     5,     6,    12,    13,    16,    17,    18,
      19,    20,    21,    27,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    44,    45,    46,    47,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    66,    67,
      68,    71,    72,    73,    74,    77,    78,    84,    85,    88,
      89,    90,    91,    92,    93,   106,   107,   108,   110,   111,
     112,   113,   114,   117,   133,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   168,   169,   174,   175,   176,   177,
     207,   208,   209,   210,   211,   212,   213,   214,   217,   218,
     219,   221,   222,   226,   227,   228,   229,   230,   231,   233,
     238,   246,   248,   249,   250,   279,   280,   283,   284,   131,
      48,   250,     1,   250,     1,   250,   251,    14,    15,   129,
       1,    39,    40,    71,    72,    77,    78,    84,   172,   241,
     242,   243,   288,    49,    76,   178,   204,   250,   250,   292,
     129,   250,     1,    72,   295,   295,   292,   292,   165,     1,
      69,   224,   279,   285,   289,    72,   117,   293,   294,   250,
       1,     1,    50,    51,    52,   292,     1,     4,    22,    23,
      24,    25,    29,    46,    79,    80,   104,   105,   223,   225,
       1,    28,   285,   292,   292,     1,   129,     1,    25,    78,
     239,   241,   242,   243,    25,    81,   247,   292,   293,   295,
     129,   292,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    66,    67,    68,    88,    89,    90,    91,    92,    93,
     106,   107,   108,   110,   111,   112,   113,   114,   281,   282,
     281,   131,    48,   278,   290,   292,   170,     1,    42,    70,
      83,    96,   244,     1,   244,   171,     1,    66,   200,    10,
      11,   206,   108,    66,   200,   130,     1,     1,     1,    25,
      26,    71,    72,    78,   117,   166,   179,   182,   289,   119,
     289,    65,    71,   108,   110,   111,   117,   286,   287,   288,
     294,     1,   293,   250,     1,    25,    53,    54,   220,   250,
     295,     1,   292,   105,   105,    46,   104,     1,   225,   224,
       1,   293,     1,    14,    45,   160,   161,   234,   235,   250,
       1,     1,    41,    43,    87,   244,   245,     1,   245,   250,
       1,     1,     1,   263,   256,   265,   267,   266,   268,   269,
     270,   271,   262,   261,   260,   272,   273,   274,   275,   276,
     277,   258,   259,   264,   252,   253,   254,   255,   257,   278,
       1,   118,   119,   173,   290,     1,    82,   240,     1,   173,
     250,   250,   129,   250,     1,    14,   167,   250,     1,   183,
     184,   185,   186,   190,   191,   192,   200,   250,   292,   285,
     117,   287,   117,   287,   117,   287,   279,   285,   118,   250,
       1,    26,   224,     1,     1,     1,   250,     1,   161,   235,
     237,     1,   240,     1,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     291,   118,     1,    72,    78,     1,   200,   200,   125,   128,
     132,   133,   200,   130,   180,   181,   180,     1,   187,   191,
     192,   200,   188,   190,   192,   200,   189,   190,   191,   200,
      94,    95,    96,   118,   180,   279,   279,   279,   118,   293,
     130,     1,   130,    45,   232,     1,   290,   132,   133,   176,
     133,     7,     8,     9,   205,     1,   126,   182,   192,   200,
     191,   200,   192,   200,   190,   200,   191,   200,   190,   200,
       1,   200,     1,   200,     1,   200,   200,   118,   118,   118,
      14,   128,   295,   250,   250,   250,   205,    15,   200,   200,
     200,   200,   200,   200,    97,   100,   101,   102,   103,   193,
     195,    97,    98,    99,   194,   195,   194,   180,   130,     1,
     200,   200,   200,   205,   130,     1,   198,   199,     1,   196,
     197,     1,   198,     1,   198,     1,   195,     1,   195,   132,
       1,   126,   200,   200,    14,   201,   202,   203,   288,   294,
     202
};

#define yyerrok      (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)
#define YYEMPTY      (-2)
#define YYEOF     0

#define YYACCEPT  goto yyacceptlab
#define YYABORT      goto yyabortlab
#define YYERROR      goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL    goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)               \
do                      \
  if (yychar == YYEMPTY && yylen == 1)          \
    {                      \
      yychar = (Token);                \
      yylval = (Value);                \
      YYPOPSTACK (1);                  \
      goto yybackup;                \
    }                      \
  else                        \
    {                      \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                   \
    }                      \
while (YYID (0))


#define YYTERROR  1
#define YYERRCODE 256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
    do                           \
      if (YYID (N))                                                    \
   {                       \
     (Current).first_line   = YYRHSLOC (Rhs, 1).first_line; \
     (Current).first_column = YYRHSLOC (Rhs, 1).first_column;  \
     (Current).last_line    = YYRHSLOC (Rhs, N).last_line;     \
     (Current).last_column  = YYRHSLOC (Rhs, N).last_column;   \
   }                       \
      else                       \
   {                       \
     (Current).first_line   = (Current).last_line   =    \
       YYRHSLOC (Rhs, 0).last_line;          \
     (Current).first_column = (Current).last_column =    \
       YYRHSLOC (Rhs, 0).last_column;           \
   }                       \
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)         \
do {                 \
  if (yydebug)             \
    YYFPRINTF Args;           \
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)           \
do {                            \
  if (yydebug)                        \
    {                           \
      YYFPRINTF (stderr, "%s ", Title);                 \
      yy_symbol_print (stderr,                    \
        Type, Value); \
      YYFPRINTF (stderr, "\n");                   \
    }                           \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
   break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)            \
do {                       \
  if (yydebug)                   \
    yy_stack_print ((Bottom), (Top));           \
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
        yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
             &(yyvsp[(yyi + 1) - (yynrhs)])
                         );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)      \
do {              \
  if (yydebug)          \
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef  YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
   switch (*++yyp)
     {
     case '\'':
     case ',':
       goto do_not_strip_quotes;

     case '\\':
       if (*++yyp != '\\')
         goto do_not_strip_quotes;
       /* Fall through.  */
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
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
      if (!yypact_value_is_default (yyn))
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
   break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
   /* Give user a chance to reallocate the stack.  Use copies of
      these so that the &'s don't force the real ones into
      memory.  */
   YYSTYPE *yyvs1 = yyvs;
   yytype_int16 *yyss1 = yyss;

   /* Each stack pointer address is followed by the size of the
      data in use in that stack, in bytes.  This used to be a
      conditional around just the two extra args, but that might
      be undefined if yyoverflow is a macro.  */
   yyoverflow (YY_("memory exhausted"),
          &yyss1, yysize * sizeof (*yyssp),
          &yyvs1, yysize * sizeof (*yyvsp),
          &yystacksize);

   yyss = yyss1;
   yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
   goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
   yystacksize = YYMAXDEPTH;

      {
   yytype_int16 *yyss1 = yyss;
   union yyalloc *yyptr =
     (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
   if (! yyptr)
     goto yyexhaustedlab;
   YYSTACK_RELOCATE (yyss_alloc, yyss);
   YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
   if (yyss1 != yyssa)
     YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
        (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
   YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 159 "./yaccsrc.y"
    { level = 0;
                                         if ( get_options_flag( parser_data.TSD->currlevel, EXT_CALLS_AS_FUNCS )
                                           && !get_options_flag( parser_data.TSD->currlevel, EXT_STRICT_ANSI ) )
                                            start_parendepth = 1;
                                         else
                                            start_parendepth = 0;
                                         parendepth = 0; }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 169 "./yaccsrc.y"
    { (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) );
                                         (yyval)->o.last = NULL;
                                         EndProg( (yyval) ) ; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 172 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]);
                                         if ( (yyval) != NULL )
                                            (yyval)->o.last = NULL;
                                         EndProg( (yyval) ); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 178 "./yaccsrc.y"
    { /* fixes bug 579711 */
                                         (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 180 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 183 "./yaccsrc.y"
    { /* fixes bug 579711 */
                                         (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) ); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 185 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 188 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 189 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_END, 1 ); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 197 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 198 "./yaccsrc.y"
    { (yyval) = NULL; }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 201 "./yaccsrc.y"
    { (yyval) = optgluelast( (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 202 "./yaccsrc.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 205 "./yaccsrc.y"
    { (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) ); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 206 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 209 "./yaccsrc.y"
    { (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) ); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 210 "./yaccsrc.y"
    { (yyval) = NULL; }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 217 "./yaccsrc.y"
    { tmpchr=parser_data.tstart;
                                         tmplno=parser_data.tline; }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 221 "./yaccsrc.y"
    { (yyval)=(yyvsp[(2) - (2)]); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 224 "./yaccsrc.y"
    { (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) ); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 257 "./yaccsrc.y"
    { (yyval) = makenode(X_CALL,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 261 "./yaccsrc.y"
    { (yyval) = makenode(X_DO,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         level++; }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 266 "./yaccsrc.y"
    { (yyval) = makenode(X_EXIT,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 270 "./yaccsrc.y"
    { (yyval) = makenode(X_IF,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         level++; }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 275 "./yaccsrc.y"
    { (yyval) = makenode(X_ITERATE,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 279 "./yaccsrc.y"
    { (yyval) = makenode(X_LEAVE,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 283 "./yaccsrc.y"
    { (yyval) = makenode(X_SAY,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 287 "./yaccsrc.y"
    { (yyval) = makenode(X_RETURN,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 291 "./yaccsrc.y"
    { (yyval) = makenode(X_ADDR_N,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 295 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_ARG,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 299 "./yaccsrc.y"
    { (yyval) = makenode(X_DROP,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 303 "./yaccsrc.y"
    { (yyval) = makenode(X_IPRET,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 307 "./yaccsrc.y"
    { (yyval) = makenode(X_LABEL,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 311 "./yaccsrc.y"
    { (yyval) = makenode(X_NULL,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 315 "./yaccsrc.y"
    { (yyval) = makenode(0,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 319 "./yaccsrc.y"
    { (yyval) = makenode(X_OPTIONS,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 323 "./yaccsrc.y"
    { (yyval) = makenode(0,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 327 "./yaccsrc.y"
    { (yyval) = makenode(X_PROC,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 331 "./yaccsrc.y"
    { (yyval) = makenode(X_PULL,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 335 "./yaccsrc.y"
    { (yyval) = makenode(X_PUSH,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 339 "./yaccsrc.y"
    { (yyval) = makenode(X_QUEUE,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 343 "./yaccsrc.y"
    { (yyval) = makenode(X_SELECT,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         level++; }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 348 "./yaccsrc.y"
    { (yyval) = makenode(X_SIG_LAB,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 352 "./yaccsrc.y"
    { (yyval) = makenode(X_WHEN,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 356 "./yaccsrc.y"
    { (yyval) = makenode(X_OTHERWISE,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 360 "./yaccsrc.y"
    { (yyval) = makenode(X_TRACE,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 364 "./yaccsrc.y"
    { (yyval) = makenode(X_UPPER_VAR,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 368 "./yaccsrc.y"
    { (yyval) = current = (yyvsp[(1) - (1)]) ; }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 372 "./yaccsrc.y"
    { current->type = X_ADDR_V ;
                                         current->p[0] = (yyvsp[(2) - (3)]) ;
                                         current->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 375 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 1, __reginatext ) ;}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 376 "./yaccsrc.y"
    { current->type = X_ADDR_S ; }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 377 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 1, __reginatext ) ;}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 379 "./yaccsrc.y"
    { current->name = (streng *)(yyvsp[(1) - (3)]) ;
                                         current->type = X_ADDR_N ;
                                         current->p[0] = (yyvsp[(2) - (3)]) ;
                                         current->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 383 "./yaccsrc.y"
    { current->type = X_ADDR_V ;
                                         current->p[0] = (yyvsp[(2) - (5)]) ;
                                         current->p[1] = (yyvsp[(5) - (5)]) ;
                                         current->u.nonansi = 1 ; }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 389 "./yaccsrc.y"
    { (yyval) = makenode( X_PARSE, 2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) );
                                         (yyval)->u.parseflags = PARSE_UPPER;
                                         (yyval)->lineno = (yyvsp[(1) - (2)])->lineno;
                                         (yyval)->charnr = (yyvsp[(1) - (2)])->charnr; }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 395 "./yaccsrc.y"
    { parendepth = start_parendepth; }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 396 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (4)]);
                                         (yyval)->p[0] = (yyvsp[(4) - (4)]);
                                         (yyval)->name = (streng *) (yyvsp[(2) - (4)]);
                                         parendepth = 0; }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 400 "./yaccsrc.y"
    { parendepth = start_parendepth; }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 401 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (4)]);
                                         (yyval)->type = X_EX_FUNC;
                                         (yyval)->p[0] = (yyvsp[(4) - (4)]);
                                         (yyval)->name = (streng *) (yyvsp[(2) - (4)]);
                                         parendepth = 0; }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 406 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 1, "ERROR FAILURE HALT NOTREADY", __reginatext ) ;}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 407 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 2, "ERROR FAILURE HALT NOTREADY", __reginatext ) ;}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 408 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 409 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 3, __reginatext ) ;}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 411 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (4)]) ;
                                         (yyval)->type = X_CALL_SET ;
                                         (yyval)->p[0] = (yyvsp[(2) - (4)]) ;
                                         (yyval)->name = (streng *)(yyvsp[(4) - (4)]) ;
                                         (yyval)->p[1] = (yyvsp[(3) - (4)]) ; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 416 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_CALL_SET ;
                                         (yyval)->p[0] = (yyvsp[(2) - (3)]) ;
                                         (yyval)->name = NULL ;
                                         (yyval)->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 421 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 422 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_CALL_SET ;
                                         (yyval)->p[0] = (yyvsp[(2) - (3)]) ;
                                         (yyval)->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 428 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 429 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 2, __reginatext );}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 432 "./yaccsrc.y"
    {
                                         /*
                                          * "call_args" accepted probably with
                                          * surrounding parentheses. Strip them.
                                          */
                                         (yyval) = reduce_expr_list( (yyvsp[(1) - (1)]),
                                                                REDUCE_CALL );
                                       }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 440 "./yaccsrc.y"
    { exiterror(ERR_UNEXPECTED_PARAN, 2); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 443 "./yaccsrc.y"
    { (yyval) = makenode(X_COMMAND,0) ;
                                         (yyval)->charnr = tmpchr ;
                                         (yyval)->lineno = tmplno;
                                         (yyval)->p[0] = (yyvsp[(1) - (1)]) ; }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 449 "./yaccsrc.y"
    { (yyval) = makenode(X_END,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         level--; }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 455 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->name = (streng*)((yyvsp[(2) - (2)])) ; }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 457 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 458 "./yaccsrc.y"
    {  exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 462 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (6)]);
                                         (yyval)->p[0] = (yyvsp[(2) - (6)]);
                                         (yyval)->p[1] = (yyvsp[(3) - (6)]);
                                         (yyval)->p[2] = optgluelast( (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)]) );
                                         if ( (yyval)->p[2] )
                                            (yyval)->p[2]->o.last = NULL;
                                         (yyval)->p[3] = (yyvsp[(6) - (6)]);
                          if (((yyval)->p[0]==NULL || (yyval)->p[0]->name==NULL)
                              && (yyval)->p[3]->name)
                                            exiterror( ERR_UNMATCHED_END, 0 );
                          if (((yyval)->p[0])&&((yyval)->p[0]->name)&&
                              ((yyval)->p[3]->name)&&
                              (((yyval)->p[3]->name->len != (yyval)->p[0]->name->len)||
                               (strncmp((yyval)->p[3]->name->value,
                                        (yyval)->p[0]->name->value,
                                        (yyval)->p[0]->name->len))))
                                            exiterror( ERR_UNMATCHED_END, 0 );
                                       }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 483 "./yaccsrc.y"
    { (yyval) =makenode(X_REP,4,(yyvsp[(3) - (7)]),(yyvsp[(5) - (7)]),(yyvsp[(6) - (7)]),(yyvsp[(7) - (7)])) ;
                                         (yyval)->name = (streng *)(yyvsp[(1) - (7)]) ;
                                         checkdosyntax((yyval)) ; }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 487 "./yaccsrc.y"
    { (yyval) =makenode(X_REP,3,(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]),(yyvsp[(6) - (6)])) ;
                                         (yyval)->name = (streng *)(yyvsp[(1) - (6)]) ;
                                         checkdosyntax((yyval)) ; }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 491 "./yaccsrc.y"
    { (yyval) = makenode(X_REP,2,(yyvsp[(3) - (5)]),(yyvsp[(5) - (5)])) ;
                                         (yyval)->name = (streng *)(yyvsp[(1) - (5)]) ;
                                         checkdosyntax((yyval)) ; }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 494 "./yaccsrc.y"
    { (yyval) = makenode(X_REP,1,(yyvsp[(3) - (4)])) ;
                                         (yyval)->name = (streng *)(yyvsp[(1) - (4)]) ;
                                         checkdosyntax((yyval)) ; }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 497 "./yaccsrc.y"
    { (yyval) = makenode(X_REP_FOREVER,0) ; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 498 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 16, "WHILE UNTIL", __reginatext ) ; }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 499 "./yaccsrc.y"
    { (yyvsp[(1) - (2)]) = makenode(X_DO_EXPR,1,(yyvsp[(1) - (2)])) ;
                                         (yyval) = makenode(X_REP,2,NULL,(yyvsp[(1) - (2)])) ; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 501 "./yaccsrc.y"
    { (yyval) = NULL ; }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 504 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 505 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 506 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 509 "./yaccsrc.y"
    { SymbolDetect |= SD_ADDRWITH ;
                                         (yyval) = with = makenode(X_ADDR_WITH,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 513 "./yaccsrc.y"
    { with = NULL ;
                                         SymbolDetect &= ~SD_ADDRWITH ; }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 515 "./yaccsrc.y"
    { (yyval) = NULL ; }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 518 "./yaccsrc.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 519 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 5, __reginatext ) ; }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 520 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 5, __reginatext ) ; }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 526 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 5, __reginatext ) ; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 562 "./yaccsrc.y"
    { with->p[0] = (yyvsp[(4) - (4)]); }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 563 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 6, __reginatext ) ; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 566 "./yaccsrc.y"
    { with->p[1] = (yyvsp[(4) - (4)]); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 567 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 7, __reginatext ) ; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 570 "./yaccsrc.y"
    { with->p[2] = (yyvsp[(4) - (4)]); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 571 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 14, __reginatext ) ; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 574 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 575 "./yaccsrc.y"
    { (yyval) = makenode(X_ADDR_WITH, 0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 580 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 581 "./yaccsrc.y"
    { (yyval) = (yyvsp[(2) - (2)]) ;
                                         (yyval)->u.of.append = 1 ; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 583 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 8, __reginatext ) ; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 584 "./yaccsrc.y"
    { (yyval) = (yyvsp[(2) - (2)]) ; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 585 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 9, __reginatext ) ; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 586 "./yaccsrc.y"
    { (yyval) = makenode(X_ADDR_WITH, 0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 591 "./yaccsrc.y"
    { /* ANSI extension: nsimsymb is
                                          * used by the standard, but I think
                                          * there are no reasons why using
                                          * it here as a must. FGC
                                          */
                                         (yyval) = (yyvsp[(2) - (2)]) ;
                                         (yyval)->u.of.awt = awtSTREAM;
                                         SymbolDetect |= SD_ADDRWITH ; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 599 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 1, __reginatext ) ; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 600 "./yaccsrc.y"
    {
                                         streng *tmp = (yyvsp[(2) - (2)])->name;
                                         char *p;

                                         /*
                                          * expect a single dot as the last character
                                          */
                                         p = (char *)memchr( tmp->value, '.', tmp->len );
                                         if ( p != tmp->value + tmp->len - 1 )
                                            exiterror( ERR_INVALID_OPTION, 3, __reginatext );
                                         (yyval) = (yyvsp[(2) - (2)]) ;
                                         (yyval)->u.of.awt = awtSTEM ;
                                         SymbolDetect |= SD_ADDRWITH ; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 613 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 2, __reginatext ) ; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 614 "./yaccsrc.y"
    {
                                         (yyval) = (yyvsp[(2) - (2)]) ;
                                         (yyval)->u.of.awt = awtLIFO ;
                                         SymbolDetect |= SD_ADDRWITH ; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 618 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 100, __reginatext ) ; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 619 "./yaccsrc.y"
    {
                                         (yyval) = (yyvsp[(2) - (2)]) ;
                                         (yyval)->u.of.awt = awtFIFO ;
                                         SymbolDetect |= SD_ADDRWITH ; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 623 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 101, __reginatext ) ; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 626 "./yaccsrc.y"
    { SymbolDetect &= ~SD_ADDRWITH ; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 627 "./yaccsrc.y"
    { (yyval) = (yyvsp[(3) - (3)]) ; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 630 "./yaccsrc.y"
    { SymbolDetect &= ~SD_ADDRWITH ; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 631 "./yaccsrc.y"
    { (yyval) = (yyvsp[(3) - (3)]) ; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 638 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 639 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 642 "./yaccsrc.y"
    { (yyval) = makenode(X_ADDR_WITH, 0) ;
                                         (yyval)->name = (streng *) (yyvsp[(1) - (1)]) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         (yyval)->u.of.ant = antSIMSYMBOL;
                                       }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 650 "./yaccsrc.y"
    { (yyval) = makenode(X_ADDR_WITH, 0) ;
                                         (yyval)->name = (streng *) (yyvsp[(1) - (1)]) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         (yyval)->u.of.ant = antSTRING;
                                       }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 658 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 661 "./yaccsrc.y"
    { (yyval) = makenode(X_DO_TO,1,(yyvsp[(2) - (3)])) ; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 662 "./yaccsrc.y"
    { (yyval) = makenode(X_DO_FOR,1,(yyvsp[(2) - (3)])) ; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 663 "./yaccsrc.y"
    { (yyval) = makenode(X_DO_BY,1,(yyvsp[(2) - (3)])) ; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 666 "./yaccsrc.y"
    { (yyval) = makenode(X_WHILE,1,(yyvsp[(2) - (3)])) ; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 667 "./yaccsrc.y"
    { (yyval) = makenode(X_UNTIL,1,(yyvsp[(2) - (3)])) ; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 668 "./yaccsrc.y"
    { (yyval) = NULL ; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 671 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 672 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 676 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;}
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 677 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 681 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 686 "./yaccsrc.y"
    { move_labels( (yyvsp[(1) - (6)]), (yyvsp[(6) - (6)]), level - 1 );
                                         (yyval) = (yyvsp[(1) - (6)]);
                                         (yyval)->p[0] = optgluelast( (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]) );
                                         (yyval)->p[0]->o.last = NULL;
                                         (yyval)->p[1] = optgluelast( (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)]) );
                                         (yyval)->p[1]->o.last = NULL;
                                         level--; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 694 "./yaccsrc.y"
    { move_labels( (yyvsp[(1) - (9)]), (yyvsp[(9) - (9)]), level - 1 );
                                         (yyval) = (yyvsp[(1) - (9)]);
                                         (yyval)->p[0] = optgluelast( (yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]) );
                                         (yyval)->p[0]->o.last = NULL;
                                         (yyval)->p[1] = optgluelast( (yyvsp[(5) - (9)]), (yyvsp[(6) - (9)]) );
                                         (yyval)->p[1]->o.last = NULL;
                                         (yyval)->p[2] = optgluelast( (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)]) );
                                         (yyval)->p[2]->o.last = NULL;
                                         level--; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 704 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 4 ) ;}
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 706 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 3 ) ;}
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 707 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 0 ) ;}
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 708 "./yaccsrc.y"
    {  exiterror( ERR_THEN_EXPECTED, 1, parser_data.if_linenr, __reginatext )  ; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 711 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 1 )  ; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 714 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 2 )  ; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 717 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 722 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 724 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 725 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 728 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ;
                                         (yyval)->u.trace_only =
                                                         (level == 0) ? 0 : 1;
                                         newlabel( (const tsd_t *)parser_data.TSD,
                                                   &parser_data,
                                                   (yyvsp[(1) - (1)]) ) ; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 736 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ;
                                         (yyval)->name = Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 740 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 742 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 743 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 746 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 747 "./yaccsrc.y"
    {  exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 750 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_NUM_D ;
                                         (yyval)->p[0] = (yyvsp[(3) - (3)]) ; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 753 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]); (yyval)->type = X_NUM_DDEF ; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 754 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 755 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_NUM_F ;
                                         (yyval)->p[0] = (yyvsp[(3) - (3)]) ; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 758 "./yaccsrc.y"
    { /* NOTE! This clashes ANSI! */
                                         (yyval) = (yyvsp[(1) - (2)]) ; (yyval)->type=X_NUM_FRMDEF ;}
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 760 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (4)]) ; (yyval)->type=X_NUM_V ;
                                         (yyval)->p[0] = (yyvsp[(4) - (4)]) ; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 762 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 11, "ENGINEERING SCIENTIFIC", __reginatext ) ;}
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 763 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]); (yyval)->type = X_NUM_FDEF ;}
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 764 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_NUM_FUZZ ;
                                         (yyval)->p[0] = (yyvsp[(3) - (3)]) ; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 767 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 15, "DIGITS FORM FUZZ", __reginatext ) ;}
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 770 "./yaccsrc.y"
    { (yyval) = makenode(X_NUM_SCI,0) ; }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 771 "./yaccsrc.y"
    { (yyval) = makenode(X_NUM_ENG,0) ; }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 774 "./yaccsrc.y"
    { ((yyval)=(yyvsp[(1) - (2)]))->p[0]=(yyvsp[(2) - (2)]) ; }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 778 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (4)]) ;
                                         (yyval)->type = X_PARSE ;
                                         (yyval)->u.parseflags = (int) (yyvsp[(2) - (4)]) ;
                                         (yyval)->p[0] = (yyvsp[(3) - (4)]) ;
                                         (yyval)->p[1] = (yyvsp[(4) - (4)]) ; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 784 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_PARSE ;
                                         (yyval)->u.parseflags = 0;
                                         (yyval)->p[0] = (yyvsp[(2) - (3)]) ;
                                         (yyval)->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 789 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 12, "ARG EXTERNAL LINEIN PULL SOURCE VAR VALUE VERSION", __reginatext ) ;}
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 790 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 12, "ARG CASELESS EXTERNAL LINEIN LOWER PULL SOURCE UPPER VAR VALUE VERSION", __reginatext ) ;}
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 793 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_UPPER  |
                                                         PARSE_NORMAL); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 795 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_UPPER  |
                                                         PARSE_CASELESS); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 797 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_UPPER  |
                                                         PARSE_CASELESS); }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 799 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_LOWER  |
                                                         PARSE_NORMAL); }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 801 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_LOWER  |
                                                         PARSE_CASELESS); }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 803 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_LOWER  |
                                                         PARSE_CASELESS); }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 805 "./yaccsrc.y"
    { (yyval) = (nodeptr) (PARSE_NORMAL |
                                                         PARSE_CASELESS); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 809 "./yaccsrc.y"
    { /* fixes bugs like bug 579711 */
                                         (yyval) = optgluelast( (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 811 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 814 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_ARG,0) ; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 815 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_EXT,0) ; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 816 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_EXT,0) ; }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 817 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_VER,0) ; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 818 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_PULL,0) ; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 819 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_SRC,0) ; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 820 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_VAR,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 822 "./yaccsrc.y"
    { (yyval) = makenode(X_PARSE_VAL,1,(yyvsp[(2) - (3)])) ; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 823 "./yaccsrc.y"
    { exiterror( ERR_INVALID_TEMPLATE, 3 ) ;}
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 826 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 827 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 17, __reginatext ) ;}
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 828 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;}
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 829 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;}
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 830 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->p[0] = (yyvsp[(3) - (3)]) ; }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 834 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 838 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 842 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 846 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 850 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 854 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_END, 0 ) ;}
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 855 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 856 "./yaccsrc.y"
    { level--; }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 860 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (5)]);
                                         (yyval)->p[0] = optgluelast( (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]) );
                                         (yyval)->p[0]->o.last = NULL;
                                         (yyval)->p[1] = (yyvsp[(4) - (5)]); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 864 "./yaccsrc.y"
    {  exiterror( ERR_WHEN_EXPECTED, 1, parser_data.select_linenr, __reginatext ) ;}
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 866 "./yaccsrc.y"
    {  exiterror( ERR_WHEN_EXPECTED, 2, parser_data.select_linenr, __reginatext ) ;}
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 867 "./yaccsrc.y"
    {  exiterror( ERR_EXTRA_DATA, 1, __reginatext )  ;}
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 868 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 0 ) ;}
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 870 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 0 ) ;}
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 873 "./yaccsrc.y"
    { (yyval) = optgluelast( (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]) ); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 874 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 875 "./yaccsrc.y"
    {  exiterror( ERR_WHEN_EXPECTED, 2, parser_data.select_linenr, __reginatext )  ;}
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 879 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (6)]); /* fixes bugs like bug 579711 */
                                         (yyval)->p[0] = optgluelast( (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]) );
                                         (yyval)->p[0]->o.last = NULL;
                                         (yyval)->p[1] = optgluelast( (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)]) );
                                         (yyval)->p[1]->o.last = NULL; }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 885 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 0 ) ;}
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 886 "./yaccsrc.y"
    {  exiterror( ERR_THEN_EXPECTED, 2, parser_data.select_linenr, __reginatext )  ; }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 887 "./yaccsrc.y"
    {  exiterror( ERR_INVALID_EXPRESSION, 0 ) ;}
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 897 "./yaccsrc.y"
    { exiterror( ERR_WHEN_UNEXPECTED, 1 ); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 898 "./yaccsrc.y"
    { exiterror( ERR_WHEN_UNEXPECTED, 2 ); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 901 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]);
                                         (yyval)->p[0] = optgluelast( (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) );
                                         if ( (yyval)->p[0] )
                                            (yyval)->p[0]->o.last = NULL; }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 905 "./yaccsrc.y"
    { (yyval) = makenode(X_NO_OTHERWISE,0) ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->charnr = parser_data.tstart ; }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 911 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_SIG_VAL ;
                                         (yyval)->p[0] = (yyvsp[(3) - (3)]) ; }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 914 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 915 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->name = (streng *)(yyvsp[(2) - (2)]) ; }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 917 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 3, "ERROR FAILURE HALT NOTREADY NOVALUE SYNTAX LOSTDIGITS", __reginatext ) ;}
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 918 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 4, "ERROR FAILURE HALT NOTREADY NOVALUE SYNTAX LOSTDIGITS", __reginatext ) ;}
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 919 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 920 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 3, __reginatext ) ;}
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 922 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (4)]) ;
                                         (yyval)->type = X_SIG_SET ;
                                         (yyval)->p[0] = (yyvsp[(2) - (4)]) ;
                                         (yyval)->name = (streng *)(yyvsp[(4) - (4)]) ;
                                         (yyval)->p[1] = (yyvsp[(3) - (4)]) ; }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 927 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_SIG_SET ;
                                         (yyval)->p[0] = (yyvsp[(2) - (3)]) ;
                                         (yyval)->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 931 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 932 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->type = X_SIG_SET ;
                                         (yyval)->p[0] = (yyvsp[(2) - (3)]) ;
                                         (yyval)->p[1] = (yyvsp[(3) - (3)]) ; }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 938 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 939 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 940 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 4, __reginatext );}
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 943 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue);}
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 944 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 945 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 3, __reginatext ) ;}
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 948 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 949 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 952 "./yaccsrc.y"
    { (yyval) = makenode(X_ON,0) ; }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 955 "./yaccsrc.y"
    { (yyval) = makenode(X_OFF,0) ; }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 958 "./yaccsrc.y"
    { (yyval) = makenode(X_S_ERROR,0) ; }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 959 "./yaccsrc.y"
    { (yyval) = makenode(X_S_HALT,0) ; }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 960 "./yaccsrc.y"
    { (yyval) = makenode(X_S_NOTREADY,0) ; }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 961 "./yaccsrc.y"
    { (yyval) = makenode(X_S_FAILURE,0) ; }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 964 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 965 "./yaccsrc.y"
    { (yyval) = makenode(X_S_NOVALUE,0) ; }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 966 "./yaccsrc.y"
    { (yyval) = makenode(X_S_SYNTAX,0) ; }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 967 "./yaccsrc.y"
    { (yyval) = makenode(X_S_LOSTDIGITS,0) ; }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 970 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (3)]) ;
                                         (yyval)->p[0] = (yyvsp[(3) - (3)]) ; }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 972 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (2)]) ; }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 974 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;}
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 975 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 979 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 983 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]) ;
                                         (yyval)->p[1] = (yyvsp[(2) - (2)]) ;
                                         /*
                                          * An assignment is a numerical
                                          * assignment if and only if we have
                                          * to do a numerical operation, which
                                          * is equivalent to the existence of
                                          * one more argument to $2.
                                          * This fixes bug 720166.
                                          */
                                         if ((yyvsp[(2) - (2)]) &&
                                             (yyvsp[(2) - (2)])->p[0] &&
                                             gettypeof((yyvsp[(2) - (2)])) == IS_A_NUMBER)
                                            (yyval)->type = X_NASSIGN ; }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 999 "./yaccsrc.y"
    { (yyval) = makenode(X_ASSIGN,0) ;
                                         (yyval)->charnr = parser_data.tstart ;
                                         (yyval)->lineno = parser_data.tline ;
                                         (yyval)->p[0] = (nodeptr)create_head( (const char *)retvalue ); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1006 "./yaccsrc.y"
    { /* We have to accept exprs here even
                                          * if we just want to accept
                                          * '(' expr ')' only. We do this
                                          * because we appect
                                          * "call '(' exprs ')'" too.
                                          * This kann happen only if the
                                          * related control flag parendepth is
                                          * set. But since the parentheses are
                                          * voided just for the outer ones, we
                                          * can reduce the allowness level.
                                          * We don't have to set it back,
                                          * because the outer parentheses
                                          * either is THE one we look for or
                                          * none. This allows a faster error
                                          * detection and that's a good goal.*/
                                         parendepth--; }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1022 "./yaccsrc.y"
    { parendepth++;
                                         if ( parendepth == 1 )
                                         {
                                            /* exprs on as-is basis */
                                            (yyval) = (yyvsp[(3) - (3)]);
                                         }
                                         else
                                         {
                                            /* Must already be a plain expr.
                                             * The nexpr part of exprs detects
                                             * mistakes. */
                                            (yyval) = reduce_expr_list( (yyvsp[(3) - (3)]),
                                                                REDUCE_EXPR );
                                            if ( (yyval) == (yyvsp[(3) - (3)]) )
                                               exiterror( ERR_INTERPRETER_FAILURE, 1, __FILE__, __LINE__, "Reduction of `exprs' not happened." );
                                         }
                                       }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1039 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1040 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "+" );
                                         (yyval) = makenode( X_PLUSS, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1042 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1043 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "-" );
                                         (yyval) = makenode( X_MINUS, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1045 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1046 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "*" );
                                         (yyval) = makenode( X_MULT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1048 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "*" ); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1049 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1050 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "/" );
                                         (yyval) = makenode( X_DEVIDE, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1052 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "/" ); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1053 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1054 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "//" );
                                         (yyval) = makenode( X_MODULUS, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1056 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "//" ); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1057 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1058 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "%" );
                                         (yyval) = makenode( X_INTDIV, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1060 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "%" ); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1061 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1062 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "|" );
                                         (yyval) = makenode( X_LOG_OR, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1064 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "|" ); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1065 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1066 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "&" );
                                         (yyval) = makenode( X_LOG_AND, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1068 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "&" ); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1069 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1070 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "&&" );
                                         (yyval) = makenode( X_LOG_XOR, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1072 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "&&" ); }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1073 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1074 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "**" );
                                         (yyval) = makenode( X_EXP, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1076 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "**" ); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1077 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1078 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), " " );
                                         (yyval) = makenode( X_SPACE, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1080 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, " " ); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1081 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1082 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "||" );
                                         (yyval) = makenode( X_CONCAT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1084 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "||" ); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1085 "./yaccsrc.y"
    { AUTO_REDUCE( (yyvsp[(2) - (2)]), "\\" );
                                         (yyval) = makenode( X_LOG_NOT, 1, (yyvsp[(2) - (2)]) ); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1087 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\" ); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1088 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1089 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "=" );
                                         (yyval) = makenode( X_EQUAL, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         transform( (yyval) ); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1092 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "=" ); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1093 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1094 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), ">=" );
                                         (yyval) = makenode( X_GTE, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         transform( (yyval) ) ; }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1097 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">=" ); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1098 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1099 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "<=" );
                                         (yyval) = makenode( X_LTE, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         transform( (yyval) ) ; }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1102 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<=" ); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1103 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1104 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), ">" );
                                         (yyval) = makenode( X_GT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         transform( (yyval) ) ; }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1107 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">" ); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1108 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1109 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "<" );
                                         (yyval) = makenode( X_LT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         transform( (yyval) ) ; }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1112 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<" ); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1113 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1114 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "\\=" );
                                         (yyval) = makenode( X_DIFF, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         transform( (yyval) ) ; }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1117 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\=" ); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1118 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1119 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "==" );
                                         (yyval) = makenode( X_S_EQUAL, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1121 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "==" ); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1122 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1123 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "\\==" );
                                         (yyval) = makenode( X_S_DIFF, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1125 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\==" ); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1126 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1127 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), ">>" );
                                         (yyval) = makenode( X_S_GT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1129 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">>" ); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1130 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1131 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "<<" );
                                         (yyval) = makenode( X_S_LT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1133 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<<" ); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1134 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1135 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "\\>>" );
                                         (yyval) = makenode( X_S_NGT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1137 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\>>" ); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1138 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1139 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "\\<<" );
                                         (yyval) = makenode( X_S_NLT, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1141 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\<<" ); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1142 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1143 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), ">>=" );
                                         (yyval) = makenode( X_S_GTE, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1145 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">>=" ); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1146 "./yaccsrc.y"
    { parendepth--; }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1147 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( (yyvsp[(1) - (4)]), "<<=" );
                                         (yyval) = makenode( X_S_LTE, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) ); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1149 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<<=" ); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1150 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1151 "./yaccsrc.y"
    { (yyval) = makenode( X_STRING, 0 );
                                         (yyval)->name = Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1153 "./yaccsrc.y"
    { (yyval) = makenode( X_STRING, 0 );
                                         (yyval)->name = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy((yyval)->name->value,retvalue,
                                                    (yyval)->name->len=retlength); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1157 "./yaccsrc.y"
    { (yyval) = makenode( X_STRING, 0 );
                                         (yyval)->name = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy((yyval)->name->value,retvalue,
                                                    (yyval)->name->len=retlength); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1161 "./yaccsrc.y"
    { (yyval) = makenode( X_STRING, 0 );
                                         (yyval)->name = Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1163 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1164 "./yaccsrc.y"
    { AUTO_REDUCE( (yyvsp[(2) - (2)]), nullptr );
                                         (yyval) = makenode( X_U_PLUSS, 1, (yyvsp[(2) - (2)]) ); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1166 "./yaccsrc.y"
    { AUTO_REDUCE( (yyvsp[(2) - (2)]), nullptr );
                                         (yyval) = makenode( X_U_MINUS, 1, (yyvsp[(2) - (2)]) ); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1168 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, __reginatext ); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1169 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, __reginatext ); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1172 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (2)]); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1173 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_PARAN, 0 ); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1174 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_PARAN, 0 ); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1177 "./yaccsrc.y"
    { (yyval) = (nodeptr)create_head( (const char *)retvalue ) ; }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1181 "./yaccsrc.y"
    { (yyval) = makenode(X_EX_FUNC,1,(yyvsp[(2) - (2)])) ;
                                         (yyval)->name = (streng *)(yyvsp[(1) - (2)]) ; }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1183 "./yaccsrc.y"
    { (yyval) = makenode(X_IN_FUNC,1,(yyvsp[(2) - (2)])) ;
                                         (yyval)->name = (streng *)(yyvsp[(1) - (2)]) ; }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1187 "./yaccsrc.y"
    { /* ugly fake preservs parendepth */
                                         (yyval) = (YYSTYPE) parendepth;
                                         parendepth = 0; }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1190 "./yaccsrc.y"
    { parendepth = (int) (yyval);
                                         (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1194 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1197 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1200 "./yaccsrc.y"
    { (yyval) =makenode(X_TPL_SOLID,3,(yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]),(yyvsp[(3) - (3)]));}
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1201 "./yaccsrc.y"
    { (yyval) =makenode(X_TPL_SOLID,1,(yyvsp[(1) - (1)])) ; }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1202 "./yaccsrc.y"
    { exiterror( ERR_INVALID_TEMPLATE, 1, __reginatext ) ;}
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1205 "./yaccsrc.y"
    { (yyval) = makenode(X_NEG_OFFS,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1207 "./yaccsrc.y"
    { (yyval) = makenode(X_POS_OFFS,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1209 "./yaccsrc.y"
    { (yyval) = makenode(X_ABS_OFFS,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(1) - (1)]) ; }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1211 "./yaccsrc.y"
    { (yyval) = makenode(X_ABS_OFFS,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (2)]) ; }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1213 "./yaccsrc.y"
    { (yyval) = makenode(X_TPL_VAR,0) ;
                                         (yyval)->p[0] = (yyvsp[(2) - (3)]) ; }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1215 "./yaccsrc.y"
    { (yyval) = makenode(X_NEG_OFFS,0) ;
                                         (yyval)->p[0] = (yyvsp[(3) - (4)]) ; }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1217 "./yaccsrc.y"
    { (yyval) = makenode(X_POS_OFFS,0) ;
                                         (yyval)->p[0] = (yyvsp[(3) - (4)]) ; }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1219 "./yaccsrc.y"
    { (yyval) = makenode(X_ABS_OFFS,0) ;
                                         (yyval)->p[0] = (yyvsp[(3) - (4)]) ; }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1221 "./yaccsrc.y"
    { (yyval) = makenode(X_TPL_MVE,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(1) - (1)]) ; }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1225 "./yaccsrc.y"
    { (yyval) = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1226 "./yaccsrc.y"
    { streng *sptr = Str_cre_TSD(parser_data.TSD,retvalue) ;
                                          if (myisnumber(parser_data.TSD, sptr))
                                          {
                                             exiterror( ERR_INVALID_INTEGER, 4, sptr->value ) ;
                                          }
                                          else
                                             exiterror( ERR_INVALID_TEMPLATE, 0 ) ;}
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1235 "./yaccsrc.y"
    { (yyval) = (nodeptr) Str_cre_TSD(parser_data.TSD,retvalue) ; }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1236 "./yaccsrc.y"
    { streng *sptr = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy(sptr->value,retvalue,
                                                 sptr->len=retlength) ;
                                         (yyval) = (nodeptr) sptr ; }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1240 "./yaccsrc.y"
    { streng *sptr = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy(sptr->value,retvalue,
                                                 sptr->len=retlength) ;
                                         (yyval) = (nodeptr) sptr ; }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1246 "./yaccsrc.y"
    { (yyval) = makenode(X_TPL_POINT,1,(yyvsp[(2) - (2)])) ; }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1247 "./yaccsrc.y"
    { (yyval) = makenode(X_TPL_SYMBOL,2,(yyvsp[(2) - (2)]),(yyvsp[(1) - (2)])) ; }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1248 "./yaccsrc.y"
    { (yyval) = NULL ; }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1258 "./yaccsrc.y"
    { /* detect
                                          * "x = ( a,. b )",
                                          * "x = ( ,. b )",
                                          * "x = a,. b",
                                          * "x = ,. b",
                                          * "x = f( ( x,. b ) )",
                                          * "x = f( ( ,. b ) )"      */
                                         if ( parendepth < 0 )
                                            exiterror( ERR_UNEXPECTED_PARAN, 1 );

                                         /* With call being the extended kind
                                          * of CALL we may have:
                                          * "x = f( (a),. b )",
                                          * "x = f( a,. b )",
                                          * "x = f( ,. b )",
                                          * "CALL s (a),. b",
                                          * "CALL s a,. b",
                                          * "CALL s ,. b",
                                          * "call s (a),. b",
                                          * "call s a,. b",
                                          * "call s ,. b",
                                          * "call s( (a),. b )",
                                          * "call s( a,. b )",
                                          * "call s( ,. b )",
                                          * "call s (a,a),. b",
                                          * "call s (a),. b",
                                          * "call s (),. b"
                                          *
                                          * detect "(a),." and transform it
                                          * to "a,."                         */
                                         (yyvsp[(1) - (2)]) = reduce_expr_list( (yyvsp[(1) - (2)]),
                                                                REDUCE_EXPR );

                                         /* detect "call s (a,b),. b" and
                                          * "call s (),. b", but every list on
                                          * the left side of "," is wrong, so
                                          * complain about every exprlist.   */
                                         if ( IS_EXPRLIST( (yyvsp[(1) - (2)]) ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 1 );

                                         (yyvsp[(1) - (2)]) = reduce_expr_list( (yyvsp[(1) - (2)]),
                                                              REDUCE_SUBEXPR );
                                       }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1301 "./yaccsrc.y"
    { /*
                                          * Fixes bug 961301.
                                          */
                                         nodeptr curr;

                                         assert( IS_EXPRLIST( (yyvsp[(4) - (4)]) ) );

                                         /* detect ",()." */
                                         if ( IS_EXPRLIST( (yyvsp[(4) - (4)])->p[0] )
                                           && ( (yyvsp[(4) - (4)])->p[1] == NULL )
                                           && ( (yyvsp[(4) - (4)])->p[0]->p[0] == NULL ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 0 );

                                         /* detect ",(a,b)." */
                                         if ( IS_EXPRLIST( (yyvsp[(4) - (4)])->p[0] )
                                           && ( (yyvsp[(4) - (4)])->p[1] == NULL )
                                           && IS_EXPRLIST( (yyvsp[(4) - (4)])->p[0]->p[1] ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 1 );

                                         /* detect ",(a)." and transform it
                                          * to ",a."                         */
                                         (yyvsp[(4) - (4)]) = reduce_expr_list( (yyvsp[(4) - (4)]),
                                                                REDUCE_RIGHT );
                                         assert( IS_EXPRLIST( (yyvsp[(4) - (4)]) ) );

                                         /* Detect something like
                                          * "call s (a,b)+1"                 */
                                         curr = (yyvsp[(4) - (4)])->p[0];
                                         if ( ( curr != NULL )
                                           && !IS_EXPRLIST( curr )
                                           && !IS_FUNCTION( curr )
                                           && ( IS_EXPRLIST( curr->p[0] )
                                             || IS_EXPRLIST( curr->p[1] ) ) )
                                            exiterror( ERR_INVALID_EXPRESSION, 0 );

                                         (yyval) = makenode( X_EXPRLIST, 2, (yyvsp[(1) - (4)]), (yyvsp[(4) - (4)]) );
                                         checkconst( (yyval) ); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1338 "./yaccsrc.y"
    { /* detect
                                          * "x = ()."
                                          * "x = f(().)"
                                          * "call s (().)"
                                          * "CALL s ()."                     */
                                         if ( ( parendepth < 0 ) && ( (yyvsp[(1) - (1)]) == NULL ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 0 );

                                         /* With call being the extended kind
                                          * of CALL we may have:
                                          * "x = ( a. )",
                                          * "x = f( . )",
                                          * "x = f( ., )",
                                          * "x = f( a. )",
                                          * "x = f( a., )",
                                          * "x = f( a, . )",
                                          * "x = f( a, b. )",
                                          * "CALL s .",
                                          * "CALL s .,",
                                          * "CALL s a.,",
                                          * "CALL s a, .",
                                          * "CALL s a, b.",
                                          * "call s .",
                                          * "call s .,",
                                          * "call s a.,",
                                          * "call s a, .",
                                          * "call s a, b.",
                                          * "call s (a.)",
                                          * "call s (a)+1, .",
                                          * "call s (a), .",
                                          * "call s (a), a.",
                                          * "call s (a), (a).",
                                          * "call s ( ., )",
                                          * "call s ( a., )",
                                          * "call s ( a, . )",
                                          * "call s ( a, b. )"               */

                                         (yyvsp[(1) - (1)]) = reduce_expr_list( (yyvsp[(1) - (1)]),
                                                              REDUCE_SUBEXPR );
                                         (yyval) = makenode( X_EXPRLIST, 1, (yyvsp[(1) - (1)]) );
                                         checkconst( (yyval) );
                                        }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1382 "./yaccsrc.y"
    { (yyval) = (yyvsp[(1) - (1)]) ; }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1383 "./yaccsrc.y"
    { (yyval) = NULL ; }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1386 "./yaccsrc.y"
    { (yyval) = makenode(X_SIM_SYMBOL,1,(yyvsp[(2) - (2)])) ;
                                         (yyval)->name = (streng *) (yyvsp[(1) - (2)]) ; }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1388 "./yaccsrc.y"
    { (yyval) = makenode(X_SIM_SYMBOL,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(1) - (1)]) ; }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1390 "./yaccsrc.y"
    { (yyval) = makenode(X_IND_SYMBOL,1,(yyvsp[(4) - (4)])) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (4)]) ; }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1392 "./yaccsrc.y"
    { (yyval) = makenode(X_IND_SYMBOL,0) ;
                                         (yyval)->name = (streng *) (yyvsp[(2) - (3)]) ; }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1396 "./yaccsrc.y"
    { (yyval) = (treenode *) Str_cre_TSD(parser_data.TSD,retvalue);}
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1399 "./yaccsrc.y"
    { (yyval) = (treenode *) Str_cre_TSD(parser_data.TSD,retvalue);}
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1400 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;}
    break;



/* Line 1806 of yacc.c  */
#line 5819 "./yaccsrc.c"
      default: break;
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
     yydestruct ("Error: discarding",
            yytoken, &yylval);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;   /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
        yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
        yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1403 "./yaccsrc.y"


static nodeptr makenode( int type, int numb, ... )
{
   nodeptr thisleave ;
   va_list argptr ;
   int i ;

   assert(numb <= (int) (sizeof(thisleave->p)/sizeof(thisleave->p[0])));
#ifdef REXXDEBUG
   printf("makenode: making new node, type: %d\n",type) ;
#endif /* REXXDEBUG */

   thisleave = FreshNode( ) ;
   /* thisleave is initialized to zero except for nodeindex */
   va_start( argptr, numb ) ;
   thisleave->type = type ;
   thisleave->lineno = -1 ;
   thisleave->charnr = -1 ;
   for (i=0;i<numb;i++)
      thisleave->p[i]=va_arg(argptr, nodeptr) ;

   va_end( argptr ) ;

   return( thisleave ) ;
}

static char *getdokeyword( int type )
{
   char *ptr;
   switch( type )
   {
      case X_DO_TO: ptr="TO";break;
      case X_DO_BY: ptr="BY";break;
      case X_DO_FOR: ptr="FOR";break;
      default: ptr="";break;
   }
   return ptr;
}

static void checkdosyntax( cnodeptr thisptr )
{
   if ((thisptr->p[1]!=NULL)&&(thisptr->p[2]!=NULL))
   {
      if ((thisptr->p[1]->type)==(thisptr->p[2]->type))
      {
         exiterror( ERR_INVALID_DO_SYNTAX, 1, getdokeyword(thisptr->p[1]->type) )  ;
      }
   }
   if ((thisptr->p[2]!=NULL)&&(thisptr->p[3]!=NULL))
   {
      if ((thisptr->p[2]->type)==(thisptr->p[3]->type))
      {
         exiterror( ERR_INVALID_DO_SYNTAX, 1, getdokeyword(thisptr->p[2]->type) )  ;
      }
   }
   if ((thisptr->p[1]!=NULL)&&(thisptr->p[3]!=NULL))
   {
      if ((thisptr->p[1]->type)==(thisptr->p[3]->type))
      {
         exiterror( ERR_INVALID_DO_SYNTAX, 1, getdokeyword(thisptr->p[1]->type) )  ;
      }
   }
   return ;
}


void newlabel( const tsd_t *TSD, internal_parser_type *ipt, nodeptr thisptr )
{
   labelboxptr newptr ;

   assert( thisptr ) ;

   newptr = (labelboxptr)MallocTSD(sizeof(labelbox)) ;

   newptr->next = NULL ;
   newptr->entry = thisptr ;
   if (ipt->first_label == NULL)
   {
      ipt->first_label = newptr ;
      ipt->last_label = newptr ; /* must be NULL, too */
   }
   else
   {
      ipt->last_label->next = newptr ;
      ipt->last_label = newptr ;
   }
   ipt->numlabels++;
}

static nodeptr create_tail( const char *name )
{
   const char *cptr ;
   nodeptr node ;
   int constant ;
   streng *tname ;
   tsd_t *TSD = parser_data.TSD;

   if (!*name)
   {
      node = makenode( X_CTAIL_SYMBOL, 0 ) ;
      node->name = Str_make_TSD( parser_data.TSD, 0) ;
      return node ;
   }

   cptr = name ;
   constant = rx_isdigit(*cptr) || *cptr=='.' || (!*cptr) ;
   node = makenode( (constant) ? X_CTAIL_SYMBOL : X_VTAIL_SYMBOL, 0 ) ;

   for (;*cptr && *cptr!='.'; cptr++) ;
   node->name = Str_ncre_TSD( parser_data.TSD, name, cptr-name ) ;

   if (*cptr)
   {
      node->p[0] = create_tail( ++cptr ) ;
      if (constant && node->p[0]->type==X_CTAIL_SYMBOL)
      {
         streng *first, *second ;
         nodeptr tptr ;

         first = node->name ;
         second = node->p[0]->name ;
         node->name = NULL;
         node->p[0]->name = NULL;
         tname = Str_makeTSD( first->len + second->len + 1) ;
         memcpy( tname->value, first->value, first->len ) ;
         tname->value[first->len] = '.' ;
         memcpy( tname->value+first->len+1, second->value, second->len) ;
         tname->len = first->len + second->len + 1 ;

         Free_stringTSD( first ) ;
         Free_stringTSD( second ) ;
         node->name = tname ;
         tptr = node->p[0] ;
         node->p[0] = tptr->p[0] ;
         RejectNode(tptr);
      }
   }

   return node ;
}

static nodeptr create_head( const char *name )
{
   const char *cptr ;
   nodeptr node ;

   /* Bypass reserved variables */
   cptr = ( *name ) ? ( name + 1 ) : name;
   for (; *cptr && *cptr!='.'; cptr++) ;
   node = makenode( X_SIM_SYMBOL, 0 ) ;
   node->name = Str_ncre_TSD( parser_data.TSD, name, cptr-name+(*cptr=='.')) ;

   if (*cptr)
   {
      if (*(++cptr))
         node->p[0] = create_tail( cptr ) ;
      else
         node->p[0] = NULL ;

      node->type = (node->p[0]) ? X_HEAD_SYMBOL : X_STEM_SYMBOL ;
   }

   return node ;
}


static node_type gettypeof( nodeptr thisptr )
{
   tsd_t *TSD = parser_data.TSD;

   switch(thisptr->type)
   {
      case X_PLUSS:
      case X_MINUS:
      case X_MULT:
      case X_U_PLUSS:
      case X_U_MINUS:
      case X_DEVIDE:
      case X_INTDIV:
      case X_MODULUS:
      case X_EQUAL:
      case X_DIFF:
      case X_GTE:
      case X_GT:
      case X_LTE:
      case X_LT:
      case X_SEQUAL:
      case X_SDIFF:
      case X_SGTE:
      case X_SGT:
      case X_SLTE:
      case X_SLT:
      case X_NEQUAL:
      case X_NDIFF:
      case X_NGTE:
      case X_NGT:
      case X_NLTE:
      case X_NLT:
         return IS_A_NUMBER ;


      case X_SIM_SYMBOL:
         return IS_SIM_SYMBOL ;

      case X_HEAD_SYMBOL:
         return IS_COMP_SYMBOL ;

      case X_STRING:
      case X_CON_SYMBOL:
      {
         if (thisptr->u.number)
         {
            fprintf( stderr, "Found an internal spot of investigation of the Regina interpreter.\n"
                             "Please inform Mark Hessling or Florian Coosmann about the\n"
                             "circumstances and this script.\n"
                             "\n"
                             "Many thanks!\n"
                             "email addresses:\n"
                             "m.hessling@qut.edu.au\n"
                             "florian@grosse-coosmann.de\n");
            /* FIXME: When does this happen?
             * It doesn't happen if no feedback is send until end of 2005.
             */
            return IS_A_NUMBER ;
         }

         if ( ( thisptr->u.number = is_a_descr( TSD, thisptr->name ) ) != NULL )
            return IS_A_NUMBER;
         return IS_NO_NUMBER;
      }
   }
   return IS_UNKNOWN ;
}



static void transform( nodeptr thisptr )
{
   int type ;
   node_type left,rght;

   left = gettypeof( thisptr->p[0] ) ;
   rght = gettypeof( thisptr->p[1] ) ;
   type = thisptr->type ;

   if ( ( left == IS_A_NUMBER ) && ( rght == IS_A_NUMBER ) )
   {
      if (type==X_EQUAL)
         thisptr->type = X_NEQUAL ;
      else if (type==X_DIFF)
         thisptr->type = X_NDIFF ;
      else if (type==X_GTE)
         thisptr->type = X_NGTE ;
      else if (type==X_GT)
         thisptr->type = X_NGT ;
      else if (type==X_LTE)
         thisptr->type = X_NLTE ;
      else if (type==X_LT)
         thisptr->type = X_NLT ;
   }
   else if ( ( left == IS_NO_NUMBER ) || ( rght == IS_NO_NUMBER ) )
   {
      if (type==X_EQUAL)
         thisptr->type = X_SEQUAL ;
      else if (type==X_DIFF)
         thisptr->type = X_SDIFF ;
      else if (type==X_GTE)
         thisptr->type = X_SGTE ;
      else if (type==X_GT)
         thisptr->type = X_SGT ;
      else if (type==X_LTE)
         thisptr->type = X_SLTE ;
      else if (type==X_LT)
         thisptr->type = X_SLT ;
   }
   else
   {
      type = thisptr->p[0]->type ;
      if ( ( left == IS_A_NUMBER )
        && ( ( type == X_STRING ) || ( type == X_CON_SYMBOL ) ) )
         thisptr->u.flags.lnum = 1 ;
      else if ( left == IS_SIM_SYMBOL )
         thisptr->u.flags.lsvar = 1 ;
      else if ( left == IS_COMP_SYMBOL )
         thisptr->u.flags.lcvar = 1 ;

      type = thisptr->p[1]->type ;
      if ( ( rght == IS_A_NUMBER )
        && ( ( type == X_STRING ) || ( type == X_CON_SYMBOL ) ) )
         thisptr->u.flags.rnum = 1 ;
      else if ( rght == IS_SIM_SYMBOL )
         thisptr->u.flags.rsvar = 1 ;
      else if ( rght == IS_COMP_SYMBOL )
         thisptr->u.flags.rcvar = 1 ;
   }
}


static int is_const( cnodeptr thisptr )
{
   if (!thisptr)
      return 1 ;

   switch (thisptr->type)
   {
      case X_STRING:
      case X_CON_SYMBOL:
         return 1 ;

#if 0
      Pre-evaluation is not allowed. DIGITS and FUZZ may change within loops
      and the resulting value may or may not be the same. Concatenation with
      or without spaces is the sole allowed operation.

      NEVER ENABLE THIS AGAIN WITHOUT SOLVING THIS PROBLEMS!

      case X_U_PLUSS:
      case X_U_MINUS:
         return is_const( thisptr->p[0] ) ;

      case X_PLUSS:
      case X_MINUS:
      case X_MULT:
/*    case X_DEVIDE: Bug 20000807-41821 */
      case X_INTDIV:
      case X_MODULUS:
      case X_EQUAL:
      case X_DIFF:
      case X_GTE:
      case X_GT:
      case X_LTE:
      case X_LT:
      case X_SEQUAL:
      case X_SDIFF:
      case X_SGTE:
      case X_SGT:
      case X_SLTE:
      case X_SLT:
      case X_NEQUAL:
      case X_NDIFF:
      case X_NGTE:
      case X_NGT:
      case X_NLTE:
      case X_NLT:
#endif

      case X_SPACE:
      case X_CONCAT:
         return is_const( thisptr->p[0] ) && is_const( thisptr->p[1] ) ;
   }
   return 0 ;
}


static void checkconst( nodeptr thisptr )
{
   tsd_t *TSD = parser_data.TSD;

   assert( thisptr->type == X_EXPRLIST ) ;
   if (is_const(thisptr->p[0]))
   {
      if (thisptr->p[0])
         thisptr->u.strng = evaluate( TSD, thisptr->p[0], NULL ) ;
      else
         thisptr->u.strng = NULL ;

      thisptr->type = X_CEXPRLIST ;
   }
}

/*
 * reduce_expr_list will be invoked if the reduction of a list expression for
 * "call" arguments or a plain "(expr)" is needed. The reduction of the
 * outer parentheses of the extended CALL syntax is done with
 * mode==REDUCE_CALL, the reduction of a simple "(expr)" is done with
 * mode==REDUCE_EXPR. REDUCE_RIGHT is a synonym for REDUCE_CALL currently and
 * is intended to be used for reducing the right side of an expression list.
 *
 * REDUCE_SUBEXPR detect "call s (a)+1," and "call s 1+(a)," and reduces it.
 * Furthermore it detects "call s ()+1", "call s 1+()", "call s 1+(a,b)",
 * "call s (a,b)+1" and raises an error in this case.
 */
static nodeptr reduce_expr_list( nodeptr thisptr, reduce_mode mode )
{
   nodeptr h, retval = thisptr;

   if ( !thisptr )
      return retval;

   if ( mode == REDUCE_SUBEXPR )
   {
      if ( ( parendepth == 1 ) && !IS_FUNCTION( thisptr ) && !IS_EXPRLIST( thisptr ) )
      {
         if ( IS_EXPRLIST( thisptr->p[0] ) )
         {
            h = thisptr->p[0];
            if ( ( h->p[0] == NULL ) || ( h->p[1] != NULL ) )
               exiterror( ERR_INVALID_EXPRESSION, 0 );
            thisptr->p[0] = h->p[0];
            RejectNode( h );
         }
         if ( IS_EXPRLIST( thisptr->p[1] ) )
         {
            h = thisptr->p[1];
            if ( ( h->p[0] == NULL ) || ( h->p[1] != NULL ) )
               exiterror( ERR_INVALID_EXPRESSION, 0 );
            thisptr->p[1] = h->p[0];
            RejectNode( h );
         }
      }
      return retval;
   }

   if ( !IS_EXPRLIST( thisptr ) )
      return retval;

   if ( ( mode == REDUCE_CALL ) || ( mode == REDUCE_RIGHT ) )
   {
      if ( IS_EXPRLIST( thisptr->p[0] ) && ( thisptr->p[1] == NULL ) )
      {
         retval = thisptr->p[0];
         RejectNode( thisptr );
      }
   }
   else
   {
      /*
       * mode == REDUCE_EXPR
       */
      if ( ( thisptr->p[0] != NULL ) && ( thisptr->p[1] == NULL ) )
      {
         if ( !IS_EXPRLIST( thisptr->p[0] ) )
         {
            retval = thisptr->p[0];
            RejectNode( thisptr );
         }
      }
   }
   return retval;
}

/*
 * optgluelast connect p2 as the ->next element to p1. Every element may be
 * NULL.
 * If both are non-NULL, the connection is performed using the o.last elements.
 * Just the o.last element of p1 remains non-NULL.
 *
 * Returns: NULL if p1 and p2 are NULL.
 *          The non-NULL element if one argumet is NULL.
 *          p1 otherwise.
 */
static nodeptr optgluelast( nodeptr p1, nodeptr p2 )
{
   nodeptr p2last;

   if ( p1 == NULL )
      return p2;
   if ( p2 == NULL )
      return p1;

   /*
    * This is performed very often, so keep the code fast.
    *
    * p2last is the "o.last"-element of p2 or just p2 if p2 has no next
    * elements. We set p1's o.last further down, but we have to ensure that
    * p2->o.last is NULL first. Therefore every element in the ->next chain
    * of p1 will have NULL as its o.last field.
    */
   if ( ( p2last = p2->o.last ) == NULL )
      p2last = p2;
   else
      p2->o.last = NULL;

   if ( p1->o.last == NULL )
      p1->next = p2;
   else
      p1->o.last->next = p2;
   p1->o.last = p2last;

   return p1;
}

/*
 * justlabels returns 1, if n consists of a sequence of labels. The return
 * value is 0 otherwise.
 */
static int justlabels( nodeptr n )
{
   while ( n != NULL )
   {
      if ( n->type != X_LABEL )
         return 0;
      n = n->next;
   }

   return 1;
}

/*
 * move_labels move the labels from the end of "end" to the end of "front".
 * The labels are marked "read_only" if level is nonnull, the read-only flag
 * is removed if level is 0.
 * NOTE: At least one element of the "end" chain must contain a non-label
 * element.
 */
static void move_labels( nodeptr front, nodeptr end, int level )
{
   nodeptr oend = end;
   nodeptr labels;

   assert( front != NULL );
   assert( !justlabels( end ) );

   while ( !justlabels( end->next ) )
      end = end->next;

   if ( ( labels = end->next ) == NULL )
      return;

   /*
    * extract the labels.
    */
   labels->o.last = oend->o.last;
   end->next = NULL;
   if ( end == oend )
      oend->o.last = NULL;
   else
      oend->o.last = end;

   if ( labels->next == NULL )
      labels->o.last = NULL;

   /*
    * add the labels to the end of front and then re-mark the labels.
    */
   optgluelast( front, labels );
   while ( labels ) {
      labels->u.trace_only = ( level == 0 ) ? 0 : 1;
      labels = labels->next;
   }
}

