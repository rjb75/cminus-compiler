#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "scanner.h"

typedef struct parser_main_s parser_main;

struct parser_main_s
{
    scanner_token *tokens;
    scanner_token *current_token;
    int token_len;
    int line_no;
    declaration_node *root;
    int32_t debug_mode;
    char file_name[128];
    char out_file_name[128]; 
};

extern parser_main parser;

int32_t parser_init(parser_main* parser);
int32_t parser_write_file(parser_main* parser);
int32_t parser_cleanup(parser_main* parser);
int32_t parse(parser_main* parser);
int getTokenFromScanner(scanner_token *token);
extern int savedLineNo;

#endif
