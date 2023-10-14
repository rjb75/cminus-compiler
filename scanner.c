#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "scanner.h"
#include "logger.h"
#include "file.h"

const char* token_name(scanner_token token) {
    switch (token.token_type) {
        case SCANNER_ID:
            return "ID";
        case SCANNER_NUM:
            return "NUM";
        default:
            return "";
    }

}

int cli_parser(scanner_main* scanner, int argc, char *argv[]) {
    if(argc < 1) {
        return -1;
    } else if(argc == 1) {
        LogError(__FUNCTION__, __LINE__, "Error parsing arguments");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-o") == 0) {
            if(i < argc) {
                char* filename = argv[i+1];
                if(strlen(filename) > 127) {
                    LogError(__FUNCTION__, __LINE__, "Output file name too long");
                    goto end;
                }
                strcpy(scanner->out_file_name, filename);
                i++;
            }
        } else if(strcmp(argv[i], "-d") == 0) {
            scanner->debug_mode = 1;
        }

        // handle filename
        if(i == argc - 1) {
            char* filename = argv[i];
            if(strlen(filename) > 127) {
                LogError(__FUNCTION__, __LINE__, "File name too long");
                goto end;
            }
            strcpy(scanner->file_name, filename);
        }
    }
    
    int32_t readbytes = read_file(scanner->file_name, NULL, 0);
    
    if(readbytes <1) {
        LogError(__FUNCTION__, __LINE__, "Error reading file");
        goto end;
    }
    
    scanner->data = malloc(readbytes * sizeof(char));

    if(scanner->data == NULL) {
        LogError(__FUNCTION__, __LINE__, "Error allocating data");
        goto end;
    }
    
    if(read_file(scanner->file_name, scanner->data, readbytes) < 0) {
        LogError(__FUNCTION__, __LINE__, "Error copying data");
        goto end;
    }

    scanner->data_len = readbytes;

end:
    return 1;
}

int32_t scanner_init(scanner_main* scanner) {
    scanner->data = NULL;
    scanner->tokens = NULL;
    scanner->token_len = 0;
    scanner->debug_mode = 0;
    strcpy(scanner->out_file_name, "scanner.out");

    return 1;
}

const char* symbol_name(enum cminus_symbol symbol) {
    switch(symbol) {
        case SYMBOL_PLUS:
            return "ADD";
        case SYMBOL_MINUS:
            return "SUB";
        case SYMBOL_MULTIPLY:
            return "MULT";
        case SYMBOL_DIVIDE:
            return "DIV";
        case SYMBOL_LESSTHAN:
            return "LT";
        case SYMBOL_LESSTHANEQUAL:
            return "LESS_EQUAL";
        case SYMBOL_GREATERTHAN:
            return "GT";
        case SYMBOL_GREATERTHANEQUAL:
            return "GREATER_EQUAL";
        case SYMBOL_EQUALEQUAL:
            return "EQ";
        case SYMBOL_NOTEQUAL:
            return "NEQ";
        case SYMBOL_EQUAL:
            return "SET";
        case SYMBOL_SEMICOLON:
            return "SEM_COL";
        case SYMBOL_COMMA:
            return "COMMA";
        case SYMBOL_PARENTHESISOPEN:
            return "O_PAREN";
        case SYMBOL_PARENTHESISCLOSE:
            return "C_PAREN";
        case SYMBOL_BRACKETOPEN:
            return "O_BRACK";
        case SYMBOL_BRACKETCLOSE:
            return "C_BRACK";
        case SYMBOL_BRACESOPEN:
            return "O_BRACE";
        case SYMBOL_BRACESCLOSE:
            return "C_BRACE";
        default:
            return "";
    }
}

const char* keyword_name(enum cminus_keyword keyword) {
    switch(keyword) {
        case KEYWORD_ELSE:
            return "ELSE";
        case KEYWORD_IF:
            return "IF";
        case KEYWORD_INT:
            return "INT";
        case KEYWORD_RETURN:
            return "RETURN";
        case KEYWORD_VOID:
            return "VOID";
        case KEYWORD_WHILE:
            return "WHILE";
        default:
            return "";
    }
}

char* format_token_string(scanner_token token) {
    int32_t len = 0, max_len = 128;
    char *output = malloc((max_len+1)*sizeof(char)), *cursor = output;
    memset(output, 0, (max_len+1)*sizeof(char));
    
    len += sprintf(output, "%d: ", token.line_start);
    cursor = &output[len];

    if(token.token_type == SCANNER_NUM) {
        char *tmp = malloc((token.token_len)* sizeof(char));
        memcpy(tmp, token.token_ptr, token.token_len);
        tmp[token.token_len] = '\0';
        len = sprintf(cursor, "%s\t\"%s\"", token_name(token), tmp);
        free(tmp);
    } else if(token.token_type == SCANNER_ID) {
        char *tmp = malloc((token.token_len + 1)* sizeof(char));
        memcpy(tmp, token.token_ptr, token.token_len);
        tmp[token.token_len] = '\0';
        len = sprintf(cursor, "%s\t\"%s\"", token_name(token), tmp);
        free(tmp);
    } else if(token.token_type == SCANNER_SYMBOL) {
        len = sprintf(cursor, "%s", symbol_name(token.symbol));
    } else if(token.token_type == SCANNER_COMMENT) {
        len = sprintf(cursor, "COMMENT");
    } else if(token.token_type == SCANNER_KEYWORD) {
        len = sprintf(cursor, "%s", keyword_name(token.keyword));
    }

    cursor = &output[len];
    
    return output;
}

int32_t scanner_cleanup(scanner_main* scanner) {
    int32_t status = -1;

    if(scanner->tokens != NULL) {
        LogDebug(__FUNCTION__, __LINE__, "Freeing token data");
        scanner_token** token_ptr = &scanner->tokens;
        while(*token_ptr != NULL) {
            scanner_token* temp = (*token_ptr)->next_token;
            free(*token_ptr);
            *token_ptr = temp;
        }
    }
    
    if(scanner->data != NULL) {
        LogDebug(__FUNCTION__, __LINE__, "Freeing scanner data");
        free(scanner->data);
    }

    status = 1;

    return status;
}

int32_t scanner_write_file(scanner_main* scanner) {
    int32_t status = -1;
    FILE* file = NULL;

    if(!open_file(scanner->out_file_name, &file)) {
        return -1;
    }

    if(scanner->tokens != NULL) {
        scanner_token* token_ptr = scanner->tokens;
        char *tok_str = NULL;
        while(token_ptr != NULL) {
            if(token_ptr->token_type != SCANNER_COMMENT) {
                tok_str = format_token_string(*token_ptr);
                write_file(file, tok_str);
                write_file(file, "\n");
                free(tok_str);
            }
            token_ptr = token_ptr->next_token;
        }
    }

    if(!close_file(&file)) {
        return -1;
    }
    
    status = 1;

    return status;
}

int32_t add_token(scanner_main* scanner, scanner_token* token) {

    scanner_token** token_ptr = &scanner->tokens;
    
    if(*token_ptr == NULL) {
        *token_ptr = token;
        scanner->token_len++;
        return 0;
    }
    
    while((*token_ptr)->next_token != NULL) {
        token_ptr = &(*token_ptr)->next_token;
    }
    
    scanner_token* to_add = *token_ptr;
    to_add->next_token = token;

    if(token->token_type != SCANNER_COMMENT && scanner->debug_mode > 0) {
        char *tok_str = NULL;
        tok_str = format_token_string(**token_ptr);
        printf("%s\n", tok_str);
        free(tok_str);
    }

    return 0;
}


int32_t handle_common(scanner_main* scanner, int32_t* position, int32_t* line) {
    char* current_char = &scanner->data[*position];

    switch(*current_char) {
        case '\n':
            *line = *line + 1;
        case ' ':
        case '\t':
            return 1;
    }
    return 0;
}

int32_t handle_unknown(scanner_main* scanner, int32_t* position, int32_t* line) {
    char* current_char = &scanner->data[*position];

    fprintf(stderr, "warning skipping unknown character \'%c\' at or near line %d\n",
            *current_char, *line);

    return 1;
}

int32_t handle_comment(scanner_main* scanner, int32_t* position, int32_t* line) {
    int32_t status = 0, start = *position, end = *position, length = 0,
            start_line = *line, end_line = *line;
    char* current_char = &scanner->data[*position];
    scanner_token* token = NULL;
    
    if(*current_char != '/') {
        return status;
    }

    if(*position + 1 >= scanner->data_len) {
        return status;
    }
    
    *position = *position + 1;
    current_char = &scanner->data[*position];
    
    if(*current_char != '*') {
        return status;
    }

    *position = *position + 1;
    current_char = &scanner->data[*position];

scan:
    while(*current_char != '*' && *position < scanner->data_len)
    {
        if(*current_char == '\n') {
            *line = *line + 1;
        }
        *position = *position + 1;
        current_char = &scanner->data[*position];
    }

    if(*position == scanner->data_len) {
        fprintf(stderr, "unterminated comment at or near line %d\n", *line);
        return -1;
    }
    
    current_char = &scanner->data[*position + 1];
    
    if(*position >= scanner->data_len) {
        return -1;
    }
    else if(*current_char != '/') {
        goto scan;
    }

    *position = *position + 1;
    status = 1;

    end = *position;
    end_line = *line;
    length = end - start;
    token = malloc(sizeof(scanner_token));
    token->token_type = SCANNER_COMMENT;
    token->token_start = start;
    token->token_end = end;
    token->token_len = length;
    token->next_token = NULL;
    token->token_ptr = &scanner->data[start];
    token->line_start = start_line;
    token->line_end = end_line;
    add_token(scanner, token); 
    
    return status;
}

int32_t check_keyword(scanner_main* scanner, int32_t* position, int32_t* line) {
    int32_t start_pos = *position;
    enum cminus_keyword keyword = KEYWORD_NONE;

    char* current_char = &scanner->data[*position];
    start_pos = *position;
    
    switch(*current_char) {
        case 'e':
            if(strncmp("else", current_char, 4 * sizeof(char)) != 0) {
                return 0;
            }
            keyword = KEYWORD_ELSE;
            *position = *position + 4;
            break;
        case 'i':
            if(strncmp(current_char, "if", 2) == 0) {
                keyword = KEYWORD_IF;
                *position = *position + 2;
                break;
            } else if(strncmp(current_char, "int", 3) == 0) {
                keyword = KEYWORD_INT;
                *position = *position + 3;
                break;
            }
            return 0;
        case 'r':
            if(strncmp(current_char, "return", 6) != 0) {
                return 0;
            }
            keyword = KEYWORD_RETURN;
            *position = *position + 6;
            break;
        case 'v':
            if(strncmp(current_char, "void", 4) != 0) {
                return 0;
            }
            keyword = KEYWORD_VOID;
            *position = *position + 4;
            break;
        case 'w':
            if(strncmp(current_char, "while", 5) != 0) {
                return 0;
            }
            keyword = KEYWORD_WHILE;
            *position = *position + 5;
            break;
        default:
            return 0;
        }

        current_char = &scanner->data[*position];
        
        if(isalpha(*current_char)) {
            *position = start_pos;
            return 0;
        }

        scanner_token* token = malloc(sizeof(scanner_token));
        token->token_type = SCANNER_KEYWORD;
        token->keyword = keyword;
        token->next_token = NULL;
        token->token_start = start_pos;
        token->token_end = *position;
        token->token_len = *position - start_pos;
        token->token_ptr = &scanner->data[start_pos];
        token->line_start = *line;
        token->line_end = *line;
        
        add_token(scanner, token);
        *position = *position - 1;
        return 1;

}

int32_t check_id(scanner_main* scanner, int32_t* position, int32_t* line) {
    int32_t start_pos = *position, length = 0, end_pos = *position;
    char* current_char = &scanner->data[*position];
    start_pos = *position;

    if(check_keyword(scanner, position, line)) {
        return 1;
    }

    while(*position < scanner->data_len) {
        current_char = &scanner->data[*position];
        if(!isalpha(*current_char)) {
            if(isdigit(*current_char)) {
                char *idstr = NULL;
                int32_t id_len = *position - start_pos + 1;
                idstr = malloc((id_len+ 1) * sizeof(char));
                memcpy(idstr, &scanner->data[start_pos], id_len);
                idstr[id_len] = '\0';
                fprintf(stderr, "error: invalid ID %s may not contain digits at or near line 2\n", idstr);
                return -1;
            }
            *position = *position - 1;
            end_pos = *position;
            break;
        }
        *position = *position + 1;
        length++;
    }

    scanner_token* token = malloc(sizeof(scanner_token));
    token->token_type = SCANNER_ID;
    token->next_token = NULL;
    token->token_start = start_pos;
    token->token_end = end_pos;
    token->token_len = length;
    token->token_ptr = &scanner->data[start_pos];
    token->line_start = *line;
    token->line_end = *line;

    add_token(scanner, token);

    return 1;
}

int32_t check_number(scanner_main* scanner, int32_t* position, int32_t* line) {
    int32_t start_pos = *position, length = 1, end_pos = *position;
    char* current_char = &scanner->data[*position];
    start_pos = *position;
    *position = *position + 1;

    while(*position < scanner->data_len) {
        current_char = &scanner->data[*position];
        if(!isdigit(*current_char)) {
            *position = *position - 1;
            end_pos = *position;
            break;
        }
        *position = *position + 1;
        length++;
    }
    
    scanner_token* token = malloc(sizeof(scanner_token));
    token->token_type = SCANNER_NUM;
    token->next_token = NULL;
    token->token_start = start_pos;
    token->token_end = end_pos;
    token->token_len = length;
    token->token_ptr = &scanner->data[start_pos];
    token->line_start = *line;
    token->line_end = *line;
    
    add_token(scanner, token);

    return 1;
}

int32_t check_symbol(scanner_main* scanner, int32_t* position, int32_t* line) {
    enum cminus_symbol status = 0, check_next = 0, length = 0, start_pos = *position;
    char* current_char = &scanner->data[*position];
    scanner_token* token = NULL;

    switch(*current_char) {
        case '+':
            status = SYMBOL_PLUS;
            break;
        case '-':
            status = SYMBOL_MINUS;
            break;
        case ';':
            status = SYMBOL_SEMICOLON;
            break;
        case ',':
            status = SYMBOL_COMMA;
            break;
        case '(':
            status = SYMBOL_PARENTHESISOPEN;
            break;
        case ')':
            status = SYMBOL_PARENTHESISCLOSE;
            break;
        case '[':
            status = SYMBOL_BRACKETOPEN;
            break;
        case ']':
            status = SYMBOL_BRACKETCLOSE;
            break;
        case '{':
            status = SYMBOL_BRACESOPEN;
            break;
        case '}':
            status = SYMBOL_BRACESCLOSE;
            break;
        case '*':
            status = SYMBOL_MULTIPLY;
            check_next = 1;            
            break;
        case '/':
            status = SYMBOL_DIVIDE;
            check_next = 1;
            break;
        case '<':
            status = SYMBOL_LESSTHAN;
            check_next = 1;
            break;
        case '>':
            status = SYMBOL_GREATERTHAN;
            check_next = 1;
            break;
        case '=':
            status = SYMBOL_EQUAL;
            check_next = 1;
            break;
        case '|':
            status = SYMBOL_NOTEQUAL;
            check_next = 1;
            break;
        default:
            return 0;
    }
    
    length++;

    if(check_next != 1) {
        goto end; 
    }

    if(*position >= scanner->data_len) {
        if(status == SYMBOL_NOTEQUAL) {
            status = 0;
        }
        goto end;
    }

    current_char = &scanner->data[*position + 1];
    
    switch(*current_char) {
        case '=':
            *position = *position + 1;
            break;
        case '*':
            if(status != SYMBOL_DIVIDE) {
                goto end;
            }
            return handle_comment(scanner, position, line);
        case '/':
            if(status == SYMBOL_MULTIPLY) {
                fprintf(stderr, "error: end of comment with no matching start "
                        "at or near line %d\n", *line);
                return -1;
            }
        default:
            goto end;
    }

    switch(status) {
        case SYMBOL_LESSTHAN:
            status = SYMBOL_LESSTHANEQUAL;
            length++;
            break;
        case SYMBOL_GREATERTHAN:
            status = SYMBOL_GREATERTHANEQUAL;
            length++;
            break;
        case SYMBOL_EQUAL:
            status = SYMBOL_EQUALEQUAL;
            length++;
            break;
        case SYMBOL_NOTEQUAL:
            status = SYMBOL_NOTEQUAL;
            length++;
            break;
        default:
            *position = *position - 1;
            goto end;
    }

end:
    token = malloc(sizeof(scanner_token));
    token->token_type = SCANNER_SYMBOL;
    token->symbol = status;
    token->next_token = NULL;
    token->token_len = length;
    token->token_start = start_pos;
    token->token_end = start_pos + length;
    token->token_ptr = &scanner->data[start_pos];
    token->line_start= *line;
    token->line_end= *line;
    add_token(scanner, token);

    return 1;
}

int32_t process_next(scanner_main* scanner, int32_t* position, int32_t* line, enum scanner_state* state) {
    char* current_char = &scanner->data[*position];
    int32_t status = 0;

    if(handle_common(scanner, position, line)) {
        return 1;
    }

    if(isalpha(*current_char)) {
        return check_id(scanner, position, line);
    }
    
    if(isdigit(*current_char)) {
        return check_number(scanner, position, line);
    }

    status = check_symbol(scanner, position, line);
    
    if(status < 0) {
        return -1;
    } else if(status > 0) {
        return 1;
    }

    handle_unknown(scanner, position, line);

    return 0;
}

int32_t scanner_tokenizer(scanner_main* scanner) {
    int32_t status = 0, position = 0, line = 1;
    enum scanner_state state = NONE;

    while(position < scanner->data_len && status >= 0) {
        status = process_next(scanner, &position, &line, &state);
        position++;
    }

    return status;
}

int main(int argc, char *argv[]) {
    scanner_main scanner;
    scanner_init(&scanner);
    if(!cli_parser(&scanner, argc, argv)) {
        goto end;
    }

    if(!scanner_tokenizer(&scanner)) {
        LogError(__FUNCTION__, __LINE__, "Error Scanning");
        goto end;
    }

    scanner_write_file(&scanner);
    
end:
    scanner_cleanup(&scanner);
    return 0;
}
