#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>
#include <ctype.h>

/*
 * This enum is used to identify the type of the token
 */
enum scanner_token_type {
    SCANNER_NONE,       // reserved for no token
    SCANNER_KEYWORD,    // a keyword
    SCANNER_SYMBOL,     // a symbol
    SCANNER_ID,         // a id
    SCANNER_NUM,        // a number
    SCANNER_COMMENT     // a comment
};

/*
 * This enum is used to denote the current scanner state
 */
enum scanner_state {
    NONE,
    INNUM,
    INID,
    INASSIGN,
    INCOMMENT
};

typedef struct scanner_token_s scanner_token;

/*
 * This struct holds the data required for each token
 */
struct scanner_token_s {
    enum scanner_token_type token_type;
    enum cminus_keyword keyword;
    char* token_ptr;
    int32_t token_len;
    int32_t token_start;
    int32_t token_end;
    scanner_token* next_token;
};

/*
 * This struct holds the data required for an instance of the scanner
 */
struct scanner_main_s {
    char file_name[128];
    char *data;
    int32_t data_len;
    scanner_token *tokens;
    int32_t token_len;
};

/*
 * This type is defined by a struct that holds the scanner instance data
 */
typedef struct scanner_main_s scanner_main;

#endif

