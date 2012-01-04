
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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

extern YYSTYPE __reginalval;


