/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     PLUS = 258,
     MINUS = 259,
     MULTIPLY = 260,
     DIVIDE = 261,
     LESSTHAN = 262,
     LESSTHANEQUAL = 263,
     GREATERTHAN = 264,
     GREATERTHANEQUAL = 265,
     EQUALEQUAL = 266,
     NOTEQUAL = 267,
     EQUAL = 268,
     SEMICOLON = 269,
     COMMA = 270,
     O_PAREN = 271,
     C_PAREN = 272,
     O_BRACK = 273,
     C_BRACK = 274,
     O_BRACE = 275,
     C_BRACE = 276,
     ID = 277,
     NUM = 278,
     ELSE = 279,
     IF = 280,
     INT = 281,
     RETURN = 282,
     VOID = 283,
     WHILE = 284,
     ERROR = 285,
   };
#endif
/* Tokens.  */
#define PLUS 258
#define MINUS 259
#define MULTIPLY 260
#define DIVIDE 261
#define LESSTHAN 262
#define LESSTHANEQUAL 263
#define GREATERTHAN 264
#define GREATERTHANEQUAL 265
#define EQUALEQUAL 266
#define NOTEQUAL 267
#define EQUAL 268
#define SEMICOLON 269
#define COMMA 270
#define O_PAREN 271
#define C_PAREN 272
#define O_BRACK 273
#define C_BRACK 274
#define O_BRACE 275
#define C_BRACE 276
#define ID 277
#define NUM 278
#define ELSE 279
#define IF 280
#define INT 281
#define RETURN 282
#define VOID 283
#define WHILE 284
#define ERROR 285

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 103 "parser.y"
{
    struct decl *decl;
    struct expr *expr;
    struct type *type;
    char *id;
    int num;
}

#line 112 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;


