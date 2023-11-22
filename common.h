#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>

extern int lineno; /* source line number for listing */

// make int so multiple enums can be used
typedef int TokenType;

enum cminus_token {
    TOKEN_NONE,
    TOKEN_ERROR,
    TOKEN_ENDOFFILE,
    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_COMMENT,
};

/*
 * This enum defines all of the keywords in c-minus
 */
enum cminus_keyword {
    KEYWORD_NONE = 0,       // reserved for no keyword
    KEYWORD_ELSE = TOKEN_COMMENT + 1,       // else
    KEYWORD_IF,         // if
    KEYWORD_INT,        // int
    KEYWORD_RETURN,     // return
    KEYWORD_VOID,       // void
    KEYWORD_WHILE       // while
};

/*
 * This enum defines all of the symbols in c-minus
 */
enum cminus_symbol {
    SYMBOL_NONE,
    SYMBOL_PLUS = KEYWORD_WHILE + 1,
    SYMBOL_MINUS,
    SYMBOL_MULTIPLY,
    SYMBOL_DIVIDE,
    SYMBOL_LESSTHAN,
    SYMBOL_LESSTHANEQUAL,
    SYMBOL_GREATERTHAN,
    SYMBOL_GREATERTHANEQUAL,
    SYMBOL_EQUALEQUAL,
    SYMBOL_NOTEQUAL,
    SYMBOL_EQUAL,
    SYMBOL_SEMICOLON,
    SYMBOL_COMMA,
    SYMBOL_PARENTHESISOPEN,
    SYMBOL_PARENTHESISCLOSE,
    SYMBOL_BRACKETOPEN,
    SYMBOL_BRACKETCLOSE,
    SYMBOL_BRACESOPEN,
    SYMBOL_BRACESCLOSE,
};

#endif

