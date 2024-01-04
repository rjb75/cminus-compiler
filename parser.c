#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "ast.h"
#include "common.h"
#include "logger.h"
#include "pr.h"
#include "file.h"
#include "bison.h"

int32_t parser_init(parser_main *parser)
{
    parser->tokens = NULL;
    parser->root = NULL;
    parser->debug_mode = 0;
    strcpy(parser->out_file_name, "parser.out");
    return 1;
}

int32_t parser_cleanup(parser_main *parser)
{
    return 1;
}

int getTokenFromScanner(scanner_token *token)
{
    switch (token->token_id)
    {
    case SYMBOL_PLUS:
        return PLUS;
    case SYMBOL_MINUS:
        return MINUS;
    case SYMBOL_MULTIPLY:
        return MULTIPLY;
    case SYMBOL_DIVIDE:
        return DIVIDE;
    case SYMBOL_LESSTHAN:
        return LESSTHAN;
    case SYMBOL_LESSTHANEQUAL:
        return LESSTHANEQUAL;
    case SYMBOL_GREATERTHAN:
        return GREATERTHAN;
    case SYMBOL_GREATERTHANEQUAL:
        return GREATERTHANEQUAL;
    case SYMBOL_EQUALEQUAL:
        return EQUALEQUAL;
    case SYMBOL_NOTEQUAL:
        return NOTEQUAL;
    case SYMBOL_EQUAL:
        return EQUAL;
    case SYMBOL_SEMICOLON:
        return SEMICOLON;
    case SYMBOL_COMMA:
        return COMMA;
    case SYMBOL_PARENTHESISOPEN:
        return O_PAREN;
    case SYMBOL_PARENTHESISCLOSE:
        return C_PAREN;
    case SYMBOL_BRACKETOPEN:
        return O_BRACK;
    case SYMBOL_BRACKETCLOSE:
        return C_BRACK;
    case SYMBOL_BRACESOPEN:
        return O_BRACE;
    case SYMBOL_BRACESCLOSE:
        return C_BRACE;
    case KEYWORD_ELSE:
        return ELSE;
    case KEYWORD_IF:
        return IF;
    case KEYWORD_INT:
        return INT;
    case KEYWORD_RETURN:
        return RETURN;
    case KEYWORD_VOID:
        return VOID;
    case KEYWORD_WHILE:
        return WHILE;
    case SCANNER_ID:
        return ID;
    case SCANNER_NUM:
        return NUM;
    default:
        fprintf(stderr, "got error %d\n", token->token_id);
        return ERROR;
    }
}

int32_t parser_write_file(parser_main *parser)
{
    FILE *file = NULL;
    if (!open_file(parser->out_file_name, &file))
    {
        return -1;
    }

    if (parser->root != NULL)
    {
        printAST(parser->root, file);
    }

    if (!close_file(&file))
    {
        return -1;
    }

    return 1;
}

#ifdef MODE_PARSER
int main(int argc, char *argv[])
{
    scanner_main scanner;
    scanner_init(&scanner);
    if (!cli_parser(&scanner, argc, argv))
    {
        goto end;
    }

    if (!scanner_tokenizer(&scanner))
    {
        LogError(__FUNCTION__, __LINE__, "Error Scanning");
        goto end;
    }

    parser_main parser;

    parser_init(&parser);
    parser.tokens = scanner.tokens;
    parser.current_token = scanner.tokens;
    parser.token_len = scanner.token_len;
    parser.debug_mode = scanner.debug_mode;
    strncpy(parser.file_name, scanner.file_name, 128);
    strncpy(parser.out_file_name, scanner.out_file_name, 128);
    if (!parse(&parser))
    {
        LogError(__FUNCTION__, __LINE__, "Error Parsing");
        goto end;
    }

    if(parser.debug_mode) {
        printAST(parser.root, stdout);
    }

    parser_write_file(&parser);

end:
    parser_cleanup(&parser);
    scanner_cleanup(&scanner);
    return 0;
}
#endif
