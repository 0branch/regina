/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse __reginaparse
#define yylex   __reginalex
#define yyerror __reginaerror
#define yylval  __reginalval
#define yychar  __reginachar
#define yydebug __reginadebug
#define yynerrs __reginanerrs


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
     ERROR = 296,
     SYNTAX = 297,
     HALT = 298,
     NOVALUE = 299,
     TRACE = 300,
     END = 301,
     UPPER = 302,
     ASSIGNMENTVARIABLE = 303,
     STATSEP = 304,
     FOREVER = 305,
     DIGITS = 306,
     FORM = 307,
     FUZZ = 308,
     SCIENTIFIC = 309,
     ENGINEERING = 310,
     NOT = 311,
     CONCATENATE = 312,
     MODULUS = 313,
     GTE = 314,
     GT = 315,
     LTE = 316,
     LT = 317,
     DIFFERENT = 318,
     EQUALEQUAL = 319,
     NOTEQUALEQUAL = 320,
     OFFSET = 321,
     SPACE = 322,
     EXP = 323,
     XOR = 324,
     PLACEHOLDER = 325,
     NOTREADY = 326,
     CONSYMBOL = 327,
     SIMSYMBOL = 328,
     EXFUNCNAME = 329,
     INFUNCNAME = 330,
     LABEL = 331,
     DOVARIABLE = 332,
     HEXSTRING = 333,
     STRING = 334,
     VERSION = 335,
     LINEIN = 336,
     WHATEVER = 337,
     NAME = 338,
     FAILURE = 339,
     BINSTRING = 340,
     OPTIONS = 341,
     ENVIRONMENT = 342,
     LOSTDIGITS = 343,
     GTGT = 344,
     LTLT = 345,
     NOTGTGT = 346,
     NOTLTLT = 347,
     GTGTE = 348,
     LTLTE = 349,
     INPUT = 350,
     OUTPUT = 351,
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
     UPLUSS = 362,
     UMINUS = 363,
     SYNTOP = 364
   };
#endif
#define ADDRESS 258
#define ARG 259
#define CALL 260
#define DO 261
#define TO 262
#define BY 263
#define FOR 264
#define WHILE 265
#define UNTIL 266
#define EXIT 267
#define IF 268
#define THEN 269
#define ELSE 270
#define ITERATE 271
#define INTERPRET 272
#define LEAVE 273
#define NOP 274
#define NUMERIC 275
#define PARSE 276
#define EXTERNAL 277
#define SOURCE 278
#define VAR 279
#define VALUE 280
#define WITH 281
#define PROCEDURE 282
#define EXPOSE 283
#define PULL 284
#define PUSH 285
#define QUEUE 286
#define SAY 287
#define RETURN 288
#define SELECT 289
#define WHEN 290
#define DROP 291
#define OTHERWISE 292
#define SIGNAL 293
#define ON 294
#define OFF 295
#define ERROR 296
#define SYNTAX 297
#define HALT 298
#define NOVALUE 299
#define TRACE 300
#define END 301
#define UPPER 302
#define ASSIGNMENTVARIABLE 303
#define STATSEP 304
#define FOREVER 305
#define DIGITS 306
#define FORM 307
#define FUZZ 308
#define SCIENTIFIC 309
#define ENGINEERING 310
#define NOT 311
#define CONCATENATE 312
#define MODULUS 313
#define GTE 314
#define GT 315
#define LTE 316
#define LT 317
#define DIFFERENT 318
#define EQUALEQUAL 319
#define NOTEQUALEQUAL 320
#define OFFSET 321
#define SPACE 322
#define EXP 323
#define XOR 324
#define PLACEHOLDER 325
#define NOTREADY 326
#define CONSYMBOL 327
#define SIMSYMBOL 328
#define EXFUNCNAME 329
#define INFUNCNAME 330
#define LABEL 331
#define DOVARIABLE 332
#define HEXSTRING 333
#define STRING 334
#define VERSION 335
#define LINEIN 336
#define WHATEVER 337
#define NAME 338
#define FAILURE 339
#define BINSTRING 340
#define OPTIONS 341
#define ENVIRONMENT 342
#define LOSTDIGITS 343
#define GTGT 344
#define LTLT 345
#define NOTGTGT 346
#define NOTLTLT 347
#define GTGTE 348
#define LTLTE 349
#define INPUT 350
#define OUTPUT 351
#define NORMAL 352
#define APPEND 353
#define REPLACE 354
#define STREAM 355
#define STEM 356
#define LIFO 357
#define FIFO 358
#define LOWER 359
#define CASELESS 360
#define CCAT 361
#define UPLUSS 362
#define UMINUS 363
#define SYNTOP 364




/* Copy the first part of user declarations.  */
#line 1 "./yaccsrc.y"


#ifndef lint
static char *RCSid = "$Id: yaccsrc.c,v 1.32 2004/02/10 11:11:50 mark Exp $";
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

typedef enum { IS_UNKNOWN,
               IS_A_NUMBER,
               IS_NO_NUMBER,
               IS_SIM_SYMBOL,
               IS_COMP_SYMBOL } node_type;

typedef enum { REDUCE_CALL,
               REDUCE_EXPR,
               REDUCE_RIGHT,
               REDUCE_SUBEXPR } reduce_mode;

static node_type gettypeof( nodeptr this ) ;
static void checkconst( nodeptr this ) ;
static nodeptr reduce_expr_list( nodeptr this, reduce_mode mode );
static void transform( nodeptr this ) ;
static nodeptr create_head( const char *name ) ;
static nodeptr makenode( int type, int numb, ... ) ;
static void checkdosyntax( cnodeptr this ) ;
void newlabel( const tsd_t *TSD, internal_parser_type *ipt, nodeptr this ) ;
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
#line 148 "./yaccsrc.y"

#ifdef NDEBUG
# define YYDEBUG 0
#else
# define YYDEBUG 1
#endif


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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 436 "./yaccsrc.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2074

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  121
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  177
/* YYNRULES -- Number of rules. */
#define YYNRULES  446
/* YYNRULES -- Number of states. */
#define YYNSTATES  599

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   364

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   114,   107,     2,
     118,   119,   112,   110,   120,   111,     2,   113,     2,     2,
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
     105,   109,   115,   116,   117
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
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
     267,   269,   273,   280,   287,   293,   298,   302,   304,   307,
     309,   310,   312,   314,   316,   317,   320,   321,   324,   328,
     331,   333,   335,   337,   339,   342,   345,   348,   351,   355,
     358,   362,   364,   367,   371,   374,   378,   380,   383,   387,
     390,   394,   396,   401,   405,   410,   414,   419,   423,   425,
     427,   429,   432,   435,   438,   441,   443,   446,   449,   452,
     455,   458,   461,   464,   467,   468,   472,   473,   477,   479,
     480,   482,   484,   486,   488,   490,   493,   496,   499,   502,
     505,   506,   510,   513,   517,   520,   523,   530,   540,   550,
     557,   560,   565,   568,   571,   574,   577,   581,   583,   585,
     587,   590,   594,   596,   598,   601,   605,   608,   613,   617,
     620,   625,   629,   632,   636,   639,   641,   643,   646,   651,
     655,   659,   662,   664,   667,   670,   672,   675,   678,   680,
     684,   686,   688,   690,   692,   694,   696,   698,   701,   705,
     708,   710,   713,   717,   722,   726,   729,   732,   735,   738,
     741,   744,   748,   750,   756,   761,   766,   769,   773,   779,
     782,   784,   786,   793,   801,   804,   807,   809,   811,   814,
     818,   819,   823,   827,   830,   834,   838,   843,   849,   854,
     858,   863,   867,   869,   871,   874,   877,   879,   881,   883,
     885,   887,   889,   891,   893,   895,   897,   899,   901,   905,
     908,   912,   915,   917,   920,   922,   923,   927,   928,   933,
     934,   939,   940,   945,   947,   948,   953,   955,   956,   961,
     963,   964,   969,   971,   972,   977,   979,   980,   985,   987,
     988,   993,   995,   996,  1001,  1003,  1004,  1009,  1011,  1012,
    1017,  1019,  1022,  1024,  1025,  1030,  1032,  1033,  1038,  1040,
    1041,  1046,  1048,  1049,  1054,  1056,  1057,  1062,  1064,  1065,
    1070,  1072,  1073,  1078,  1080,  1081,  1086,  1088,  1089,  1094,
    1096,  1097,  1102,  1104,  1105,  1110,  1112,  1113,  1118,  1120,
    1121,  1126,  1128,  1129,  1134,  1136,  1138,  1140,  1142,  1144,
    1146,  1148,  1151,  1154,  1156,  1158,  1161,  1164,  1166,  1168,
    1171,  1174,  1175,  1178,  1180,  1182,  1186,  1188,  1190,  1193,
    1196,  1198,  1201,  1205,  1210,  1215,  1220,  1222,  1224,  1226,
    1228,  1230,  1232,  1235,  1238,  1239,  1240,  1245,  1247,  1249,
    1250,  1253,  1255,  1260,  1264,  1266,  1268
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     122,     0,    -1,    -1,   123,   124,    -1,   131,   125,    -1,
     131,    -1,   125,   127,    -1,   127,    -1,   126,   133,    -1,
     133,   134,    -1,   133,    -1,   128,    -1,   134,   177,   130,
      -1,   126,    -1,   134,    -1,   130,    49,   132,    -1,    49,
     132,    -1,   132,   130,    -1,   132,    -1,   132,   216,    -1,
      -1,   135,    -1,   238,    -1,    -1,   134,   136,    -1,   137,
     130,    -1,   211,    -1,   212,    -1,   213,    -1,   165,    -1,
     175,    -1,   169,    -1,   170,    -1,   178,    -1,   208,    -1,
     210,    -1,   214,    -1,   215,    -1,   218,    -1,   219,    -1,
     220,    -1,   222,    -1,   223,    -1,   227,    -1,   228,    -1,
     229,    -1,   230,    -1,   232,    -1,   231,    -1,   234,    -1,
     240,    -1,   248,    -1,   209,    -1,   250,    -1,     5,    -1,
       6,    -1,    12,    -1,    13,    -1,    16,    -1,    18,    -1,
      32,    -1,    33,    -1,     3,    -1,     4,    -1,    36,    -1,
      17,    -1,    76,    -1,    19,    -1,    20,    -1,    86,    -1,
      21,    -1,    27,    -1,    29,    -1,    30,    -1,    31,    -1,
      34,    -1,    38,    -1,    35,    -1,    37,    -1,    45,    -1,
      47,    -1,    -1,   146,   166,   167,    -1,    25,   252,   181,
      -1,   183,    -1,    -1,    -1,     1,   168,   181,    -1,   180,
     294,   181,    -1,   118,   252,   119,   201,   181,    -1,   147,
     225,    -1,    -1,   138,   173,   171,   174,    -1,    -1,   138,
     290,   172,   174,    -1,   138,   244,     1,    -1,   138,   245,
       1,    -1,   138,   244,   246,     1,    -1,   138,   244,   246,
     242,     1,    -1,   138,   244,   246,   242,    -1,   138,   244,
     246,    -1,   138,   245,   246,     1,    -1,   138,   245,   246,
      -1,   243,    -1,     1,    -1,   292,    -1,   292,   119,    -1,
     252,    -1,    46,    -1,   176,   297,    -1,   176,    -1,   176,
     297,     1,    -1,   139,   179,   207,   130,   129,   177,    -1,
     205,   108,   252,   206,   206,   206,    -1,   205,   108,   252,
     206,   206,    -1,   205,   108,   252,   206,    -1,   205,   108,
     252,    -1,    50,    -1,    50,     1,    -1,   252,    -1,    -1,
      72,    -1,    73,    -1,    79,    -1,    -1,   182,   183,    -1,
      -1,    26,   184,    -1,    26,   184,     1,    -1,    26,   201,
      -1,   185,    -1,   186,    -1,   187,    -1,     1,    -1,   191,
     188,    -1,   192,   189,    -1,   193,   190,    -1,   192,   201,
      -1,   192,   193,   201,    -1,   193,   201,    -1,   193,   192,
     201,    -1,   201,    -1,   191,   201,    -1,   191,   193,   201,
      -1,   193,   201,    -1,   193,   191,   201,    -1,   201,    -1,
     191,   201,    -1,   191,   192,   201,    -1,   192,   201,    -1,
     192,   191,   201,    -1,   201,    -1,   201,    95,   201,   194,
      -1,   201,    95,     1,    -1,   201,    96,   201,   195,    -1,
     201,    96,     1,    -1,   201,    41,   201,   195,    -1,   201,
      41,     1,    -1,   196,    -1,    97,    -1,   196,    -1,    98,
     196,    -1,    98,     1,    -1,    99,   196,    -1,    99,     1,
      -1,    97,    -1,   100,   199,    -1,   100,     1,    -1,   101,
     197,    -1,   101,     1,    -1,   102,   199,    -1,   102,     1,
      -1,   103,   199,    -1,   103,     1,    -1,    -1,   198,   201,
     203,    -1,    -1,   200,   201,   202,    -1,    67,    -1,    -1,
     203,    -1,   204,    -1,   296,    -1,   290,    -1,    77,    -1,
       7,   252,    -1,     9,   252,    -1,     8,   252,    -1,    10,
     252,    -1,    11,   252,    -1,    -1,   148,   295,     1,    -1,
     148,   295,    -1,   164,   295,     1,    -1,   164,   295,    -1,
     140,   294,    -1,   141,   252,   131,    14,   131,   127,    -1,
     141,   252,   131,    14,   131,   127,    15,   131,   127,    -1,
     141,   252,   131,    14,   131,   127,    15,   131,     1,    -1,
     141,   252,   131,    14,   131,     1,    -1,   141,   130,    -1,
     141,   252,   131,     1,    -1,   134,    14,    -1,   134,    15,
      -1,   149,   252,    -1,   142,   297,    -1,   142,   297,     1,
      -1,   142,    -1,   217,    -1,   150,    -1,   143,   297,    -1,
     143,   297,     1,    -1,   143,    -1,   151,    -1,   151,     1,
      -1,   152,    51,   252,    -1,   152,    51,    -1,   152,    52,
     221,     1,    -1,   152,    52,   221,    -1,   152,    52,    -1,
     152,    52,    25,   252,    -1,   152,    52,     1,    -1,   152,
      53,    -1,   152,    53,   252,    -1,   152,     1,    -1,    54,
      -1,    55,    -1,   153,   294,    -1,   154,   224,   226,   225,
      -1,   154,   226,   225,    -1,   154,   224,     1,    -1,   154,
       1,    -1,    47,    -1,    47,   105,    -1,   105,    47,    -1,
     104,    -1,   104,   105,    -1,   105,   104,    -1,   105,    -1,
     225,   120,   287,    -1,   287,    -1,     4,    -1,    81,    -1,
      22,    -1,    80,    -1,    29,    -1,    23,    -1,    24,   297,
      -1,    25,   294,    26,    -1,    25,     1,    -1,   155,    -1,
     155,     1,    -1,   155,    28,     1,    -1,   155,    28,   295,
       1,    -1,   155,    28,   295,    -1,   156,   287,    -1,   157,
     294,    -1,   158,   294,    -1,   144,   294,    -1,   145,   294,
      -1,    46,   297,    -1,    46,   297,     1,    -1,    46,    -1,
     159,   130,   235,   239,   233,    -1,   159,   130,    46,     1,
      -1,   159,   130,   162,     1,    -1,   159,     1,    -1,   159,
     130,    14,    -1,   159,   130,   235,   162,     1,    -1,   235,
     236,    -1,   236,    -1,     1,    -1,   161,   252,   131,    14,
     131,   133,    -1,   161,   252,   131,    14,   131,   133,    14,
      -1,   161,   252,    -1,   161,     1,    -1,   161,    -1,   162,
      -1,   134,   237,    -1,   162,   131,   129,    -1,    -1,   160,
      25,   252,    -1,   160,   241,     1,    -1,   160,   241,    -1,
     160,   244,     1,    -1,   160,   245,     1,    -1,   160,   244,
     247,     1,    -1,   160,   244,   247,   242,     1,    -1,   160,
     244,   247,   242,    -1,   160,   244,   247,    -1,   160,   245,
     247,     1,    -1,   160,   245,   247,    -1,   243,    -1,     1,
      -1,    83,    73,    -1,    83,     1,    -1,    72,    -1,    73,
      -1,    39,    -1,    40,    -1,    41,    -1,    43,    -1,    71,
      -1,    84,    -1,   246,    -1,    44,    -1,    42,    -1,    88,
      -1,   163,    25,   252,    -1,   163,   252,    -1,   163,   249,
       1,    -1,   163,   249,    -1,    82,    -1,   251,   294,    -1,
      48,    -1,    -1,   118,   253,   280,    -1,    -1,   252,   110,
     254,   252,    -1,    -1,   252,   111,   255,   252,    -1,    -1,
     252,   112,   256,   252,    -1,   112,    -1,    -1,   252,   113,
     257,   252,    -1,   113,    -1,    -1,   252,    58,   258,   252,
      -1,    58,    -1,    -1,   252,   114,   259,   252,    -1,   114,
      -1,    -1,   252,   106,   260,   252,    -1,   106,    -1,    -1,
     252,   107,   261,   252,    -1,   107,    -1,    -1,   252,    69,
     262,   252,    -1,    69,    -1,    -1,   252,    68,   263,   252,
      -1,    68,    -1,    -1,   252,    67,   264,   252,    -1,    67,
      -1,    -1,   252,    57,   265,   252,    -1,    57,    -1,    56,
     252,    -1,    56,    -1,    -1,   252,   108,   266,   252,    -1,
     108,    -1,    -1,   252,    59,   267,   252,    -1,    59,    -1,
      -1,   252,    61,   268,   252,    -1,    61,    -1,    -1,   252,
      60,   269,   252,    -1,    60,    -1,    -1,   252,    62,   270,
     252,    -1,    62,    -1,    -1,   252,    63,   271,   252,    -1,
      63,    -1,    -1,   252,    64,   272,   252,    -1,    64,    -1,
      -1,   252,    65,   273,   252,    -1,    65,    -1,    -1,   252,
      89,   274,   252,    -1,    89,    -1,    -1,   252,    90,   275,
     252,    -1,    90,    -1,    -1,   252,    91,   276,   252,    -1,
      91,    -1,    -1,   252,    92,   277,   252,    -1,    92,    -1,
      -1,   252,    93,   278,   252,    -1,    93,    -1,    -1,   252,
      94,   279,   252,    -1,    94,    -1,   281,    -1,    72,    -1,
      78,    -1,    85,    -1,    79,    -1,   282,    -1,   110,   252,
      -1,   111,   252,    -1,   110,    -1,   111,    -1,   292,   119,
      -1,   292,     1,    -1,    49,    -1,    73,    -1,   286,   283,
      -1,   285,   283,    -1,    -1,   284,   280,    -1,    75,    -1,
      74,    -1,   291,   288,   287,    -1,   291,    -1,     1,    -1,
     111,   289,    -1,   110,   289,    -1,   289,    -1,   108,   289,
      -1,   118,   281,   119,    -1,   111,   118,   281,   119,    -1,
     110,   118,   281,   119,    -1,   108,   118,   281,   119,    -1,
     290,    -1,    66,    -1,    72,    -1,    79,    -1,    78,    -1,
      85,    -1,    70,   291,    -1,   281,   291,    -1,    -1,    -1,
     294,   120,   293,   292,    -1,   294,    -1,   252,    -1,    -1,
     296,   295,    -1,   296,    -1,   118,   296,   119,   295,    -1,
     118,   296,   119,    -1,    73,    -1,    73,    -1,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   158,   158,   158,   168,   171,   177,   179,   182,   184,
     187,   188,   191,   196,   197,   200,   201,   204,   205,   208,
     209,   212,   213,   216,   220,   223,   224,   225,   226,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   256,   260,   265,   269,   274,   278,
     282,   286,   290,   294,   298,   302,   306,   310,   314,   318,
     322,   326,   330,   334,   338,   342,   347,   351,   355,   359,
     363,   367,   367,   371,   374,   375,   376,   376,   378,   382,
     388,   394,   394,   399,   399,   405,   406,   407,   408,   409,
     415,   420,   421,   427,   428,   431,   439,   442,   448,   454,
     456,   457,   460,   481,   485,   489,   492,   495,   496,   497,
     499,   502,   503,   504,   507,   507,   513,   516,   517,   518,
     521,   522,   523,   524,   527,   531,   535,   539,   540,   541,
     542,   543,   546,   547,   548,   549,   550,   553,   554,   555,
     556,   557,   560,   561,   564,   565,   568,   569,   572,   573,
     578,   579,   581,   582,   583,   584,   589,   597,   598,   611,
     612,   616,   617,   621,   624,   624,   628,   628,   632,   633,
     636,   637,   640,   648,   656,   659,   660,   661,   664,   665,
     666,   669,   670,   674,   675,   679,   683,   691,   701,   703,
     705,   706,   709,   712,   715,   720,   722,   723,   726,   734,
     738,   740,   741,   744,   745,   748,   751,   752,   753,   756,
     758,   760,   761,   762,   765,   768,   769,   772,   775,   781,
     787,   788,   791,   793,   795,   797,   799,   801,   803,   807,
     809,   812,   813,   814,   815,   816,   817,   818,   820,   821,
     824,   825,   826,   827,   828,   832,   836,   840,   844,   848,
     852,   853,   854,   857,   862,   863,   865,   866,   867,   871,
     872,   873,   876,   882,   884,   885,   888,   889,   892,   895,
     899,   905,   908,   909,   911,   912,   913,   914,   915,   921,
     925,   926,   932,   933,   936,   937,   940,   941,   944,   947,
     950,   951,   952,   953,   956,   957,   958,   959,   962,   964,
     966,   967,   971,   975,   991,   998,   998,  1031,  1031,  1034,
    1034,  1037,  1037,  1040,  1041,  1041,  1044,  1045,  1045,  1048,
    1049,  1049,  1052,  1053,  1053,  1056,  1057,  1057,  1060,  1061,
    1061,  1064,  1065,  1065,  1068,  1069,  1069,  1072,  1073,  1073,
    1076,  1077,  1079,  1080,  1080,  1084,  1085,  1085,  1089,  1090,
    1090,  1094,  1095,  1095,  1099,  1100,  1100,  1104,  1105,  1105,
    1109,  1110,  1110,  1113,  1114,  1114,  1117,  1118,  1118,  1121,
    1122,  1122,  1125,  1126,  1126,  1129,  1130,  1130,  1133,  1134,
    1134,  1137,  1138,  1138,  1141,  1142,  1143,  1145,  1149,  1153,
    1155,  1156,  1158,  1160,  1161,  1164,  1165,  1166,  1169,  1173,
    1175,  1179,  1179,  1186,  1189,  1192,  1193,  1194,  1197,  1199,
    1201,  1203,  1205,  1207,  1209,  1211,  1213,  1217,  1218,  1227,
    1228,  1232,  1238,  1239,  1240,  1250,  1250,  1325,  1369,  1370,
    1373,  1375,  1377,  1379,  1383,  1386,  1387
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ADDRESS", "ARG", "CALL", "DO", "TO", "BY", 
  "FOR", "WHILE", "UNTIL", "EXIT", "IF", "THEN", "ELSE", "ITERATE", 
  "INTERPRET", "LEAVE", "NOP", "NUMERIC", "PARSE", "EXTERNAL", "SOURCE", 
  "VAR", "VALUE", "WITH", "PROCEDURE", "EXPOSE", "PULL", "PUSH", "QUEUE", 
  "SAY", "RETURN", "SELECT", "WHEN", "DROP", "OTHERWISE", "SIGNAL", "ON", 
  "OFF", "ERROR", "SYNTAX", "HALT", "NOVALUE", "TRACE", "END", "UPPER", 
  "ASSIGNMENTVARIABLE", "STATSEP", "FOREVER", "DIGITS", "FORM", "FUZZ", 
  "SCIENTIFIC", "ENGINEERING", "NOT", "CONCATENATE", "MODULUS", "GTE", 
  "GT", "LTE", "LT", "DIFFERENT", "EQUALEQUAL", "NOTEQUALEQUAL", "OFFSET", 
  "SPACE", "EXP", "XOR", "PLACEHOLDER", "NOTREADY", "CONSYMBOL", 
  "SIMSYMBOL", "EXFUNCNAME", "INFUNCNAME", "LABEL", "DOVARIABLE", 
  "HEXSTRING", "STRING", "VERSION", "LINEIN", "WHATEVER", "NAME", 
  "FAILURE", "BINSTRING", "OPTIONS", "ENVIRONMENT", "LOSTDIGITS", "GTGT", 
  "LTLT", "NOTGTGT", "NOTLTLT", "GTGTE", "LTLTE", "INPUT", "OUTPUT", 
  "NORMAL", "APPEND", "REPLACE", "STREAM", "STEM", "LIFO", "FIFO", 
  "LOWER", "CASELESS", "'|'", "'&'", "'='", "CCAT", "'+'", "'-'", "'*'", 
  "'/'", "'%'", "UPLUSS", "UMINUS", "SYNTOP", "'('", "')'", "','", 
  "$accept", "start", "@1", "prog", "stats", "xstats", "ystatement", 
  "lonely_end", "nxstats", "ncl", "nlncl", "optLabels", "statement", 
  "gruff", "mttstatement", "mtstatement", "nclstatement", "call", "do", 
  "exit", "if", "iterate", "leave", "say", "return", "address", "arg", 
  "drop", "interpret", "label", "nop", "numeric", "options", "parse", 
  "proc", "pull", "push", "queue", "select", "signal", "when", 
  "otherwise", "trace", "upper", "address_stat", "@2", "address_stat2", 
  "@3", "arg_stat", "call_stat", "@4", "@5", "call_name", "call_args", 
  "expr_stat", "end_stat", "end", "do_stat", "repetitor", "nvir", 
  "naddr_with", "@6", "addr_with", "connection", "inputstmts", 
  "outputstmts", "errorstmts", "adeo", "adei", "adio", "inputstmt", 
  "outputstmt", "errorstmt", "resourcei", "resourceo", "resources", 
  "nsimsymb", "@7", "nnvir", "@8", "nspace", "addrAll", "addrSim", 
  "addrString", "dovar", "tobyfor", "conditional", "drop_stat", 
  "upper_stat", "exit_stat", "if_stat", "unexp_then", "unexp_else", 
  "ipret_stat", "iterate_stat", "label_stat", "labelname", "leave_stat", 
  "nop_stat", "numeric_stat", "form_expr", "options_stat", "parse_stat", 
  "parse_flags", "templs", "parse_param", "proc_stat", "pull_stat", 
  "push_stat", "queue_stat", "say_stat", "return_stat", "sel_end", 
  "select_stat", "when_stats", "when_stat", "when_or_other", 
  "ex_when_stat", "otherwise_stat", "signal_stat", "signal_name", 
  "namespec", "asymbol", "on", "off", "c_action", "s_action", 
  "trace_stat", "whatever", "assignment", "ass_part", "expr", "@9", "@10", 
  "@11", "@12", "@13", "@14", "@15", "@16", "@17", "@18", "@19", "@20", 
  "@21", "@22", "@23", "@24", "@25", "@26", "@27", "@28", "@29", "@30", 
  "@31", "@32", "@33", "@34", "@35", "exprs_sub", "symbtree", "function", 
  "func_args", "@36", "intfunc", "extfunc", "template", "solid", "offset", 
  "string", "pv", "exprs", "@37", "nexpr", "anyvars", "xsimsymb", 
  "simsymb", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
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
      43,    45,    42,    47,    37,   362,   363,   364,    40,    41,
      44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   121,   123,   122,   124,   124,   125,   125,   126,   126,
     127,   127,   128,   129,   129,   130,   130,   131,   131,   132,
     132,   133,   133,   134,   135,   136,   136,   136,   136,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   166,   165,   167,   167,   167,   168,   167,   167,   167,
     169,   171,   170,   172,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   173,   173,   174,   174,   175,   176,   177,
     177,   177,   178,   179,   179,   179,   179,   179,   179,   179,
     179,   180,   180,   180,   182,   181,   181,   183,   183,   183,
     184,   184,   184,   184,   185,   186,   187,   188,   188,   188,
     188,   188,   189,   189,   189,   189,   189,   190,   190,   190,
     190,   190,   191,   191,   192,   192,   193,   193,   194,   194,
     195,   195,   195,   195,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   198,   197,   200,   199,   201,   201,
     202,   202,   203,   204,   205,   206,   206,   206,   207,   207,
     207,   208,   208,   209,   209,   210,   211,   211,   211,   211,
     211,   211,   212,   213,   214,   215,   215,   215,   216,   217,
     218,   218,   218,   219,   219,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   221,   221,   222,   223,   223,
     223,   223,   224,   224,   224,   224,   224,   224,   224,   225,
     225,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   227,   227,   227,   228,   229,   230,   231,   232,
     233,   233,   233,   234,   234,   234,   234,   234,   234,   235,
     235,   235,   236,   236,   236,   236,   237,   237,   238,   239,
     239,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   241,   241,   242,   242,   243,   243,   244,   245,
     246,   246,   246,   246,   247,   247,   247,   247,   248,   248,
     248,   248,   249,   250,   251,   253,   252,   254,   252,   255,
     252,   256,   252,   252,   257,   252,   252,   258,   252,   252,
     259,   252,   252,   260,   252,   252,   261,   252,   252,   262,
     252,   252,   263,   252,   252,   264,   252,   252,   265,   252,
     252,   252,   252,   266,   252,   252,   267,   252,   252,   268,
     252,   252,   269,   252,   252,   270,   252,   252,   271,   252,
     252,   272,   252,   252,   273,   252,   252,   274,   252,   252,
     275,   252,   252,   276,   252,   252,   277,   252,   252,   278,
     252,   252,   279,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   280,   280,   280,   281,   282,
     282,   284,   283,   285,   286,   287,   287,   287,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   289,   289,   290,
     290,   290,   291,   291,   291,   293,   292,   292,   294,   294,
     295,   295,   295,   295,   296,   297,   297
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
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
       1,     3,     6,     6,     5,     4,     3,     1,     2,     1,
       0,     1,     1,     1,     0,     2,     0,     2,     3,     2,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     2,
       3,     1,     2,     3,     2,     3,     1,     2,     3,     2,
       3,     1,     4,     3,     4,     3,     4,     3,     1,     1,
       1,     2,     2,     2,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     3,     0,     3,     1,     0,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       0,     3,     2,     3,     2,     2,     6,     9,     9,     6,
       2,     4,     2,     2,     2,     2,     3,     1,     1,     1,
       2,     3,     1,     1,     2,     3,     2,     4,     3,     2,
       4,     3,     2,     3,     2,     1,     1,     2,     4,     3,
       3,     2,     1,     2,     2,     1,     2,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     2,
       1,     2,     3,     4,     3,     2,     2,     2,     2,     2,
       2,     3,     1,     5,     4,     4,     2,     3,     5,     2,
       1,     1,     6,     7,     2,     2,     1,     1,     2,     3,
       0,     3,     3,     2,     3,     3,     4,     5,     4,     3,
       4,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       3,     2,     1,     2,     1,     0,     3,     0,     4,     0,
       4,     0,     4,     1,     0,     4,     1,     0,     4,     1,
       0,     4,     1,     0,     4,     1,     0,     4,     1,     0,
       4,     1,     0,     4,     1,     0,     4,     1,     0,     4,
       1,     2,     1,     0,     4,     1,     0,     4,     1,     0,
       4,     1,     0,     4,     1,     0,     4,     1,     0,     4,
       1,     0,     4,     1,     0,     4,     1,     0,     4,     1,
       0,     4,     1,     0,     4,     1,     0,     4,     1,     0,
       4,     1,     0,     4,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     2,     2,     1,     1,     2,
       2,     0,     2,     1,     1,     3,     1,     1,     2,     2,
       1,     2,     3,     4,     4,     4,     1,     1,     1,     1,
       1,     1,     2,     2,     0,     0,     4,     1,     1,     0,
       2,     1,     4,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       2,     0,    20,     1,     3,    23,    18,    23,     7,    11,
      10,    23,    21,    22,    20,    66,    17,   209,    19,   208,
       6,    62,    63,    54,    55,    56,    57,    58,    65,    59,
      67,    68,    70,    71,    72,    73,    74,    60,    61,    75,
      77,    64,    78,    76,    79,   108,    80,   314,   352,   350,
     329,   358,   364,   361,   367,   370,   373,   376,   347,   344,
     341,   396,   408,   414,   413,   397,   399,   398,    69,   379,
     382,   385,   388,   391,   394,   335,   338,   355,   403,   404,
     323,   326,   332,   315,     0,    24,     0,     0,   120,   439,
       0,     0,     0,   439,   439,    81,     0,     0,     0,     0,
       0,   439,     0,     0,     0,   439,   439,     0,     0,   276,
     277,     0,     0,    29,    31,    32,    30,     0,     0,    33,
      34,    52,    35,    26,    27,    28,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    48,    47,    49,
     278,    50,    51,    53,   439,   107,   395,   400,   411,   411,
      16,    20,   351,   401,   402,   439,   202,   203,    25,   104,
     298,   299,   296,   297,   430,   429,   431,    91,   103,     0,
       0,    93,     0,   184,   190,     0,   119,   438,   195,   200,
      20,   446,   445,     0,     0,   258,   259,     0,   417,   434,
      90,   434,   240,   416,   444,     0,     0,   441,   204,   214,
     224,   216,     0,   222,   227,   231,   241,   243,   246,     0,
       0,   245,   232,   244,   242,   235,   238,     0,     0,   251,
       0,   255,   256,   257,   266,     0,   293,     0,     0,   292,
       0,     0,     0,   312,     0,   309,     0,     0,    12,   313,
     348,   327,   356,   362,   359,   365,   368,   371,   374,   345,
     342,   339,   377,   380,   383,   386,   389,   392,   333,   336,
     353,   317,   319,   321,   324,   330,   410,   439,   409,    15,
     407,   316,     0,   437,   439,    95,   300,   301,   302,   303,
       0,    96,     0,   439,   118,     0,     0,     0,     0,     0,
     206,   211,    86,     0,     0,   121,   122,   123,     0,    82,
     439,    84,   432,     0,   433,   427,   428,     0,     0,     0,
       0,     0,   420,   426,     0,   191,   440,   215,   221,     0,
     225,   226,     0,   223,   247,   249,     0,   233,   236,   234,
     237,   230,     0,   229,   252,     0,   271,   267,     0,     0,
       0,   280,   270,   281,   282,   284,   306,   305,   307,   304,
       0,   285,     0,   308,   310,   193,   111,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   412,   406,   405,   435,    92,   105,    97,
       0,     0,   101,    94,   188,   189,    23,   116,   201,    20,
     124,   124,   133,   178,     0,   130,   131,   132,   179,   179,
     179,   129,     0,   124,   239,     0,   421,     0,   419,     0,
     418,     0,   415,   443,   220,   217,   248,   228,   253,   264,
     275,    20,   265,     0,   269,     0,   286,     0,   290,   349,
     328,   357,   363,   360,   366,   369,   372,   375,   346,   343,
     340,   378,   381,   384,   387,   390,   393,   334,   337,   354,
     318,   320,   322,   325,   331,   439,   106,   295,   294,    98,
      23,     0,    23,    23,     0,     0,     0,   115,     0,    87,
       0,    83,   128,   134,   179,   179,   141,   135,   179,   179,
     146,   136,   179,   179,   151,     0,     0,     0,   179,    88,
       0,     0,     0,   422,   442,     0,   268,    23,     0,   263,
     287,   436,     8,    23,   112,     9,   185,   187,   186,   114,
     199,   196,   125,   179,   137,   179,   139,   179,   142,   179,
     144,   179,   147,   179,   149,   157,     0,   153,     0,   155,
       0,   124,   425,   424,   423,    20,   279,     0,   113,    20,
     138,   140,   143,   145,   148,   150,   165,     0,     0,     0,
       0,     0,     0,   156,   160,   159,   152,   158,   154,    89,
      23,   261,     0,   162,   161,   164,   163,   167,   166,   179,
     169,   168,   179,   171,   170,   173,   172,   272,   198,   197,
       0,     0,   273,   177,   180,   181,   183,   182,   175
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,     4,     7,   470,     8,     9,   471,    16,
       5,     6,    10,    11,    12,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    17,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   187,   299,   400,   114,   115,
     274,   283,   167,   387,   116,   117,   118,   119,   174,   300,
     479,   480,   301,   404,   405,   406,   407,   483,   487,   491,
     408,   409,   410,   566,   563,   564,   581,   582,   578,   579,
     411,   593,   594,   595,   175,   477,   287,   120,   121,   122,
     123,   124,   125,   126,   127,    18,    19,   128,   129,   130,
     322,   131,   132,   217,   190,   218,   133,   134,   135,   136,
     137,   138,   509,   139,   341,   342,   140,    13,   435,   141,
     228,   391,   168,   169,   170,   349,   350,   142,   234,   143,
     144,   177,   155,   378,   379,   380,   381,   358,   382,   375,
     376,   368,   367,   366,   357,   377,   359,   361,   360,   362,
     363,   364,   365,   369,   370,   371,   372,   373,   374,   271,
     146,   147,   266,   267,   148,   149,   192,   311,   312,   171,
     193,   272,   465,   273,   196,   197,   183
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -430
static const short yypact[] =
{
    -430,    71,  -430,  -430,  -430,    82,    25,   154,  -430,  -430,
    -430,   992,  -430,  -430,  -430,  -430,   120,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,   650,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,   650,   650,
    -430,  -430,  -430,  -430,   252,  -430,   156,   242,  1541,  1666,
    1410,    35,    39,  1666,  1666,  -430,   160,    83,  1666,    62,
     292,  1666,   404,    44,   180,  1666,  1666,   246,   307,  -430,
    -430,  1287,    83,  -430,  -430,  -430,  -430,   188,   156,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  1666,  1791,  -430,  -430,  -430,  -430,
     135,  -430,  -430,  -430,  -430,  1476,  -430,  -430,   120,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,   165,
     411,  -430,   406,  -430,   169,    50,  1791,  1791,  -430,   120,
    1791,  -430,  -430,   262,   263,  -430,  -430,    31,  -430,    -6,
      94,    -6,  -430,  1956,  -430,   149,   284,    83,  1791,  -430,
    -430,  1666,   452,  1666,  -430,  -430,  -430,  -430,  -430,    40,
     811,  -430,   126,  -430,  -430,   139,    -5,   586,   160,  -430,
      33,  -430,  -430,  -430,  -430,  1388,  -430,  1666,   335,  -430,
      34,   405,  1666,  -430,   336,  1791,   381,   382,   120,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  1476,  -430,   135,
    -430,  -430,    29,   105,  1666,  -430,  -430,  -430,  -430,  -430,
      24,  -430,   386,  1666,  -430,  1666,  1666,   156,  1666,    48,
    -430,  -430,  -430,  1666,    95,  -430,  -430,  -430,  1666,  -430,
    1666,  -430,  -430,   160,  -430,  -430,  -430,   168,   176,   259,
     178,   160,  -430,  -430,   187,  -430,  -430,  1791,  -430,  1666,
    -430,  -430,   408,  1791,  -430,  -430,   261,  -430,  -430,  -430,
    -430,  -430,   160,    94,  -430,   409,  -430,  -430,   296,   876,
     308,   140,  -430,  1791,  -430,  -430,  -430,  -430,  -430,  -430,
      45,  -430,   410,  1791,  -430,  -430,  -430,  1666,  1666,  1666,
    1666,  1666,  1666,  1666,  1666,  1666,  1666,  1666,  1666,  1666,
    1666,  1666,  1666,  1666,  1666,  1666,  1666,  1666,  1666,  1666,
    1666,  1666,  1666,  -430,  -430,  -430,  -430,  -430,   194,  -430,
      46,   412,  -430,  -430,  1791,  1791,   120,   331,  -430,  -430,
     267,  1607,  -430,  -430,   439,  -430,  -430,  -430,   255,   255,
     255,    51,  1728,   267,  -430,   178,  -430,   178,  -430,   178,
    -430,   278,  -430,    83,  1791,  -430,  -430,    94,  -430,  -430,
    -430,  1350,  -430,   510,  -430,   358,  -430,   468,  -430,   523,
     274,  1861,  1861,  1861,  1861,  1861,  1861,  1861,   523,  -430,
    1849,  1861,  1861,  1861,  1861,  1861,  1861,  1849,  1919,  1861,
     142,   142,   274,   274,   274,  1666,  -430,  -430,  -430,  -430,
     369,   372,  -430,  1108,  1666,  1666,  1666,    43,   626,  -430,
     355,  -430,  -430,  -430,   255,   255,    20,  -430,   255,   255,
     109,  -430,   255,   255,   223,    85,    96,   202,   255,  -430,
     313,   315,   317,  -430,  -430,   436,  -430,  -430,   234,  -430,
    -430,  -430,  -430,  1224,  -430,  -430,  1791,  1791,  1791,    43,
    -430,   441,  -430,   255,   427,   255,   378,   255,   427,   255,
     380,   255,   378,   255,   380,  -430,   578,  -430,   694,  -430,
     578,   267,  -430,  -430,  -430,  -430,  -430,   469,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,    23,   172,    86,
      42,   256,   413,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,   742,  -430,  -430,  -430,  -430,  -430,  -430,   255,
    -430,  -430,   255,  -430,  -430,  -430,  -430,   464,  -430,  -430,
     534,   149,  -430,  -430,  -430,  -430,  -430,  -430,  -430
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -430,  -430,  -430,  -430,  -430,  -430,    -7,  -430,   -28,   -74,
    -171,     4,  -375,   -10,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -206,  -202,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,   198,  -430,  -430,    12,  -430,  -430,  -430,
    -384,  -430,     7,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -372,  -325,  -192,  -430,   -50,   -38,  -430,  -430,  -186,  -430,
    -355,  -430,   -97,  -430,  -430,  -429,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -187,   279,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,   158,  -430,  -430,  -430,  -430,
    -430,   152,   397,   402,   424,   303,   281,  -430,  -430,  -430,
    -430,    -9,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,  -430,   254,
     -91,  -430,   384,  -430,  -430,  -430,   -96,  -430,  -249,  -189,
    -123,  -263,  -430,   -79,  -106,  -188,   -89
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -440
static const short yytable[] =
{
      20,    84,   145,   184,   313,   191,   236,   314,   221,   289,
     178,   388,   158,   191,   185,   186,   179,   481,   150,   339,
     388,   472,   204,   340,   573,   389,   222,   223,   237,   499,
     384,   333,   292,   225,   334,   345,   181,   488,   492,   152,
     181,   181,   329,   580,   238,   219,   436,   467,   519,   398,
     474,   475,   476,   486,   490,   494,   293,   294,   416,   418,
     420,   495,   399,   199,   189,   239,   302,    62,   304,   153,
     154,     3,   220,  -100,    14,   276,   346,   277,   347,   176,
     -85,   180,    -5,   484,  -207,   493,   535,   577,  -212,   198,
     548,   316,   495,  -250,  -289,   512,   402,   537,   191,   330,
     191,    15,   235,   295,   296,   278,   194,   390,   182,  -174,
     297,  -213,   182,   182,   335,  -174,   497,   529,   279,   468,
     324,   533,   348,   559,   560,   561,   562,   191,   390,   524,
     526,   326,   472,   528,   530,   339,  -179,   532,   534,   433,
     536,   538,   540,   541,  -179,   427,   496,   497,   385,   298,
     495,   195,   403,  -176,    -4,   269,   194,   569,   288,  -176,
     525,   188,   403,   403,  -176,  -176,   275,   531,   550,   151,
     551,  -176,   552,   575,   553,    40,   554,    42,   555,   285,
     286,   188,  -179,  -179,  -179,  -179,  -179,  -179,  -179,   181,
    -179,  -179,   317,  -179,   323,   587,  -179,  -179,  -179,  -179,
     241,   195,   511,   539,   496,    14,   276,   414,   277,  -434,
     250,    15,   191,   396,   303,   422,   485,   489,   343,   421,
     191,   413,   194,   353,   590,   386,  -434,   591,   478,  -434,
     189,   327,  -434,    62,   305,   181,   278,  -110,  -434,  -434,
     306,   191,   305,   159,   328,  -434,  -434,   224,   306,   279,
     189,    62,  -434,    62,   263,   264,   265,   583,  -434,  -434,
     505,   182,   507,   290,   291,  -434,   156,   157,  -434,   403,
    -434,  -434,   559,   560,   561,   562,   394,   395,  -434,   397,
    -434,   160,   161,  -262,   401,   315,   415,   426,  -434,   412,
    -434,  -434,   523,   200,   417,    14,   527,   429,  -434,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,   423,   182,   226,   432,
     424,  -205,  -210,   466,   162,   163,  -126,   504,   496,   497,
     164,   165,   403,  -176,   500,   305,   501,   166,   502,  -176,
     431,   306,   227,  -192,  -176,  -176,   344,   354,   474,   475,
     476,  -176,   250,   201,   202,   203,   160,   161,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   570,   584,   586,   419,   572,   162,
     163,   294,   355,   356,  -283,  -311,   473,   392,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   503,   249,   250,
     251,   596,   597,   597,   508,   205,   351,   284,   206,   425,
     428,   438,   281,   469,   585,   -13,  -117,  -117,    45,   547,
     252,   253,   254,   255,   256,   257,   207,   208,   209,   210,
    -194,  -109,   542,   211,   543,  -102,   544,   258,   259,   260,
     482,   261,   262,   263,   264,   265,   276,   346,   277,   347,
     545,   212,   276,   318,   277,  -117,   549,  -218,  -254,  -291,
     513,   -99,   515,    84,   145,   516,   517,   518,   495,   510,
     571,   521,   280,   282,   497,   496,   278,   319,   592,   546,
    -176,   393,   278,   514,   213,   214,  -176,   522,  -127,   279,
     568,  -176,  -176,   348,   598,   279,   332,   473,  -176,   434,
     567,  -219,   437,    84,   145,   229,   320,   321,   215,   216,
     230,   506,   352,   -20,   -20,   -20,   -20,  -288,  -260,   574,
     576,   383,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
     -20,   -20,   231,   268,     0,     0,     0,   -20,     0,   -20,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,     0,
       0,     0,     0,     0,     0,   -20,   -20,   -20,   -20,   -20,
     513,     0,     0,     0,     0,   589,   -20,   -20,   -20,   -20,
     -20,   -20,   -20,   -20,   -20,   -20,     0,   -20,   -20,   -20,
       0,   241,   -20,   -20,   -20,   -20,   -20,   331,   -20,   -20,
     206,   250,     0,     0,     0,   -20,   -20,     0,     0,   -20,
     -20,   -20,   -20,   -20,   -20,     0,     0,   194,   207,   208,
     209,   210,   164,   165,     0,   211,   -20,   -20,   -20,   166,
     -20,   -20,   -20,   -20,   -20,     0,     0,   520,   -20,   -23,
     -23,   -23,   -23,   261,   262,   263,   264,   265,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,     0,     0,
       0,     0,     0,   -23,     0,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,     0,   213,   214,     0,     0,
       0,   -23,   -23,   -23,   -23,   556,   557,   558,   559,   560,
     561,   562,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,     0,   -23,   -23,   -23,     0,     0,   -23,   -23,
     -23,   -23,     0,     0,   -23,   -23,    48,     0,     0,     0,
       0,   -23,   -23,     0,     0,   -23,   -23,   -23,   -23,   -23,
     -23,     0,    61,    62,    63,    64,     0,     0,    65,    66,
       0,     0,   -23,   -23,   -23,    67,   -23,   -23,   -23,   -23,
     -23,     0,     0,   588,   -23,   -23,   -23,   -23,   -23,     0,
       0,     0,     0,     0,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,     0,     0,     0,     0,    83,   -23,
       0,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,     0,     0,     0,     0,     0,     0,   -23,   -23,   -23,
     -23,   565,     0,     0,   559,   560,   561,   562,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,     0,   -23,
     -23,   -23,   325,     0,   -23,   -23,   -23,   -23,     0,     0,
     -23,   -23,     0,     0,     0,     0,     0,   -23,   -23,     0,
       0,   -23,   -23,   -23,   -23,   -23,   -23,  -439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -23,   -23,
     -23,     0,   -23,   -23,   -23,   -23,   -23,     0,     0,     0,
     -23,     0,     0,     0,     0,     0,     0,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,   430,    58,    59,
      60,     0,     0,    61,    62,    63,    64,     0,     0,    65,
      66,     0,     0,     0,     0,     0,    67,     0,     0,     0,
      69,    70,    71,    72,    73,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,     0,    83,
       0,     0,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,    58,    59,    60,     0,     0,    61,    62,
      63,    64,     0,     0,    65,    66,     0,     0,     0,     0,
       0,    67,     0,     0,     0,    69,    70,    71,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,     0,    83,    21,    22,    23,    24,     0,
       0,     0,     0,     0,    25,    26,     0,     0,    27,    28,
      29,    30,    31,    32,     0,     0,     0,     0,     0,    33,
       0,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,     0,     0,     0,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,     0,    58,
      59,    60,     0,     0,    61,    62,    63,    64,     0,     0,
      65,    66,     0,     0,     0,     0,     0,    67,    68,     0,
       0,    69,    70,    71,    72,    73,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,     0,    78,    79,    80,    81,    82,     0,     0,     0,
      83,    21,    22,    23,    24,     0,     0,     0,     0,     0,
      25,    26,     0,     0,    27,    28,    29,    30,    31,    32,
       0,     0,     0,     0,     0,    33,     0,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     0,     0,     0,
       0,     0,     0,    44,   -14,    46,    47,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     0,    58,    59,    60,     0,     0,
      61,    62,    63,    64,     0,     0,    65,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,    69,    70,    71,
      72,    73,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,     0,    83,    21,    22,    23,
      24,     0,     0,     0,     0,     0,    25,    26,     0,     0,
      27,    28,    29,    30,    31,    32,     0,     0,     0,     0,
       0,    33,     0,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,     0,     0,     0,     0,     0,     0,    44,
       0,    46,    47,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
       0,    58,    59,    60,     0,     0,    61,    62,    63,    64,
       0,     0,    65,    66,     0,     0,     0,     0,     0,    67,
      68,     0,   232,    69,    70,    71,    72,    73,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,     0,    83,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,     0,    58,    59,    60,     0,     0,    61,
      62,    63,    64,     0,     0,    65,    66,     0,     0,   233,
       0,     0,    67,     0,     0,     0,    69,    70,    71,    72,
      73,    74,     0,     0,     0,  -274,     0,  -274,     0,   336,
       0,     0,     0,    75,    76,    77,  -274,    78,    79,    80,
      81,    82,   337,     0,     0,    83,     0,   240,   241,   242,
     243,   244,   245,   246,   247,   248,     0,   249,   250,   251,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,   338,     0,     0,   151,     0,   252,
     253,   254,   255,   256,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,   260,    14,
     261,   262,   263,   264,   265,     0,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,     0,    58,    59,    60,
       0,     0,    61,    62,    63,    64,     0,     0,    65,    66,
       0,     0,     0,     0,     0,    67,     0,     0,     0,    69,
      70,    71,    72,    73,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,     0,
      78,    79,    80,    81,    82,   270,     0,     0,    83,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,    58,    59,    60,     0,     0,    61,    62,
      63,    64,     0,     0,    65,    66,     0,     0,     0,     0,
       0,    67,     0,     0,     0,    69,    70,    71,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,     0,    78,    79,    80,    81,
      82,   172,     0,     0,    83,     0,     0,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,     0,    58,    59,
      60,     0,     0,    61,    62,    63,    64,     0,   173,    65,
      66,     0,     0,     0,     0,     0,    67,     0,     0,     0,
      69,    70,    71,    72,    73,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
       0,    78,    79,    80,    81,    82,  -126,     0,     0,    83,
       0,     0,     0,     0,   240,   241,   242,   243,   244,   245,
     246,   247,   248,     0,   249,   250,   251,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   253,   254,   255,
     256,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   259,   260,     0,   261,   262,   263,
     264,   265,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,    58,    59,    60,     0,     0,    61,    62,
      63,    64,     0,     0,    65,    66,     0,     0,     0,     0,
       0,    67,     0,     0,     0,    69,    70,    71,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,     0,    83,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   249,   250,   251,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   253,   254,
     255,   256,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   258,   259,   260,     0,   261,   262,
     263,   264,   265,     0,     0,     0,     0,   498,   240,   241,
     242,   243,   244,   245,   246,   247,   248,     0,   249,   250,
     251,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   255,   256,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,   260,
       0,   261,   262,   263,   264,   265,   240,   241,   242,   243,
     244,   245,   246,   247,   248,     0,   249,   250,   240,   241,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,     0,   249,   250,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   253,
     254,   255,   256,   257,     0,     0,     0,     0,     0,     0,
    -440,  -440,  -440,  -440,  -440,  -440,   259,   260,     0,   261,
     262,   263,   264,   265,     0,     0,     0,     0,     0,  -440,
       0,   261,   262,   263,   264,   265,   240,   241,   242,   243,
     244,   245,   246,   247,   248,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   253,
     254,   255,   256,   257,     0,     0,     0,     0,     0,     0,
       0,     0,   305,     0,     0,     0,     0,   260,   306,   261,
     262,   263,   264,   265,   164,   165,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,     0,   308,   309,     0,     0,
       0,     0,     0,     0,   310
};

static const short yycheck[] =
{
       7,    11,    11,    92,   193,    96,   112,   195,   104,   180,
      89,   274,    86,   104,    93,    94,    90,   401,    14,   225,
     283,   396,   101,   225,     1,     1,   105,   106,   117,   413,
       1,   218,     1,   107,     1,     1,     1,   409,   410,    48,
       1,     1,    47,     1,   118,     1,     1,     1,   477,     1,
       7,     8,     9,   408,   409,   410,    25,    26,   307,   308,
     309,    41,    14,     1,    70,   144,   189,    73,   191,    78,
      79,     0,    28,    49,    49,    41,    42,    43,    44,    88,
      49,    90,     0,   408,    49,   410,     1,     1,    49,    98,
     519,   197,    41,    49,    49,   470,     1,     1,   189,   104,
     191,    76,   111,    72,    73,    71,    73,    83,    73,    67,
      79,    49,    73,    73,   220,    73,    96,   489,    84,    73,
     209,   493,    88,   100,   101,   102,   103,   218,    83,   484,
     485,   210,   507,   488,   489,   341,    41,   492,   493,   341,
     495,   496,   497,   498,    49,   332,    95,    96,   119,   118,
      41,   118,    67,    67,     0,   151,    73,   541,   108,    73,
     485,     1,    67,    67,    78,    79,     1,   492,   523,    49,
     525,    85,   527,     1,   529,    35,   531,    37,   533,    10,
      11,     1,    97,    98,    99,   100,   101,   102,   103,     1,
      95,    96,   201,    97,   203,   570,   100,   101,   102,   103,
      58,   118,   465,     1,    95,    49,    41,   303,    43,    49,
      68,    76,   303,   287,   120,   311,   408,   409,   227,   310,
     311,   300,    73,   232,   579,   120,    66,   582,   399,    49,
      70,   105,    72,    73,    66,     1,    71,    49,    78,    79,
      72,   332,    66,     1,   105,    85,    66,     1,    72,    84,
      70,    73,    72,    73,   112,   113,   114,     1,    78,    79,
     431,    73,   433,     1,     1,    85,    14,    15,   108,    67,
     110,   111,   100,   101,   102,   103,   285,   286,   118,   288,
     120,    39,    40,    49,   293,     1,   118,    26,   108,   298,
     110,   111,   484,     1,   118,    49,   488,     1,   118,    97,
      98,    99,   100,   101,   102,   103,   119,    73,     1,     1,
     319,    49,    49,   119,    72,    73,    49,   423,    95,    96,
      78,    79,    67,    67,   415,    66,   417,    85,   419,    73,
     339,    72,    25,    49,    78,    79,     1,     1,     7,     8,
       9,    85,    68,    51,    52,    53,    39,    40,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   545,   561,   562,   118,   549,    72,
      73,    26,     1,     1,    49,    49,   396,     1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,   119,    67,    68,
      69,   590,   590,   591,    46,     1,     1,     1,     4,     1,
       1,     1,     1,     1,     1,    46,    10,    11,    46,   508,
      89,    90,    91,    92,    93,    94,    22,    23,    24,    25,
      49,    49,   119,    29,   119,    49,   119,   106,   107,   108,
       1,   110,   111,   112,   113,   114,    41,    42,    43,    44,
      14,    47,    41,     1,    43,    49,    15,    49,    49,    49,
     470,    49,   472,   473,   473,   474,   475,   476,    41,     1,
       1,   478,   169,   170,    96,    95,    71,    25,    14,   507,
      67,   283,    71,   471,    80,    81,    73,   480,    49,    84,
     540,    78,    79,    88,   591,    84,   217,   507,    85,   341,
     538,    49,   350,   513,   513,   108,    54,    55,   104,   105,
     108,     1,   231,     3,     4,     5,     6,    49,    49,   557,
     558,   267,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   108,   149,    -1,    -1,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
     570,    -1,    -1,    -1,    -1,   572,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    58,    72,    73,    74,    75,    76,     1,    78,    79,
       4,    68,    -1,    -1,    -1,    85,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    73,    22,    23,
      24,    25,    78,    79,    -1,    29,   106,   107,   108,    85,
     110,   111,   112,   113,   114,    -1,    -1,     1,   118,     3,
       4,     5,     6,   110,   111,   112,   113,   114,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    -1,    80,    81,    -1,    -1,
      -1,    45,    46,    47,    48,    97,    98,    99,   100,   101,
     102,   103,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    72,    73,
      74,    75,    -1,    -1,    78,    79,    56,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    72,    73,    74,    75,    -1,    -1,    78,    79,
      -1,    -1,   106,   107,   108,    85,   110,   111,   112,   113,
     114,    -1,    -1,     1,   118,     3,     4,     5,     6,    -1,
      -1,    -1,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,   118,    27,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    97,    -1,    -1,   100,   101,   102,   103,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,     1,    -1,    72,    73,    74,    75,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,     1,    67,    68,
      69,    -1,    -1,    72,    73,    74,    75,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,   118,
      -1,    -1,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    72,    73,
      74,    75,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,     3,     4,     5,     6,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    73,    74,    75,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,     3,     4,     5,     6,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    -1,
      72,    73,    74,    75,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,    -1,   118,     3,     4,     5,
       6,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    72,    73,    74,    75,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    25,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,    -1,   118,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      73,    74,    75,    -1,    -1,    78,    79,    -1,    -1,    82,
      -1,    -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    35,    -1,    37,    -1,     1,
      -1,    -1,    -1,   106,   107,   108,    46,   110,   111,   112,
     113,   114,    14,    -1,    -1,   118,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,    49,
     110,   111,   112,   113,   114,    -1,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    72,    73,    74,    75,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    49,    -1,    -1,   118,    -1,
      -1,    -1,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    72,    73,
      74,    75,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    50,    -1,    -1,   118,    -1,    -1,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    72,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    49,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    72,    73,
      74,    75,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,   119,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   110,   111,   112,   113,   114,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,   108,    72,   110,
     111,   112,   113,   114,    78,    79,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,   122,   123,     0,   124,   131,   132,   125,   127,   128,
     133,   134,   135,   238,    49,    76,   130,   150,   216,   217,
     127,     3,     4,     5,     6,    12,    13,    16,    17,    18,
      19,    20,    21,    27,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    45,    46,    47,    48,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    67,    68,
      69,    72,    73,    74,    75,    78,    79,    85,    86,    89,
      90,    91,    92,    93,    94,   106,   107,   108,   110,   111,
     112,   113,   114,   118,   134,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   169,   170,   175,   176,   177,   178,
     208,   209,   210,   211,   212,   213,   214,   215,   218,   219,
     220,   222,   223,   227,   228,   229,   230,   231,   232,   234,
     237,   240,   248,   250,   251,   252,   281,   282,   285,   286,
     132,    49,   252,   252,   252,   253,    14,    15,   130,     1,
      39,    40,    72,    73,    78,    79,    85,   173,   243,   244,
     245,   290,    50,    77,   179,   205,   252,   252,   294,   130,
     252,     1,    73,   297,   297,   294,   294,   166,     1,    70,
     225,   281,   287,   291,    73,   118,   295,   296,   252,     1,
       1,    51,    52,    53,   294,     1,     4,    22,    23,    24,
      25,    29,    47,    80,    81,   104,   105,   224,   226,     1,
      28,   287,   294,   294,     1,   130,     1,    25,   241,   243,
     244,   245,    25,    82,   249,   252,   295,   297,   130,   294,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    67,
      68,    69,    89,    90,    91,    92,    93,    94,   106,   107,
     108,   110,   111,   112,   113,   114,   283,   284,   283,   132,
      49,   280,   292,   294,   171,     1,    41,    43,    71,    84,
     246,     1,   246,   172,     1,    10,    11,   207,   108,   131,
       1,     1,     1,    25,    26,    72,    73,    79,   118,   167,
     180,   183,   291,   120,   291,    66,    72,   108,   110,   111,
     118,   288,   289,   290,   296,     1,   295,   252,     1,    25,
      54,    55,   221,   252,   297,     1,   294,   105,   105,    47,
     104,     1,   226,   225,     1,   295,     1,    14,    46,   161,
     162,   235,   236,   252,     1,     1,    42,    44,    88,   246,
     247,     1,   247,   252,     1,     1,     1,   265,   258,   267,
     269,   268,   270,   271,   272,   273,   264,   263,   262,   274,
     275,   276,   277,   278,   279,   260,   261,   266,   254,   255,
     256,   257,   259,   280,     1,   119,   120,   174,   292,     1,
      83,   242,     1,   174,   252,   252,   130,   252,     1,    14,
     168,   252,     1,    67,   184,   185,   186,   187,   191,   192,
     193,   201,   252,   294,   287,   118,   289,   118,   289,   118,
     289,   281,   287,   119,   252,     1,    26,   225,     1,     1,
       1,   252,     1,   162,   236,   239,     1,   242,     1,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   293,   119,     1,    73,     1,
     126,   129,   133,   134,     7,     8,     9,   206,   131,   181,
     182,   181,     1,   188,   192,   193,   201,   189,   191,   193,
     201,   190,   191,   192,   201,    41,    95,    96,   119,   181,
     281,   281,   281,   119,   295,   131,     1,   131,    46,   233,
       1,   292,   133,   134,   177,   134,   252,   252,   252,   206,
       1,   127,   183,   193,   201,   192,   201,   193,   201,   191,
     201,   192,   201,   191,   201,     1,   201,     1,   201,     1,
     201,   201,   119,   119,   119,    14,   129,   297,   206,    15,
     201,   201,   201,   201,   201,   201,    97,    98,    99,   100,
     101,   102,   103,   195,   196,    97,   194,   196,   195,   181,
     131,     1,   131,     1,   196,     1,   196,     1,   199,   200,
       1,   197,   198,     1,   199,     1,   199,   133,     1,   127,
     201,   201,    14,   202,   203,   204,   290,   296,   203
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
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
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 158 "./yaccsrc.y"
    { level = 0;
                                         if ( get_options_flag( parser_data.TSD->currlevel, EXT_CALLS_AS_FUNCS )
                                           && !get_options_flag( parser_data.TSD->currlevel, EXT_STRICT_ANSI ) )
                                            start_parendepth = 1;
                                         else
                                            start_parendepth = 0;
                                         parendepth = 0; ;}
    break;

  case 4:
#line 168 "./yaccsrc.y"
    { yyval = optgluelast( yyvsp[-1], yyvsp[0] );
                                         yyval->o.last = NULL;
                                         EndProg( yyval ) ; ;}
    break;

  case 5:
#line 171 "./yaccsrc.y"
    { yyval = yyvsp[0];
                                         if ( yyval != NULL )
                                            yyval->o.last = NULL;
                                         EndProg( yyval ); ;}
    break;

  case 6:
#line 177 "./yaccsrc.y"
    { /* fixes bug 579711 */
                                         yyval = optgluelast( yyvsp[-1], yyvsp[0] ); ;}
    break;

  case 7:
#line 179 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 8:
#line 182 "./yaccsrc.y"
    { /* fixes bug 579711 */
                                         yyval = optgluelast( yyvsp[-1], yyvsp[0] ); ;}
    break;

  case 9:
#line 184 "./yaccsrc.y"
    { yyval = yyvsp[-1]; ;}
    break;

  case 10:
#line 187 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 11:
#line 188 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_END, 1 ); ;}
    break;

  case 13:
#line 196 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 14:
#line 197 "./yaccsrc.y"
    { yyval = NULL; ;}
    break;

  case 15:
#line 200 "./yaccsrc.y"
    { yyval = optgluelast( yyvsp[-2], yyvsp[0] ); ;}
    break;

  case 16:
#line 201 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 17:
#line 204 "./yaccsrc.y"
    { yyval = optgluelast( yyvsp[-1], yyvsp[0] ); ;}
    break;

  case 18:
#line 205 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 19:
#line 208 "./yaccsrc.y"
    { yyval = optgluelast( yyvsp[-1], yyvsp[0] ); ;}
    break;

  case 20:
#line 209 "./yaccsrc.y"
    { yyval = NULL; ;}
    break;

  case 23:
#line 216 "./yaccsrc.y"
    { tmpchr=parser_data.tstart;
                                         tmplno=parser_data.tline; ;}
    break;

  case 24:
#line 220 "./yaccsrc.y"
    { yyval=yyvsp[0]; ;}
    break;

  case 25:
#line 223 "./yaccsrc.y"
    { yyval = optgluelast( yyvsp[-1], yyvsp[0] ); ;}
    break;

  case 54:
#line 256 "./yaccsrc.y"
    { yyval = makenode(X_CALL,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 55:
#line 260 "./yaccsrc.y"
    { yyval = makenode(X_DO,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ;
                                         level++; ;}
    break;

  case 56:
#line 265 "./yaccsrc.y"
    { yyval = makenode(X_EXIT,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 57:
#line 269 "./yaccsrc.y"
    { yyval = makenode(X_IF,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ;
                                         level++; ;}
    break;

  case 58:
#line 274 "./yaccsrc.y"
    { yyval = makenode(X_ITERATE,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 59:
#line 278 "./yaccsrc.y"
    { yyval = makenode(X_LEAVE,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 60:
#line 282 "./yaccsrc.y"
    { yyval = makenode(X_SAY,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 61:
#line 286 "./yaccsrc.y"
    { yyval = makenode(X_RETURN,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 62:
#line 290 "./yaccsrc.y"
    { yyval = makenode(X_ADDR_N,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 63:
#line 294 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_ARG,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 64:
#line 298 "./yaccsrc.y"
    { yyval = makenode(X_DROP,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 65:
#line 302 "./yaccsrc.y"
    { yyval = makenode(X_IPRET,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 66:
#line 306 "./yaccsrc.y"
    { yyval = makenode(X_LABEL,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 67:
#line 310 "./yaccsrc.y"
    { yyval = makenode(X_NULL,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 68:
#line 314 "./yaccsrc.y"
    { yyval = makenode(0,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 69:
#line 318 "./yaccsrc.y"
    { yyval = makenode(X_OPTIONS,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 70:
#line 322 "./yaccsrc.y"
    { yyval = makenode(0,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 71:
#line 326 "./yaccsrc.y"
    { yyval = makenode(X_PROC,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 72:
#line 330 "./yaccsrc.y"
    { yyval = makenode(X_PULL,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 73:
#line 334 "./yaccsrc.y"
    { yyval = makenode(X_PUSH,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 74:
#line 338 "./yaccsrc.y"
    { yyval = makenode(X_QUEUE,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 75:
#line 342 "./yaccsrc.y"
    { yyval = makenode(X_SELECT,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ;
                                         level++; ;}
    break;

  case 76:
#line 347 "./yaccsrc.y"
    { yyval = makenode(X_SIG_LAB,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 77:
#line 351 "./yaccsrc.y"
    { yyval = makenode(X_WHEN,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 78:
#line 355 "./yaccsrc.y"
    { yyval = makenode(X_OTHERWISE,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 79:
#line 359 "./yaccsrc.y"
    { yyval = makenode(X_TRACE,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 80:
#line 363 "./yaccsrc.y"
    { yyval = makenode(X_UPPER_VAR,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 81:
#line 367 "./yaccsrc.y"
    { yyval = current = yyvsp[0] ; ;}
    break;

  case 83:
#line 371 "./yaccsrc.y"
    { current->type = X_ADDR_V ;
                                         current->p[0] = yyvsp[-1] ;
                                         current->p[1] = yyvsp[0] ; ;}
    break;

  case 84:
#line 374 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 1, __reginatext ) ;;}
    break;

  case 85:
#line 375 "./yaccsrc.y"
    { current->type = X_ADDR_S ; ;}
    break;

  case 86:
#line 376 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 1, __reginatext ) ;;}
    break;

  case 88:
#line 378 "./yaccsrc.y"
    { current->name = (streng *)yyvsp[-2] ;
                                         current->type = X_ADDR_N ;
                                         current->p[0] = yyvsp[-1] ;
                                         current->p[1] = yyvsp[0] ; ;}
    break;

  case 89:
#line 382 "./yaccsrc.y"
    { current->type = X_ADDR_V ;
                                         current->p[0] = yyvsp[-3] ;
                                         current->p[1] = yyvsp[0] ;
                                         current->u.nonansi = 1 ; ;}
    break;

  case 90:
#line 388 "./yaccsrc.y"
    { yyval = makenode( X_PARSE, 2, yyvsp[-1], yyvsp[0] );
                                         yyval->u.parseflags = PARSE_UPPER;
                                         yyval->lineno = yyvsp[-1]->lineno;
                                         yyval->charnr = yyvsp[-1]->charnr; ;}
    break;

  case 91:
#line 394 "./yaccsrc.y"
    { parendepth = start_parendepth; ;}
    break;

  case 92:
#line 395 "./yaccsrc.y"
    { yyval = yyvsp[-3];
                                         yyval->p[0] = yyvsp[0];
                                         yyval->name = (streng *) yyvsp[-2];
                                         parendepth = 0; ;}
    break;

  case 93:
#line 399 "./yaccsrc.y"
    { parendepth = start_parendepth; ;}
    break;

  case 94:
#line 400 "./yaccsrc.y"
    { yyval = yyvsp[-3];
                                         yyval->type = X_EX_FUNC;
                                         yyval->p[0] = yyvsp[0];
                                         yyval->name = (streng *) yyvsp[-2];
                                         parendepth = 0; ;}
    break;

  case 95:
#line 405 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 1, "ERROR FAILURE HALT NOTREADY", __reginatext ) ;;}
    break;

  case 96:
#line 406 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 2, "ERROR FAILURE HALT NOTREADY", __reginatext ) ;;}
    break;

  case 97:
#line 407 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 98:
#line 408 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 3, __reginatext ) ;;}
    break;

  case 99:
#line 410 "./yaccsrc.y"
    { yyval = yyvsp[-3] ;
                                         yyval->type = X_CALL_SET ;
                                         yyval->p[0] = yyvsp[-2] ;
                                         yyval->name = (streng *)yyvsp[0] ;
                                         yyval->p[1] = yyvsp[-1] ; ;}
    break;

  case 100:
#line 415 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_CALL_SET ;
                                         yyval->p[0] = yyvsp[-1] ;
                                         yyval->name = NULL ;
                                         yyval->p[1] = yyvsp[0] ; ;}
    break;

  case 101:
#line 420 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 102:
#line 421 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_CALL_SET ;
                                         yyval->p[0] = yyvsp[-1] ;
                                         yyval->p[1] = yyvsp[0] ; ;}
    break;

  case 103:
#line 427 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 104:
#line 428 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 2, __reginatext );;}
    break;

  case 105:
#line 431 "./yaccsrc.y"
    {
                                         /*
                                          * "call_args" accepted probably with
                                          * surrounding parentheses. Strip them.
                                          */
                                         yyval = reduce_expr_list( yyvsp[0],
                                                                REDUCE_CALL );
                                       ;}
    break;

  case 106:
#line 439 "./yaccsrc.y"
    { exiterror(ERR_UNEXPECTED_PARAN, 2); ;}
    break;

  case 107:
#line 442 "./yaccsrc.y"
    { yyval = makenode(X_COMMAND,0) ;
                                         yyval->charnr = tmpchr ;
                                         yyval->lineno = tmplno;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 108:
#line 448 "./yaccsrc.y"
    { yyval = makenode(X_END,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ;
                                         level--; ;}
    break;

  case 109:
#line 454 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->name = (streng*)(yyvsp[0]) ; ;}
    break;

  case 110:
#line 456 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 111:
#line 457 "./yaccsrc.y"
    {  exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 112:
#line 461 "./yaccsrc.y"
    { yyval = yyvsp[-5];
                                         yyval->p[0] = yyvsp[-4];
                                         yyval->p[1] = yyvsp[-3];
                                         yyval->p[2] = optgluelast( yyvsp[-2], yyvsp[-1] );
                                         if ( yyval->p[2] )
                                            yyval->p[2]->o.last = NULL;
                                         yyval->p[3] = yyvsp[0];
                          if ((yyval->p[0]==NULL || yyval->p[0]->name==NULL)
                              && yyval->p[3]->name)
                                            exiterror( ERR_UNMATCHED_END, 0 );
                          if ((yyval->p[0])&&(yyval->p[0]->name)&&
                              (yyval->p[3]->name)&&
                              ((yyval->p[3]->name->len != yyval->p[0]->name->len)||
                               (strncmp(yyval->p[3]->name->value,
                                        yyval->p[0]->name->value,
                                        yyval->p[0]->name->len))))
                                            exiterror( ERR_UNMATCHED_END, 0 );
                                       ;}
    break;

  case 113:
#line 482 "./yaccsrc.y"
    { yyval =makenode(X_REP,4,yyvsp[-3],yyvsp[-2],yyvsp[-1],yyvsp[0]) ;
                                         yyval->name = (streng *)yyvsp[-5] ;
                                         checkdosyntax(yyval) ; ;}
    break;

  case 114:
#line 486 "./yaccsrc.y"
    { yyval =makenode(X_REP,3,yyvsp[-2],yyvsp[-1],yyvsp[0]) ;
                                         yyval->name = (streng *)yyvsp[-4] ;
                                         checkdosyntax(yyval) ; ;}
    break;

  case 115:
#line 489 "./yaccsrc.y"
    { yyval =makenode(X_REP,2,yyvsp[-1],yyvsp[0]) ;
                                         yyval->name = (streng *)yyvsp[-3] ;
                                         checkdosyntax(yyval) ; ;}
    break;

  case 116:
#line 492 "./yaccsrc.y"
    { yyval =makenode(X_REP,1,yyvsp[0]) ;
                                         yyval->name = (streng *)yyvsp[-2] ;
                                         checkdosyntax(yyval) ; ;}
    break;

  case 117:
#line 495 "./yaccsrc.y"
    { yyval = makenode(X_REP_FOREVER,0) ; ;}
    break;

  case 118:
#line 496 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 16, "WHILE UNTIL", __reginatext ) ; ;}
    break;

  case 119:
#line 497 "./yaccsrc.y"
    { yyvsp[0] = makenode(X_DO_EXPR,1,yyvsp[0]) ;
                                         yyval = makenode(X_REP,2,NULL,yyvsp[0]) ; ;}
    break;

  case 120:
#line 499 "./yaccsrc.y"
    { yyval = NULL ; ;}
    break;

  case 121:
#line 502 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 122:
#line 503 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 123:
#line 504 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 124:
#line 507 "./yaccsrc.y"
    { SymbolDetect |= SD_ADDRWITH ;
                                         yyval = with = makenode(X_ADDR_WITH,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 125:
#line 511 "./yaccsrc.y"
    { with = NULL ;
                                         SymbolDetect &= ~SD_ADDRWITH ; ;}
    break;

  case 126:
#line 513 "./yaccsrc.y"
    { yyval = NULL ; ;}
    break;

  case 127:
#line 516 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 128:
#line 517 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 5, __reginatext ) ; ;}
    break;

  case 129:
#line 518 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 5, __reginatext ) ; ;}
    break;

  case 133:
#line 524 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 5, __reginatext ) ; ;}
    break;

  case 152:
#line 560 "./yaccsrc.y"
    { with->p[0] = yyvsp[0]; ;}
    break;

  case 153:
#line 561 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 6, __reginatext ) ; ;}
    break;

  case 154:
#line 564 "./yaccsrc.y"
    { with->p[1] = yyvsp[0]; ;}
    break;

  case 155:
#line 565 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 7, __reginatext ) ; ;}
    break;

  case 156:
#line 568 "./yaccsrc.y"
    { with->p[2] = yyvsp[0]; ;}
    break;

  case 157:
#line 569 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 14, __reginatext ) ; ;}
    break;

  case 158:
#line 572 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 159:
#line 573 "./yaccsrc.y"
    { yyval = makenode(X_ADDR_WITH, 0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 160:
#line 578 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 161:
#line 579 "./yaccsrc.y"
    { yyval = yyvsp[0] ;
                                         yyval->u.of.append = 1 ; ;}
    break;

  case 162:
#line 581 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 8, __reginatext ) ; ;}
    break;

  case 163:
#line 582 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 164:
#line 583 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 9, __reginatext ) ; ;}
    break;

  case 165:
#line 584 "./yaccsrc.y"
    { yyval = makenode(X_ADDR_WITH, 0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 166:
#line 589 "./yaccsrc.y"
    { /* ANSI extension: nsimsymb is
                                          * used by the standard, but I think
                                          * there are no reasons why using
                                          * it here as a must. FGC
                                          */
                                         yyval = yyvsp[0] ;
                                         yyval->u.of.awt = awtSTREAM;
                                         SymbolDetect |= SD_ADDRWITH ; ;}
    break;

  case 167:
#line 597 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 1, __reginatext ) ; ;}
    break;

  case 168:
#line 598 "./yaccsrc.y"
    {
                                         streng *tmp = yyvsp[0]->name;
                                         char *p;

                                         /*
                                          * expect a single dot as the last character
                                          */
                                         p = memchr( tmp->value, '.', tmp->len );
                                         if ( p != tmp->value + tmp->len - 1 )
                                            exiterror( ERR_INVALID_OPTION, 3, __reginatext );
                                         yyval = yyvsp[0] ;
                                         yyval->u.of.awt = awtSTEM ;
                                         SymbolDetect |= SD_ADDRWITH ; ;}
    break;

  case 169:
#line 611 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 2, __reginatext ) ; ;}
    break;

  case 170:
#line 612 "./yaccsrc.y"
    {
                                         yyval = yyvsp[0] ;
                                         yyval->u.of.awt = awtLIFO ;
                                         SymbolDetect |= SD_ADDRWITH ; ;}
    break;

  case 171:
#line 616 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 100, __reginatext ) ; ;}
    break;

  case 172:
#line 617 "./yaccsrc.y"
    {
                                         yyval = yyvsp[0] ;
                                         yyval->u.of.awt = awtFIFO ;
                                         SymbolDetect |= SD_ADDRWITH ; ;}
    break;

  case 173:
#line 621 "./yaccsrc.y"
    { exiterror( ERR_INVALID_OPTION, 101, __reginatext ) ; ;}
    break;

  case 174:
#line 624 "./yaccsrc.y"
    { SymbolDetect &= ~SD_ADDRWITH ; ;}
    break;

  case 175:
#line 625 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 176:
#line 628 "./yaccsrc.y"
    { SymbolDetect &= ~SD_ADDRWITH ; ;}
    break;

  case 177:
#line 629 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 180:
#line 636 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 181:
#line 637 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 182:
#line 640 "./yaccsrc.y"
    { yyval = makenode(X_ADDR_WITH, 0) ;
                                         yyval->name = (streng *) yyvsp[0] ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ;
                                         yyval->u.of.ant = antSIMSYMBOL;
                                       ;}
    break;

  case 183:
#line 648 "./yaccsrc.y"
    { yyval = makenode(X_ADDR_WITH, 0) ;
                                         yyval->name = (streng *) yyvsp[0] ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ;
                                         yyval->u.of.ant = antSTRING;
                                       ;}
    break;

  case 184:
#line 656 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 185:
#line 659 "./yaccsrc.y"
    { yyval = makenode(X_DO_TO,1,yyvsp[0]) ; ;}
    break;

  case 186:
#line 660 "./yaccsrc.y"
    { yyval = makenode(X_DO_FOR,1,yyvsp[0]) ; ;}
    break;

  case 187:
#line 661 "./yaccsrc.y"
    { yyval = makenode(X_DO_BY,1,yyvsp[0]) ; ;}
    break;

  case 188:
#line 664 "./yaccsrc.y"
    { yyval = makenode(X_WHILE,1,yyvsp[0]) ; ;}
    break;

  case 189:
#line 665 "./yaccsrc.y"
    { yyval = makenode(X_UNTIL,1,yyvsp[0]) ; ;}
    break;

  case 190:
#line 666 "./yaccsrc.y"
    { yyval = NULL ; ;}
    break;

  case 191:
#line 669 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;;}
    break;

  case 192:
#line 670 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 193:
#line 674 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;;}
    break;

  case 194:
#line 675 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 195:
#line 679 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 196:
#line 684 "./yaccsrc.y"
    { move_labels( yyvsp[-5], yyvsp[0], level - 1 );
                                         yyval = yyvsp[-5];
                                         yyval->p[0] = optgluelast( yyvsp[-4], yyvsp[-3] );
                                         yyval->p[0]->o.last = NULL;
                                         yyval->p[1] = optgluelast( yyvsp[-1], yyvsp[0] );
                                         yyval->p[1]->o.last = NULL;
                                         level--; ;}
    break;

  case 197:
#line 692 "./yaccsrc.y"
    { move_labels( yyvsp[-8], yyvsp[0], level - 1 );
                                         yyval = yyvsp[-8];
                                         yyval->p[0] = optgluelast( yyvsp[-7], yyvsp[-6] );
                                         yyval->p[0]->o.last = NULL;
                                         yyval->p[1] = optgluelast( yyvsp[-4], yyvsp[-3] );
                                         yyval->p[1]->o.last = NULL;
                                         yyval->p[2] = optgluelast( yyvsp[-1], yyvsp[0] );
                                         yyval->p[2]->o.last = NULL;
                                         level--; ;}
    break;

  case 198:
#line 702 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 4 ) ;;}
    break;

  case 199:
#line 704 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 3 ) ;;}
    break;

  case 200:
#line 705 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 0 ) ;;}
    break;

  case 201:
#line 706 "./yaccsrc.y"
    {  exiterror( ERR_THEN_EXPECTED, 1, parser_data.if_linenr, __reginatext )  ; ;}
    break;

  case 202:
#line 709 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 1 )  ; ;}
    break;

  case 203:
#line 712 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 2 )  ; ;}
    break;

  case 204:
#line 715 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 205:
#line 720 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 206:
#line 722 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 207:
#line 723 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 208:
#line 726 "./yaccsrc.y"
    { yyval = yyvsp[0] ;
                                         yyval->u.trace_only =
                                                         (level == 0) ? 0 : 1;
                                         newlabel( (const tsd_t *)parser_data.TSD,
                                                   &parser_data,
                                                   yyvsp[0] ) ; ;}
    break;

  case 209:
#line 734 "./yaccsrc.y"
    { yyval = yyvsp[0] ;
                                         yyval->name = Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 210:
#line 738 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 211:
#line 740 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 212:
#line 741 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 213:
#line 744 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 214:
#line 745 "./yaccsrc.y"
    {  exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 215:
#line 748 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_NUM_D ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 216:
#line 751 "./yaccsrc.y"
    { yyval = yyvsp[-1]; yyval->type = X_NUM_DDEF ; ;}
    break;

  case 217:
#line 752 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 218:
#line 753 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_NUM_F ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 219:
#line 756 "./yaccsrc.y"
    { /* NOTE! This clashes ANSI! */
                                         yyval = yyvsp[-1] ; yyval->type=X_NUM_FRMDEF ;;}
    break;

  case 220:
#line 758 "./yaccsrc.y"
    { yyval = yyvsp[-3] ; yyval->type=X_NUM_V ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 221:
#line 760 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 11, "ENGINEERING SCIENTIFIC", __reginatext ) ;;}
    break;

  case 222:
#line 761 "./yaccsrc.y"
    { yyval = yyvsp[-1]; yyval->type = X_NUM_FDEF ;;}
    break;

  case 223:
#line 762 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_NUM_FUZZ ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 224:
#line 765 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 15, "DIGITS FORM FUZZ", __reginatext ) ;;}
    break;

  case 225:
#line 768 "./yaccsrc.y"
    { yyval = makenode(X_NUM_SCI,0) ; ;}
    break;

  case 226:
#line 769 "./yaccsrc.y"
    { yyval = makenode(X_NUM_ENG,0) ; ;}
    break;

  case 227:
#line 772 "./yaccsrc.y"
    { (yyval=yyvsp[-1])->p[0]=yyvsp[0] ; ;}
    break;

  case 228:
#line 776 "./yaccsrc.y"
    { yyval = yyvsp[-3] ;
                                         yyval->type = X_PARSE ;
                                         yyval->u.parseflags = (int) yyvsp[-2] ;
                                         yyval->p[0] = yyvsp[-1] ;
                                         yyval->p[1] = yyvsp[0] ; ;}
    break;

  case 229:
#line 782 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_PARSE ;
                                         yyval->u.parseflags = 0;
                                         yyval->p[0] = yyvsp[-1] ;
                                         yyval->p[1] = yyvsp[0] ; ;}
    break;

  case 230:
#line 787 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 12, "ARG EXTERNAL LINEIN PULL SOURCE VAR VALUE VERSION", __reginatext ) ;;}
    break;

  case 231:
#line 788 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 12, "ARG CASELESS EXTERNAL LINEIN LOWER PULL SOURCE UPPER VAR VALUE VERSION", __reginatext ) ;;}
    break;

  case 232:
#line 791 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_UPPER  |
                                                         PARSE_NORMAL); ;}
    break;

  case 233:
#line 793 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_UPPER  |
                                                         PARSE_CASELESS); ;}
    break;

  case 234:
#line 795 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_UPPER  |
                                                         PARSE_CASELESS); ;}
    break;

  case 235:
#line 797 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_LOWER  |
                                                         PARSE_NORMAL); ;}
    break;

  case 236:
#line 799 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_LOWER  |
                                                         PARSE_CASELESS); ;}
    break;

  case 237:
#line 801 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_LOWER  |
                                                         PARSE_CASELESS); ;}
    break;

  case 238:
#line 803 "./yaccsrc.y"
    { yyval = (nodeptr) (PARSE_NORMAL |
                                                         PARSE_CASELESS); ;}
    break;

  case 239:
#line 807 "./yaccsrc.y"
    { /* fixes bugs like bug 579711 */
                                         yyval = optgluelast( yyvsp[-2], yyvsp[0] ); ;}
    break;

  case 240:
#line 809 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 241:
#line 812 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_ARG,0) ; ;}
    break;

  case 242:
#line 813 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_EXT,0) ; ;}
    break;

  case 243:
#line 814 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_EXT,0) ; ;}
    break;

  case 244:
#line 815 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_VER,0) ; ;}
    break;

  case 245:
#line 816 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_PULL,0) ; ;}
    break;

  case 246:
#line 817 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_SRC,0) ; ;}
    break;

  case 247:
#line 818 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_VAR,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 248:
#line 820 "./yaccsrc.y"
    { yyval = makenode(X_PARSE_VAL,1,yyvsp[-1]) ; ;}
    break;

  case 249:
#line 821 "./yaccsrc.y"
    { exiterror( ERR_INVALID_TEMPLATE, 3 ) ;;}
    break;

  case 250:
#line 824 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 251:
#line 825 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 17, __reginatext ) ;;}
    break;

  case 252:
#line 826 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;;}
    break;

  case 253:
#line 827 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;;}
    break;

  case 254:
#line 828 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 255:
#line 832 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 256:
#line 836 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 257:
#line 840 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 258:
#line 844 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 259:
#line 848 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 260:
#line 852 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_END, 0 ) ;;}
    break;

  case 261:
#line 853 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 262:
#line 854 "./yaccsrc.y"
    { level--; ;}
    break;

  case 263:
#line 858 "./yaccsrc.y"
    { yyval = yyvsp[-4];
                                         yyval->p[0] = optgluelast( yyvsp[-3], yyvsp[-2] );
                                         yyval->p[0]->o.last = NULL;
                                         yyval->p[1] = yyvsp[-1]; ;}
    break;

  case 264:
#line 862 "./yaccsrc.y"
    {  exiterror( ERR_WHEN_EXPECTED, 0 ) ;;}
    break;

  case 265:
#line 864 "./yaccsrc.y"
    {  exiterror( ERR_WHEN_EXPECTED, 0 ) ;;}
    break;

  case 266:
#line 865 "./yaccsrc.y"
    {  exiterror( ERR_EXTRA_DATA, 1, __reginatext )  ;;}
    break;

  case 267:
#line 866 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 0 ) ;;}
    break;

  case 268:
#line 868 "./yaccsrc.y"
    {  exiterror( ERR_INCOMPLETE_STRUCT, 0 ) ;;}
    break;

  case 269:
#line 871 "./yaccsrc.y"
    { yyval = optgluelast( yyvsp[-1], yyvsp[0] ); ;}
    break;

  case 270:
#line 872 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 271:
#line 873 "./yaccsrc.y"
    {  exiterror( ERR_WHEN_EXPECTED, 0 )  ;;}
    break;

  case 272:
#line 877 "./yaccsrc.y"
    { yyval = yyvsp[-5]; /* fixes bugs like bug 579711 */
                                         yyval->p[0] = optgluelast( yyvsp[-4], yyvsp[-3] );
                                         yyval->p[0]->o.last = NULL;
                                         yyval->p[1] = optgluelast( yyvsp[-1], yyvsp[0] );
                                         yyval->p[1]->o.last = NULL; ;}
    break;

  case 273:
#line 883 "./yaccsrc.y"
    {  exiterror( ERR_THEN_UNEXPECTED, 0 ) ;;}
    break;

  case 274:
#line 884 "./yaccsrc.y"
    {  exiterror( ERR_THEN_EXPECTED, 2, parser_data.when_linenr, __reginatext )  ; ;}
    break;

  case 275:
#line 885 "./yaccsrc.y"
    {  exiterror( ERR_INVALID_EXPRESSION, 0 ) ;;}
    break;

  case 278:
#line 892 "./yaccsrc.y"
    { exiterror( ERR_WHEN_UNEXPECTED, 0 ); ;}
    break;

  case 279:
#line 895 "./yaccsrc.y"
    { yyval = yyvsp[-2];
                                         yyval->p[0] = optgluelast( yyvsp[-1], yyvsp[0] );
                                         if ( yyval->p[0] )
                                            yyval->p[0]->o.last = NULL; ;}
    break;

  case 280:
#line 899 "./yaccsrc.y"
    { yyval = makenode(X_NO_OTHERWISE,0) ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->charnr = parser_data.tstart ; ;}
    break;

  case 281:
#line 905 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_SIG_VAL ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 282:
#line 908 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 283:
#line 909 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->name = (streng *)yyvsp[0] ; ;}
    break;

  case 284:
#line 911 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 3, "ERROR FAILURE HALT NOTREADY NOVALUE SYNTAX LOSTDIGITS", __reginatext ) ;;}
    break;

  case 285:
#line 912 "./yaccsrc.y"
    { exiterror( ERR_INV_SUBKEYWORD, 4, "ERROR FAILURE HALT NOTREADY NOVALUE SYNTAX LOSTDIGITS", __reginatext ) ;;}
    break;

  case 286:
#line 913 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 287:
#line 914 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 3, __reginatext ) ;;}
    break;

  case 288:
#line 916 "./yaccsrc.y"
    { yyval = yyvsp[-3] ;
                                         yyval->type = X_SIG_SET ;
                                         yyval->p[0] = yyvsp[-2] ;
                                         yyval->name = (streng *)yyvsp[0] ;
                                         yyval->p[1] = yyvsp[-1] ; ;}
    break;

  case 289:
#line 921 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_SIG_SET ;
                                         yyval->p[0] = yyvsp[-1] ;
                                         yyval->p[1] = yyvsp[0] ; ;}
    break;

  case 290:
#line 925 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 291:
#line 926 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->type = X_SIG_SET ;
                                         yyval->p[0] = yyvsp[-1] ;
                                         yyval->p[1] = yyvsp[0] ; ;}
    break;

  case 292:
#line 932 "./yaccsrc.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 293:
#line 933 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 4, __reginatext );;}
    break;

  case 294:
#line 936 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue);;}
    break;

  case 295:
#line 937 "./yaccsrc.y"
    { exiterror( ERR_STRING_EXPECTED, 3, __reginatext ) ;;}
    break;

  case 296:
#line 940 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 297:
#line 941 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 298:
#line 944 "./yaccsrc.y"
    { yyval = makenode(X_ON,0) ; ;}
    break;

  case 299:
#line 947 "./yaccsrc.y"
    { yyval = makenode(X_OFF,0) ; ;}
    break;

  case 300:
#line 950 "./yaccsrc.y"
    { yyval = makenode(X_S_ERROR,0) ; ;}
    break;

  case 301:
#line 951 "./yaccsrc.y"
    { yyval = makenode(X_S_HALT,0) ; ;}
    break;

  case 302:
#line 952 "./yaccsrc.y"
    { yyval = makenode(X_S_NOTREADY,0) ; ;}
    break;

  case 303:
#line 953 "./yaccsrc.y"
    { yyval = makenode(X_S_FAILURE,0) ; ;}
    break;

  case 304:
#line 956 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 305:
#line 957 "./yaccsrc.y"
    { yyval = makenode(X_S_NOVALUE,0) ; ;}
    break;

  case 306:
#line 958 "./yaccsrc.y"
    { yyval = makenode(X_S_SYNTAX,0) ; ;}
    break;

  case 307:
#line 959 "./yaccsrc.y"
    { yyval = makenode(X_S_LOSTDIGITS,0) ; ;}
    break;

  case 308:
#line 962 "./yaccsrc.y"
    { yyval = yyvsp[-2] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 309:
#line 964 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[0] = yyvsp[0] ; ;}
    break;

  case 310:
#line 966 "./yaccsrc.y"
    { exiterror( ERR_EXTRA_DATA, 1, __reginatext ) ;;}
    break;

  case 311:
#line 967 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 312:
#line 971 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 313:
#line 975 "./yaccsrc.y"
    { yyval = yyvsp[-1] ;
                                         yyval->p[1] = yyvsp[0] ;
                                         /*
                                          * An assignment is a numerical
                                          * assignment if and only if we have
                                          * to do a numerical operation, which
                                          * is equivalent to the existence of
                                          * one more argument to $2.
                                          * This fixes bug 720166.
                                          */
                                         if (yyvsp[0] &&
                                             yyvsp[0]->p[0] &&
                                             gettypeof(yyvsp[0]) == IS_A_NUMBER)
                                            yyval->type = X_NASSIGN ; ;}
    break;

  case 314:
#line 991 "./yaccsrc.y"
    { yyval = makenode(X_ASSIGN,0) ;
                                         yyval->charnr = parser_data.tstart ;
                                         yyval->lineno = parser_data.tline ;
                                         yyval->p[0] = (nodeptr)create_head( (const char *)retvalue ); ;}
    break;

  case 315:
#line 998 "./yaccsrc.y"
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
                                         parendepth--; ;}
    break;

  case 316:
#line 1014 "./yaccsrc.y"
    { parendepth++;
                                         if ( parendepth == 1 )
                                         {
                                            /* exprs on as-is basis */
                                            yyval = yyvsp[0];
                                         }
                                         else
                                         {
                                            /* Must already be a plain expr.
                                             * The nexpr part of exprs detects
                                             * mistakes. */
                                            yyval = reduce_expr_list( yyvsp[0],
                                                                REDUCE_EXPR );
                                            if ( yyval == yyvsp[0] )
                                               exiterror( ERR_INTERPRETER_FAILURE, 1, __FILE__, __LINE__, "Reduction of `exprs' not happened." );
                                         }
                                       ;}
    break;

  case 317:
#line 1031 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 318:
#line 1032 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "+" );
                                         yyval = makenode( X_PLUSS, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 319:
#line 1034 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 320:
#line 1035 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "-" );
                                         yyval = makenode( X_MINUS, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 321:
#line 1037 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 322:
#line 1038 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "*" );
                                         yyval = makenode( X_MULT, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 323:
#line 1040 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "*" ); ;}
    break;

  case 324:
#line 1041 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 325:
#line 1042 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "/" );
                                         yyval = makenode( X_DEVIDE, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 326:
#line 1044 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "/" ); ;}
    break;

  case 327:
#line 1045 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 328:
#line 1046 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "//" );
                                         yyval = makenode( X_MODULUS, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 329:
#line 1048 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "//" ); ;}
    break;

  case 330:
#line 1049 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 331:
#line 1050 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "%" );
                                         yyval = makenode( X_INTDIV, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 332:
#line 1052 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "%" ); ;}
    break;

  case 333:
#line 1053 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 334:
#line 1054 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "|" );
                                         yyval = makenode( X_LOG_OR, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 335:
#line 1056 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "|" ); ;}
    break;

  case 336:
#line 1057 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 337:
#line 1058 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "&" );
                                         yyval = makenode( X_LOG_AND, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 338:
#line 1060 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "&" ); ;}
    break;

  case 339:
#line 1061 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 340:
#line 1062 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "&&" );
                                         yyval = makenode( X_LOG_XOR, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 341:
#line 1064 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "&&" ); ;}
    break;

  case 342:
#line 1065 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 343:
#line 1066 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "**" );
                                         yyval = makenode( X_EXP, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 344:
#line 1068 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "**" ); ;}
    break;

  case 345:
#line 1069 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 346:
#line 1070 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], " " );
                                         yyval = makenode( X_SPACE, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 347:
#line 1072 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, " " ); ;}
    break;

  case 348:
#line 1073 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 349:
#line 1074 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "||" );
                                         yyval = makenode( X_CONCAT, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 350:
#line 1076 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "||" ); ;}
    break;

  case 351:
#line 1077 "./yaccsrc.y"
    { AUTO_REDUCE( yyvsp[0], "\\" );
                                         yyval = makenode( X_LOG_NOT, 1, yyvsp[0] ); ;}
    break;

  case 352:
#line 1079 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\" ); ;}
    break;

  case 353:
#line 1080 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 354:
#line 1081 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "=" );
                                         yyval = makenode( X_EQUAL, 2, yyvsp[-3], yyvsp[0] );
                                         transform( yyval ); ;}
    break;

  case 355:
#line 1084 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "=" ); ;}
    break;

  case 356:
#line 1085 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 357:
#line 1086 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], ">=" );
                                         yyval = makenode( X_GTE, 2, yyvsp[-3], yyvsp[0] );
                                         transform( yyval ) ; ;}
    break;

  case 358:
#line 1089 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">=" ); ;}
    break;

  case 359:
#line 1090 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 360:
#line 1091 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "<=" );
                                         yyval = makenode( X_LTE, 2, yyvsp[-3], yyvsp[0] );
                                         transform( yyval ) ; ;}
    break;

  case 361:
#line 1094 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<=" ); ;}
    break;

  case 362:
#line 1095 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 363:
#line 1096 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], ">" );
                                         yyval = makenode( X_GT, 2, yyvsp[-3], yyvsp[0] );
                                         transform( yyval ) ; ;}
    break;

  case 364:
#line 1099 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">" ); ;}
    break;

  case 365:
#line 1100 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 366:
#line 1101 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "<" );
                                         yyval = makenode( X_LT, 2, yyvsp[-3], yyvsp[0] );
                                         transform( yyval ) ; ;}
    break;

  case 367:
#line 1104 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<" ); ;}
    break;

  case 368:
#line 1105 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 369:
#line 1106 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "\\=" );
                                         yyval = makenode( X_DIFF, 2, yyvsp[-3], yyvsp[0] );
                                         transform( yyval ) ; ;}
    break;

  case 370:
#line 1109 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\=" ); ;}
    break;

  case 371:
#line 1110 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 372:
#line 1111 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "==" );
                                         yyval = makenode( X_S_EQUAL, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 373:
#line 1113 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "==" ); ;}
    break;

  case 374:
#line 1114 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 375:
#line 1115 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "\\==" );
                                         yyval = makenode( X_S_DIFF, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 376:
#line 1117 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\==" ); ;}
    break;

  case 377:
#line 1118 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 378:
#line 1119 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], ">>" );
                                         yyval = makenode( X_S_GT, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 379:
#line 1121 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">>" ); ;}
    break;

  case 380:
#line 1122 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 381:
#line 1123 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "<<" );
                                         yyval = makenode( X_S_LT, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 382:
#line 1125 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<<" ); ;}
    break;

  case 383:
#line 1126 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 384:
#line 1127 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "\\>>" );
                                         yyval = makenode( X_S_NGT, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 385:
#line 1129 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\>>" ); ;}
    break;

  case 386:
#line 1130 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 387:
#line 1131 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "\\<<" );
                                         yyval = makenode( X_S_NLT, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 388:
#line 1133 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "\\<<" ); ;}
    break;

  case 389:
#line 1134 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 390:
#line 1135 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], ">>=" );
                                         yyval = makenode( X_S_GTE, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 391:
#line 1137 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, ">>=" ); ;}
    break;

  case 392:
#line 1138 "./yaccsrc.y"
    { parendepth--; ;}
    break;

  case 393:
#line 1139 "./yaccsrc.y"
    { parendepth++; AUTO_REDUCE( yyvsp[-3], "<<=" );
                                         yyval = makenode( X_S_LTE, 2, yyvsp[-3], yyvsp[0] ); ;}
    break;

  case 394:
#line 1141 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 1, "<<=" ); ;}
    break;

  case 395:
#line 1142 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 396:
#line 1143 "./yaccsrc.y"
    { yyval = makenode( X_STRING, 0 );
                                         yyval->name = Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 397:
#line 1145 "./yaccsrc.y"
    { yyval = makenode( X_STRING, 0 );
                                         yyval->name = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy(yyval->name->value,retvalue,
                                                    yyval->name->len=retlength); ;}
    break;

  case 398:
#line 1149 "./yaccsrc.y"
    { yyval = makenode( X_STRING, 0 );
                                         yyval->name = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy(yyval->name->value,retvalue,
                                                    yyval->name->len=retlength); ;}
    break;

  case 399:
#line 1153 "./yaccsrc.y"
    { yyval = makenode( X_STRING, 0 );
                                         yyval->name = Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 400:
#line 1155 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 401:
#line 1156 "./yaccsrc.y"
    { AUTO_REDUCE( yyvsp[0], NULL );
                                         yyval = makenode( X_U_PLUSS, 1, yyvsp[0] ); ;}
    break;

  case 402:
#line 1158 "./yaccsrc.y"
    { AUTO_REDUCE( yyvsp[0], NULL );
                                         yyval = makenode( X_U_MINUS, 1, yyvsp[0] ); ;}
    break;

  case 403:
#line 1160 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 0 ); ;}
    break;

  case 404:
#line 1161 "./yaccsrc.y"
    { exiterror( ERR_INVALID_EXPRESSION, 0 ); ;}
    break;

  case 405:
#line 1164 "./yaccsrc.y"
    { yyval = yyvsp[-1]; ;}
    break;

  case 406:
#line 1165 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_PARAN, 0 ); ;}
    break;

  case 407:
#line 1166 "./yaccsrc.y"
    { exiterror( ERR_UNMATCHED_PARAN, 0 ); ;}
    break;

  case 408:
#line 1169 "./yaccsrc.y"
    { yyval = (nodeptr)create_head( (const char *)retvalue ) ; ;}
    break;

  case 409:
#line 1173 "./yaccsrc.y"
    { yyval = makenode(X_EX_FUNC,1,yyvsp[0]) ;
                                         yyval->name = (streng *)yyvsp[-1] ; ;}
    break;

  case 410:
#line 1175 "./yaccsrc.y"
    { yyval = makenode(X_IN_FUNC,1,yyvsp[0]) ;
                                         yyval->name = (streng *)yyvsp[-1] ; ;}
    break;

  case 411:
#line 1179 "./yaccsrc.y"
    { /* ugly fake preservs parendepth */
                                         yyval = (YYSTYPE) parendepth;
                                         parendepth = 0; ;}
    break;

  case 412:
#line 1182 "./yaccsrc.y"
    { parendepth = (int) yyval;
                                         yyval = yyvsp[0]; ;}
    break;

  case 413:
#line 1186 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 414:
#line 1189 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 415:
#line 1192 "./yaccsrc.y"
    { yyval =makenode(X_TPL_SOLID,3,yyvsp[-2],yyvsp[-1],yyvsp[0]);;}
    break;

  case 416:
#line 1193 "./yaccsrc.y"
    { yyval =makenode(X_TPL_SOLID,1,yyvsp[0]) ; ;}
    break;

  case 417:
#line 1194 "./yaccsrc.y"
    { exiterror( ERR_INVALID_TEMPLATE, 1, __reginatext ) ;;}
    break;

  case 418:
#line 1197 "./yaccsrc.y"
    { yyval = makenode(X_NEG_OFFS,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 419:
#line 1199 "./yaccsrc.y"
    { yyval = makenode(X_POS_OFFS,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 420:
#line 1201 "./yaccsrc.y"
    { yyval = makenode(X_ABS_OFFS,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 421:
#line 1203 "./yaccsrc.y"
    { yyval = makenode(X_ABS_OFFS,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 422:
#line 1205 "./yaccsrc.y"
    { yyval = makenode(X_TPL_VAR,0) ;
                                         yyval->p[0] = yyvsp[-1] ; ;}
    break;

  case 423:
#line 1207 "./yaccsrc.y"
    { yyval = makenode(X_NEG_OFFS,0) ;
                                         yyval->p[0] = yyvsp[-1] ; ;}
    break;

  case 424:
#line 1209 "./yaccsrc.y"
    { yyval = makenode(X_POS_OFFS,0) ;
                                         yyval->p[0] = yyvsp[-1] ; ;}
    break;

  case 425:
#line 1211 "./yaccsrc.y"
    { yyval = makenode(X_ABS_OFFS,0) ;
                                         yyval->p[0] = yyvsp[-1] ; ;}
    break;

  case 426:
#line 1213 "./yaccsrc.y"
    { yyval = makenode(X_TPL_MVE,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 427:
#line 1217 "./yaccsrc.y"
    { yyval = (nodeptr)Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 428:
#line 1218 "./yaccsrc.y"
    { streng *sptr = Str_cre_TSD(parser_data.TSD,retvalue) ;
                                          if (myisnumber(sptr))
                                          {
                                             exiterror( ERR_INVALID_INTEGER, 4, sptr->value ) ;
                                          }
                                          else
                                             exiterror( ERR_INVALID_TEMPLATE, 0 ) ;;}
    break;

  case 429:
#line 1227 "./yaccsrc.y"
    { yyval = (nodeptr) Str_cre_TSD(parser_data.TSD,retvalue) ; ;}
    break;

  case 430:
#line 1228 "./yaccsrc.y"
    { streng *sptr = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy(sptr->value,retvalue,
                                                 sptr->len=retlength) ;
                                         yyval = (nodeptr) sptr ; ;}
    break;

  case 431:
#line 1232 "./yaccsrc.y"
    { streng *sptr = Str_make_TSD(parser_data.TSD,retlength) ;
                                         memcpy(sptr->value,retvalue,
                                                 sptr->len=retlength) ;
                                         yyval = (nodeptr) sptr ; ;}
    break;

  case 432:
#line 1238 "./yaccsrc.y"
    { yyval = makenode(X_TPL_POINT,1,yyvsp[0]) ; ;}
    break;

  case 433:
#line 1239 "./yaccsrc.y"
    { yyval = makenode(X_TPL_SYMBOL,2,yyvsp[0],yyvsp[-1]) ; ;}
    break;

  case 434:
#line 1240 "./yaccsrc.y"
    { yyval = NULL ; ;}
    break;

  case 435:
#line 1250 "./yaccsrc.y"
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
                                         yyvsp[-1] = reduce_expr_list( yyvsp[-1],
                                                                REDUCE_EXPR );

                                         /* detect "call s (a,b),. b" and
                                          * "call s (),. b", but every list on
                                          * the left side of "," is wrong, so
                                          * complain about every exprlist.   */
                                         if ( IS_EXPRLIST( yyvsp[-1] ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 1 );

                                         yyvsp[-1] = reduce_expr_list( yyvsp[-1],
                                                              REDUCE_SUBEXPR );
                                       ;}
    break;

  case 436:
#line 1293 "./yaccsrc.y"
    { assert( IS_EXPRLIST( yyvsp[0] ) );

                                         /* detect ",()." */
                                         if ( IS_EXPRLIST( yyvsp[0]->p[0] )
                                           && ( yyvsp[0]->p[1] == NULL )
                                           && ( yyvsp[0]->p[0]->p[0] == NULL ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 0 );

                                         /* detect ",(a,b)." */
                                         if ( IS_EXPRLIST( yyvsp[0]->p[0] )
                                           && ( yyvsp[0]->p[1] == NULL )
                                           && IS_EXPRLIST( yyvsp[0]->p[0]->p[1] ) )
                                            exiterror( ERR_UNEXPECTED_PARAN, 1 );

                                         /* detect ",(a)." and transform it
                                          * to ",a."                         */
                                         yyvsp[0] = reduce_expr_list( yyvsp[0],
                                                                REDUCE_RIGHT );
                                         assert( IS_EXPRLIST( yyvsp[0] ) );

                                         /* Detect something like
                                          * "call s (a,b)+1"                 */
                                         current = yyvsp[0]->p[0];
                                         if ( ( current != NULL )
                                           && !IS_EXPRLIST( current )
                                           && !IS_FUNCTION( current )
                                           && ( IS_EXPRLIST( current->p[0] )
                                             || IS_EXPRLIST( current->p[1] ) ) )
                                            exiterror( ERR_INVALID_EXPRESSION, 0 );

                                         yyval = makenode( X_EXPRLIST, 2, yyvsp[-3], yyvsp[0] );
                                         checkconst( yyval ); ;}
    break;

  case 437:
#line 1325 "./yaccsrc.y"
    { /* detect
                                          * "x = ()."
                                          * "x = f(().)"
                                          * "call s (().)"
                                          * "CALL s ()."                     */
                                         if ( ( parendepth < 0 ) && ( yyvsp[0] == NULL ) )
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

                                         yyvsp[0] = reduce_expr_list( yyvsp[0],
                                                              REDUCE_SUBEXPR );
                                         yyval = makenode( X_EXPRLIST, 1, yyvsp[0] );
                                         checkconst( yyval );
                                        ;}
    break;

  case 438:
#line 1369 "./yaccsrc.y"
    { yyval = yyvsp[0] ; ;}
    break;

  case 439:
#line 1370 "./yaccsrc.y"
    { yyval = NULL ; ;}
    break;

  case 440:
#line 1373 "./yaccsrc.y"
    { yyval = makenode(X_SIM_SYMBOL,1,yyvsp[0]) ;
                                         yyval->name = (streng *) yyvsp[-1] ; ;}
    break;

  case 441:
#line 1375 "./yaccsrc.y"
    { yyval = makenode(X_SIM_SYMBOL,0) ;
                                         yyval->name = (streng *) yyvsp[0] ; ;}
    break;

  case 442:
#line 1377 "./yaccsrc.y"
    { yyval = makenode(X_IND_SYMBOL,1,yyvsp[0]) ;
                                         yyval->name = (streng *) yyvsp[-2] ; ;}
    break;

  case 443:
#line 1379 "./yaccsrc.y"
    { yyval = makenode(X_IND_SYMBOL,0) ;
                                         yyval->name = (streng *) yyvsp[-1] ; ;}
    break;

  case 444:
#line 1383 "./yaccsrc.y"
    { yyval = (treenode *) Str_cre_TSD(parser_data.TSD,retvalue);;}
    break;

  case 445:
#line 1386 "./yaccsrc.y"
    { yyval = (treenode *) Str_cre_TSD(parser_data.TSD,retvalue);;}
    break;

  case 446:
#line 1387 "./yaccsrc.y"
    { exiterror( ERR_SYMBOL_EXPECTED, 1, __reginatext ) ;;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 4724 "./yaccsrc.c"

  yyvsp -= yylen;
  yyssp -= yylen;


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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1390 "./yaccsrc.y"


static nodeptr makenode( int type, int numb, ... )
{
   nodeptr thisleave ;
   va_list argptr ;
   int i ;

   assert(numb <= sizeof(thisleave->p)/sizeof(thisleave->p[0]));
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

static void checkdosyntax( cnodeptr this )
{
   if ((this->p[1]!=NULL)&&(this->p[2]!=NULL))
   {
      if ((this->p[1]->type)==(this->p[2]->type))
      {
         exiterror( ERR_INVALID_DO_SYNTAX, 1, getdokeyword(this->p[1]->type) )  ;
      }
   }
   if ((this->p[2]!=NULL)&&(this->p[3]!=NULL))
   {
      if ((this->p[2]->type)==(this->p[3]->type))
      {
         exiterror( ERR_INVALID_DO_SYNTAX, 1, getdokeyword(this->p[2]->type) )  ;
      }
   }
   if ((this->p[1]!=NULL)&&(this->p[3]!=NULL))
   {
      if ((this->p[1]->type)==(this->p[3]->type))
      {
         exiterror( ERR_INVALID_DO_SYNTAX, 1, getdokeyword(this->p[1]->type) )  ;
      }
   }
   return ;
}


void newlabel( const tsd_t *TSD, internal_parser_type *ipt, nodeptr this )
{
   labelboxptr new ;

   assert( this ) ;

   new = MallocTSD(sizeof(labelbox)) ;

   new->next = NULL ;
   new->entry = this ;
   if (ipt->first_label == NULL)
   {
      ipt->first_label = new ;
      ipt->last_label = new ; /* must be NULL, too */
   }
   else
   {
      ipt->last_label->next = new ;
      ipt->last_label = new ;
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


static node_type gettypeof( nodeptr this )
{
   tsd_t *TSD = parser_data.TSD;

   switch(this->type)
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
         if (this->u.number)      /* FIXME: When does this happen? */
            return IS_A_NUMBER ;

         if ( ( this->u.number = is_a_descr( TSD, this->name ) ) != NULL )
            return IS_A_NUMBER;
         return IS_NO_NUMBER;
      }
   }
   return IS_UNKNOWN ;
}



static void transform( nodeptr this )
{
   int type ;
   node_type left,rght;

   left = gettypeof( this->p[0] ) ;
   rght = gettypeof( this->p[1] ) ;
   type = this->type ;

   if ( ( left == IS_A_NUMBER ) && ( rght == IS_A_NUMBER ) )
   {
      if (type==X_EQUAL)
         this->type = X_NEQUAL ;
      else if (type==X_DIFF)
         this->type = X_NDIFF ;
      else if (type==X_GTE)
         this->type = X_NGTE ;
      else if (type==X_GT)
         this->type = X_NGT ;
      else if (type==X_LTE)
         this->type = X_NLTE ;
      else if (type==X_LT)
         this->type = X_NLT ;
   }
   else if ( ( left == IS_NO_NUMBER ) || ( rght == IS_NO_NUMBER ) )
   {
      if (type==X_EQUAL)
         this->type = X_SEQUAL ;
      else if (type==X_DIFF)
         this->type = X_SDIFF ;
      else if (type==X_GTE)
         this->type = X_SGTE ;
      else if (type==X_GT)
         this->type = X_SGT ;
      else if (type==X_LTE)
         this->type = X_SLTE ;
      else if (type==X_LT)
         this->type = X_SLT ;
   }
   else
   {
      type = this->p[0]->type ;
      if ( ( left == IS_A_NUMBER )
        && ( ( type == X_STRING ) || ( type == X_CON_SYMBOL ) ) )
         this->u.flags.lnum = 1 ;
      else if ( left == IS_SIM_SYMBOL )
         this->u.flags.lsvar = 1 ;
      else if ( left == IS_COMP_SYMBOL )
         this->u.flags.lcvar = 1 ;

      type = this->p[1]->type ;
      if ( ( rght == IS_A_NUMBER )
        && ( ( type == X_STRING ) || ( type == X_CON_SYMBOL ) ) )
         this->u.flags.rnum = 1 ;
      else if ( rght == IS_SIM_SYMBOL )
         this->u.flags.rsvar = 1 ;
      else if ( rght == IS_COMP_SYMBOL )
         this->u.flags.rcvar = 1 ;
   }
}


static int is_const( cnodeptr this )
{
   if (!this)
      return 1 ;

   switch (this->type)
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
         return is_const( this->p[0] ) ;

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
         return is_const( this->p[0] ) && is_const( this->p[1] ) ;
   }
   return 0 ;
}


static void checkconst( nodeptr this )
{
   tsd_t *TSD = parser_data.TSD;

   assert( this->type == X_EXPRLIST ) ;
   if (is_const(this->p[0]))
   {
      if (this->p[0])
         this->u.strng = evaluate( TSD, this->p[0], NULL ) ;
      else
         this->u.strng = NULL ;

      this->type = X_CEXPRLIST ;
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
static nodeptr reduce_expr_list( nodeptr this, reduce_mode mode )
{
   nodeptr h, retval = this;

   if ( !this )
      return retval;

   if ( mode == REDUCE_SUBEXPR )
   {
      if ( ( parendepth == 1 ) && !IS_FUNCTION( this ) && !IS_EXPRLIST( this ) )
      {
         if ( IS_EXPRLIST( this->p[0] ) )
         {
            h = this->p[0];
            if ( ( h->p[0] == NULL ) || ( h->p[1] != NULL ) )
               exiterror( ERR_INVALID_EXPRESSION, 0 );
            this->p[0] = h->p[0];
            RejectNode( h );
         }
         if ( IS_EXPRLIST( this->p[1] ) )
         {
            h = this->p[1];
            if ( ( h->p[0] == NULL ) || ( h->p[1] != NULL ) )
               exiterror( ERR_INVALID_EXPRESSION, 0 );
            this->p[1] = h->p[0];
            RejectNode( h );
         }
      }
      return retval;
   }

   if ( !IS_EXPRLIST( this ) )
      return retval;

   if ( ( mode == REDUCE_CALL ) || ( mode == REDUCE_RIGHT ) )
   {
      if ( IS_EXPRLIST( this->p[0] ) && ( this->p[1] == NULL ) )
      {
         retval = this->p[0];
         RejectNode( this );
      }
   }
   else
   {
      /*
       * mode == REDUCE_EXPR
       */
      if ( ( this->p[0] != NULL ) && ( this->p[1] == NULL ) )
      {
         if ( !IS_EXPRLIST( this->p[0] ) )
         {
            retval = this->p[0];
            RejectNode( this );
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

