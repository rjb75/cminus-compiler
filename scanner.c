#include <_ctype.h>
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

    status = 1;

    return status;
}

int32_t scanner_cleanup(scanner_main* scanner) {
    int32_t status = -1;

    if(scanner->data != NULL) {
        LogDebug(__FUNCTION__, __LINE__, "Freeing scanner data");
        free(scanner->data);
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

    *position++;

    return status;
}

int32_t check_id() {
    int32_t status = -1;
    
    // check if keyword  

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

    *position++;

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
        goto end;   
    }

    if(isnumber(*current_char)) {
        goto end;
    }
    
    if(check_symbol(scanner, position)) {
        LogDebug(__FUNCTION__, __LINE__, "got symbol");
        printf("%c", *current_char);
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

