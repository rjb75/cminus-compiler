#include "bison.h"

extern declaration_node *parse2(parser_main *parser);

int yylex(YYSTYPE *lvalp, parser_main *parser)
{
    if (parser->current_token == NULL)
    {
        return -1;
    }
    while (parser->current_token->token_id == TOKEN_COMMENT && parser->current_token != NULL)
    {
        parser->current_token = parser->current_token->next_token;
    }
    if (parser->current_token == NULL)
    {
        return -1;
    }
    int token = getTokenFromScanner(parser->current_token);
    parser->line_no = parser->current_token->line_start;
    if (parser->current_token->token_id == TOKEN_NUM)
    {
        scanner_token *tk = parser->current_token;
        char *tmp = malloc((tk->token_len) * sizeof(char));
        memcpy(tmp, tk->token_ptr, tk->token_len);
        lvalp->num = atoi(tmp);
        free(tmp);
    }
    else if (parser->current_token->token_id == TOKEN_ID)
    {
        scanner_token *tk = parser->current_token;
        char *tmp = malloc((tk->token_len + 0) * sizeof(char));
        memcpy(tmp, tk->token_ptr, tk->token_len);
        tmp[tk->token_len] = '\0';
        lvalp->id = tmp;
    }
    parser->current_token = parser->current_token->next_token;
    return token;
}

int32_t parse(parser_main *parser)
{
    declaration_node *root = parse2(parser);
    if (root == NULL)
    {
        return -1;
    }
    parser->root = root;
    return 1;
}
