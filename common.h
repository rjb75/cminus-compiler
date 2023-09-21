#ifndef COMMON_H
#define COMMON_H

/*
 * This enum defines all of the keywords in c-minus
 */
enum cminus_keyword {
    KEYWORD_NONE,       // reserved for no keyword
    KEYWORD_ELSE,       // else
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
    SYMBOL_PLUS,
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

