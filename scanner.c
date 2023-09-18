#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "scanner.h"
#include "logger.h"
#include "file.h"

int cli_parser(scanner_main* scanner, int argc, char *argv[]) {
    if(argc < 1) {
        return -1;
    } else if(argc == 1) {
        LogError(__FUNCTION__, __LINE__, "Error parsing arguments");
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        if(i%2) {
            if(strcmp(argv[i], "-f") == 0) {
                if(i < argc) {
                    char* filename = argv[i+1];
                    if(strlen(filename) > 127) {
                        LogError(__FUNCTION__, __LINE__, "File name too long");
                        goto end;
                    }
                    strcpy(scanner->file_name, filename);
                }
            }
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
    int32_t status = -1;

    scanner->data = NULL;
    scanner->tokens = NULL;
    scanner->token_len = 0;

    status = 1;

    return status;
}

int32_t scanner_cleanup(scanner_main* scanner) {
    int32_t status = -1;

    if(scanner->data != NULL) {
        LogDebug(__FUNCTION__, __LINE__, "Freeing scanner data");
        free(scanner->data);
    }
    
    if(scanner->tokens != NULL) {
        LogDebug(__FUNCTION__, __LINE__, "Freeing token data");
        scanner_token** token_ptr = &scanner->tokens;
        while(*token_ptr != NULL) {
            printf("got token> %d %d\n", (*token_ptr)->token_start, (*token_ptr)->token_end);
            scanner_token* temp = (*token_ptr)->next_token;
            free(*token_ptr);
            *token_ptr = temp;
        }
    }

    status = 1;

    return status;
}

/*
 * This function checks for the start of a comment
 * Return Values:
 *  0: not a comment 1: opening of a comment 2: closing of a comment
 */
int32_t check_comment(scanner_main* scanner, int32_t* position, enum scanner_state* state) {
    int32_t status = 0;
    char* current_char = &scanner->data[*position];
    
    if(*position >= scanner->data_len) {
        return status;
    }

    switch (*current_char) {
        case '/':
            status = 1;
            break;
        case '*':
            status = 2;
            break;
        default:
            return 0;
    }

    current_char = &scanner->data[*position + 1];
    
    if(*state == INCOMMENT && *current_char == '/' && status == 2) {
        LogDebug(__FUNCTION__, __LINE__, "Comment done");
        *state = NONE;
    } else if(*current_char == '*' && status == 1) {
        LogDebug(__FUNCTION__, __LINE__, "Comment open");
        *state = INCOMMENT;
    } else {
        return 0;
    }

    *position = *position + 1;

    return status;
}

int32_t add_token(scanner_main* scanner, scanner_token* token) {

    scanner_token** token_ptr = &scanner->tokens;
    
    if(*token_ptr == NULL) {
        *token_ptr = token;
        printf("added %p %p\n", scanner->tokens, token);
        scanner->token_len++;
        return 0;
    }
    
    while((*token_ptr)->next_token != NULL) {
        LogDebug(__FUNCTION__, __LINE__, "Another token");
        printf("token is %p\n", (*token_ptr)->next_token);
        token_ptr = &(*token_ptr)->next_token;
    }
    
    LogDebug(__FUNCTION__, __LINE__, "Added token");
    printf("token was %p\n", token);
   
    scanner_token* to_add = *token_ptr;
    printf("to add is %p %p\n", to_add, to_add->next_token);
    to_add->next_token = token;
    //scanner->tokens++;

    return 0;
}

int32_t check_id(scanner_main* scanner, int32_t* position) {
    int32_t status = -1, start_pos = *position, length = 1, end_pos = *position;
    char* current_char = &scanner->data[*position];
    start_pos = *position;
    printf("got id (%d): %c", *position, *current_char);
    *position = *position + 1;

    while(*position < scanner->data_len) {
        current_char = &scanner->data[*position];
        if(!isalpha(*current_char)) {
            // id end
            *position = *position - 1;;
            end_pos = *position;
            break;
        }
        printf("%c", *current_char);
        *position = *position + 1;;
        length++;
    }
    
    scanner_token* token = malloc(sizeof(scanner_token));
    token->next_token = NULL;
    token->token_start = start_pos;
    token->token_end = end_pos;
    token->token_len = length;
    token->token_ptr = &scanner->data[start_pos];

    add_token(scanner, token);

    printf("\n");
    return 1;
}

int32_t check_number(scanner_main* scanner, int32_t* position) {
    int32_t status = -1, start_pos = *position, length = 1, end_pos;
    char* current_char = &scanner->data[*position];
    start_pos = *position;
    printf("got number (%d): %c", *position, *current_char);
    *position = *position + 1;

    while(*position < scanner->data_len) {
        current_char = &scanner->data[*position];
        if(!isdigit(*current_char)) {
            *position = *position - 1;
            end_pos = *position;
            break;
        }
        printf("%c", *current_char);
        *position = *position + 1;
        length++;
    }
    printf("\n");

    return status;
}

int32_t check_symbol(scanner_main* scanner, int32_t* position) {
    int32_t status = -1;
    char* current_char = &scanner->data[*position];

    switch(*current_char) {
        case '+':
            return SYMBOL_PLUS;
        case '-':
            return SYMBOL_MINUS;
        case '*':
            return SYMBOL_MULTIPLY;
        case '/':
            return SYMBOL_DIVIDE;
        case ';':
            return SYMBOL_SEMICOLON;
        case ',':
            return SYMBOL_COMMA;
        case '(':
            return SYMBOL_PARENTHESISOPEN;
        case ')':
            return SYMBOL_PARENTHESISCLOSE;
        case '[':
            return SYMBOL_BRACKETOPEN;
        case ']':
            return SYMBOL_BRACKETCLOSE;
        case '{':
            return SYMBOL_BRACESOPEN;
        case '}':
            return SYMBOL_BRACESCLOSE;
        case '<':
            status = SYMBOL_LESSTHAN;
            break;
        case '>':
            status = SYMBOL_GREATERTHAN;
            break;
        case '=':
            status = SYMBOL_EQUAL;
            break;
        case '!':
            status = SYMBOL_NOTEQUAL;
            break;
        default:
            return -1;
    }
    
    if(*position >= scanner->data_len) {
        if(status == SYMBOL_NOTEQUAL) {
            status = -1;
        }
        return status;
    }

    current_char = &scanner->data[*position + 1];
    
    if(*current_char != '=') {
        return status;
    }

    *position = *position + 1;;

    switch(status) {
        case SYMBOL_LESSTHAN:
            return SYMBOL_LESSTHANEQUAL;
        case SYMBOL_GREATERTHAN:
            return SYMBOL_GREATERTHANEQUAL;
        case SYMBOL_EQUAL:
            return SYMBOL_EQUALEQUAL;
        case SYMBOL_NOTEQUAL:
            return SYMBOL_NOTEQUAL;
        default:
            *position = *position - 1;
            return status;
    }
}

int32_t process_next(scanner_main* scanner, int32_t* position, enum scanner_state* state) {
    char* current_char = &scanner->data[*position];
    
    if(*current_char == '/' || *current_char == '*') {
        if(check_comment(scanner, position, state) == 1) {
            goto end;
        }
    }

    if(isalpha(*current_char)) {
        check_id(scanner, position);
        goto end;   
    }

    if(isnumber(*current_char)) {
        check_number(scanner, position);
        goto end;
    }
    
    if(check_symbol(scanner, position)) {
        goto end;
    }

end:
    return 1;
}

int32_t scanner_tokenizer(scanner_main* scanner) {
    int32_t status = -1, position = 0;
    enum scanner_state state = NONE;
    
    while(position < scanner->data_len) {
        char* current_char = &scanner->data[position];
        switch(state) {
            case INCOMMENT:
                check_comment(scanner, &position, &state);
                break;
            default:
                process_next(scanner, &position, &state);
        }
        
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
        goto end;
    }
    
end:
    scanner_cleanup(&scanner);
    return 0;
}

