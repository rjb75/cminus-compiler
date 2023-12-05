#include "analyzer.h"

int analyzer_init(analyzer_main *analyzer)
{
    analyzer->root = NULL;
    analyzer->debug_mode = 0;
    strcpy(analyzer->out_file_name, "analyzer.out");
    analyzer->root_scope = create_scope(NULL, "global");
    return 1;
}

int analyzer_from_parser(analyzer_main *analyzer, parser_main *parser)
{
    analyzer->root = parser->root;
    analyzer->debug_mode = parser->debug_mode;
    strncpy(analyzer->file_name, parser->file_name, 128);
    return 1;
}

int analyzer_cli_parser(scanner_main* scanner, analyzer_main *analyzer, int argc, char *argv[])
{
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
                strcpy(analyzer->out_file_name, filename);
                i++;
            }
        } else if(strcmp(argv[i], "-d") == 0) {
            scanner->debug_mode = 1;
            analyzer->debug_mode = 1;
        }

        // handle filename
        if(i == argc - 1) {
            char* filename = argv[i];
            if(strlen(filename) > 127) {
                LogError(__FUNCTION__, __LINE__, "File name too long");
                goto end;
            }
            strcpy(scanner->file_name, filename);
            strcpy(analyzer->file_name, filename);
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

int check_compound_statement(statement_node *node, analyzer_scope *scope)
{
    declaration_node *current_declaration = node->declaration;
    // printf("\tchecking declarations\n");
    while (current_declaration != NULL)
    {
        if (!check_declaration(current_declaration, scope))
        {
            return 0;
        }
        current_declaration = current_declaration->next;
    }
    expression_node *current_expression = node->expression;
    // printf("\tchecking expressions\n");
    while (current_expression != NULL)
    {
        if (!check_expression(current_expression, scope))
        {
            return 0;
        }
    }
    // printf("\tchecking child\n");
    if (!check_statement(node->child, scope))
    {
        return 0;
    }
    return 1;
}

int check_statement(statement_node *node, analyzer_scope *scope)
{
    if (node == NULL)
    {
        return 1;
    }
    // printf("stmt %d\n", node->statementType);
    analyzer_scope *local_scope = scope;
    char* scope_string = NULL;
    switch (node->statementType)
    {
    case COMPOUND_STMT:
        if (!check_compound_statement(node, scope))
        {
            return 0;
        }
        break;
    case EXPRESSION_STMT:
    case RETURN_STMT:
        if (node->expression != NULL && !check_expression(node->expression, scope))
        {
            return 0;
        }
        break;
    case SELECTION_STMT:
        if (!check_expression(node->expression, scope))
        {
            return 0;
        }
        scope_string = malloc((strlen(scope->scope_name) + strlen("_if") + 1)*sizeof(char));
        strcpy(scope_string, scope->scope_name);
        strcat(scope_string, "_if");
        local_scope = create_scope(local_scope, scope_string);
        if (!check_statement(node->child, local_scope))
        {
            return 0;
        }
        free(scope_string);
        scope_string = NULL;
        if (node->else_child != NULL)
        {
            scope_string = malloc((strlen(scope->scope_name) + strlen("_else") + 1)*sizeof(char));
            strcpy(scope_string, scope->scope_name);
            strcat(scope_string, "_else");
            local_scope = create_scope(local_scope, scope_string);
            if (!check_statement(node->else_child, local_scope))
            {
                return 0;
            }
            free(scope_string);
        scope_string = NULL;
        }
        break;
    case ITERATION_STMT:
        scope_string = malloc((strlen(scope->scope_name) + strlen("_while") + 1)*sizeof(char));
        strcpy(scope_string, scope->scope_name);
        strcat(scope_string, "_while");
        local_scope = create_scope(local_scope, scope_string);
        if (!check_expression(node->expression, local_scope))
        {
            return 0;
        }
        if (!check_statement(node->child, local_scope))
        {
            return 0;
        }
        break;
    default:
        fprintf(stderr, "Error: unknown statement(%d) at or near line %d\n", node->statementType, node->linenumber);
        return 0;
        break;
        break;
    }
    return 1;
}

int check_function_declaration(declaration_node *node, analyzer_scope *scope)
{
    analyzer_scope *local_scope = scope;
    if (strcmp(node->id, "main") == 0)
    {
        if (node->next != NULL)
        {
            fprintf(stderr, "Error: last global declaration is not main function\n");
            return 0;
        }
    }
    // add function to symbol table
    if(!add_to_symbol_table(local_scope, node))
    {
        return 0;
    }
    // create function scope
    local_scope = create_scope(local_scope, node->id);
    // printf("created new scope %d\n", local_scope->scope_id);
    declaration_node *current_param = node->params;
    while (current_param != NULL)
    {
        if (!check_declaration(current_param, local_scope))
        {
            return 0;
        }
        current_param = current_param->next;
    }

    if (!check_statement(node->child, local_scope))
    {
        return 0;
    }
    return 1;
}

int check_declaration(declaration_node *node, analyzer_scope *scope)
{
    // printf("decl %d\n", node->declarationType);

    switch (node->declarationType)
    {
    case FUNC_DECL:
        if (!check_function_declaration(node, scope))
        {
            return 0;
        }
        break;
    case VAR_DECL:
    case VAR_ARR_DECL:
    case PARAM_DECL:
    case PARAM_LIST_DECL:
        // add the symbol to current scope symbol table
        // print_symbol_table(scope);
        if(!add_to_symbol_table(scope, node))
        {
            return 0;
        }

        break;
    case VOID_DECL:
        break;
    }

    return 1;
}

data_type check_function_call(expression_node *node, analyzer_scope *scope)
{
    char *expr_name = NULL;
    // check if ID is in symbol table
    data_type fun_type = lookup_symbol(node->id, scope, 1, 1, &expr_name);
    if (fun_type == ERROR_TYPE)
    {
        // print_symbol_table(scope);
        fprintf(stderr, "Error: unknown name \"%s\" at or near line %d\n", node->id, node->linenumber);
        return ERROR_TYPE;
    }
    
    free(node->id);
    node->id = expr_name;

    expression_node *current_arg = node->left;

    while (current_arg != NULL)
    {
        if (!check_expression(current_arg, scope))
        {
            return ERROR_TYPE;
        }
        current_arg = current_arg->next_arg;
    }

    return fun_type;
}

data_type type_check_expression(expression_node *left, expression_node *right, analyzer_scope *scope)
{
    data_type left_type, right_type;
    left_type = check_expression(left, scope);

    if(left_type == ERROR_TYPE)
    {
        return ERROR_TYPE;
    }

    right_type = check_expression(right, scope);

    if(left_type == ERROR_TYPE)
    {
        return ERROR_TYPE;
    }

    if(left_type != right_type)
    {
        return ERROR_TYPE;
    }

    return left_type;
}

data_type check_expression(expression_node *node, analyzer_scope *scope)
{
    // printf("expr %d\n", node->expressionType);
    char *expr_name = NULL;
    data_type expr_type = ERROR_TYPE;
    switch (node->expressionType)
    {
    case CALL_EXPR:
        return check_function_call(node, scope);
    case ID_EXPR:
        // check if ID is in symbol table
        expr_type = lookup_symbol(node->id, scope, 1, 0, &expr_name);
        if (expr_type == ERROR_TYPE)
        {
            // print_symbol_table(scope);
            fprintf(stderr, "Error: unknown name \"%s\" at or near line %d\n", node->id, node->linenumber);
            return 0;
        }
        free(node->id);
        node->id = expr_name;
        break;
    case IDAT_EXPR:
        if (lookup_symbol(node->id, scope, 1, 0, &expr_name) == INT_ARRAY_TYPE)
        {
            free(node->id);
            node->id = expr_name;
            return INT_TYPE;
        }
        fprintf(stderr, "Error: using variable \"%s\" as array at or near line %d\n", node->id, node->linenumber);
        return ERROR_TYPE;
    case NUM_EXPR:
        // noop
        return INT_TYPE;
    case ADD_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '+' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case SUB_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '-' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case MUL_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '*' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case DIV_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '/' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case LT_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '<' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case LEQ_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '<=>' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case GT_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '>' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case GEQ_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '>=' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case EQ_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '==' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case NEQ_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '|=' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    case SET_EXPR:
        expr_type = type_check_expression(node->left, node->right, scope);
        if(expr_type == ERROR_TYPE)
        {
            fprintf(stderr, "Error: Operand mismatch for '=' at or near line %d\n", node->linenumber);
            return ERROR_TYPE;
        }
        break;
    default:
        fprintf(stderr, "Error: unknown expression(%d) at or near line %d\n", node->expressionType, node->linenumber);
        return ERROR_TYPE;
        break;
    }
    return expr_type;
}

data_type lookup_symbol(const char *id, analyzer_scope *scope, int recursive, int callable, char** name)
{
    // printf("lookup %s\n", id);
    analyzer_symbol *symbol = scope->symbol_table->head;
    data_type type = ERROR_TYPE;
    ssize_t str_len = snprintf(NULL, 0, "%s_%s", scope->scope_name, id);
    *name = malloc(str_len + 1);
    snprintf(*name, str_len + 1, "%s_%s", scope->scope_name, id);
    while (symbol != NULL)
    {
        if (strcmp(id, symbol->id) == 0)
        {
            if((callable && symbol->decl_type == FUNC_DECL) || !callable)
            {
                return symbol->data_type;
            }
        }
        symbol = symbol->next;
    }
    if (recursive && scope->parent_scope != NULL)
    {
        return lookup_symbol(id, scope->parent_scope, 1, callable, name);
    }
    return type;
}

analyzer_scope *create_scope(analyzer_scope *parent, char* name)
{
    analyzer_scope *new_scope = malloc(sizeof(analyzer_scope));
    if (parent == NULL)
    {
        new_scope->scope_id = 1;
    }
    else
    {
        new_scope->scope_id = parent->scope_id + 1;
    }
    new_scope->parent_scope = parent;
    new_scope->scope_name = malloc(strlen(name) + 1);
    strcpy(new_scope->scope_name, name);
    new_scope->symbol_table = create_symbol_table();
    return new_scope;
}

analyzer_symbol_table *create_symbol_table()
{
    analyzer_symbol_table *new_symbol_table = malloc(sizeof(analyzer_symbol_table));
    new_symbol_table->length = 0;
    return new_symbol_table;
}

void print_symbol_table(analyzer_scope *scope)
{
    printf("symbol table %d:\n", scope->scope_id);
    analyzer_symbol *current_symbol = scope->symbol_table->head;
    while(current_symbol != NULL)
    {
        printf("\tsymbol: %s[%d]\n", current_symbol->id, current_symbol->data_type);
        current_symbol = current_symbol->next;
    }
}

int add_to_symbol_table(analyzer_scope *scope, declaration_node *node)
{
    analyzer_symbol_table *table = scope->symbol_table;
    analyzer_symbol *new_symbol = malloc(sizeof(analyzer_symbol));
    char* symbol_name = NULL;
    new_symbol->decl_type = node->declarationType;
    switch (node->declarationType)
    {
    case VAR_DECL:
    case PARAM_DECL:
    case VAR_ARR_DECL:
    case PARAM_LIST_DECL:
        if (lookup_symbol(node->id, scope, 0, 0, &symbol_name) != ERROR_TYPE)
        {
            fprintf(stderr, "Error: \"%s\" redefined at or near line %d\n", node->id, node->linenumber);
            free(new_symbol);
            return 0;
        }
        new_symbol->data_type = node->type->kind;
        new_symbol->id = node->id;
        node->id = symbol_name;
        if (node->declarationType == VAR_ARR_DECL || node->declarationType == PARAM_LIST_DECL)
        {
            // assumption based on language, all arrays are int
            new_symbol->data_type = INT_ARRAY_TYPE;
        }
        break;
    case FUNC_DECL:
        if (lookup_symbol(node->id, scope, 1, 0, &symbol_name) != ERROR_TYPE)
        {
            fprintf(stderr, "Error: \"%s\" redefined at or near line %d\n", node->id, node->linenumber);
            free(new_symbol);
            return 0;
        }
        new_symbol->data_type = node->type->kind;
        new_symbol->id = node->id;
        node->id = symbol_name;
        break;
    case VOID_DECL:
        // noop as nothing is declared
        free(new_symbol);
        return 1;
    default:
        fprintf(stderr, "Error: unknown declaration(%d) at or near line %d\n", node->declarationType, node->linenumber);
        free(new_symbol);
        return 0;
    }

    // printf("adding symbol to table %s\n", node->id);

    analyzer_symbol *tail = table->tail;
    if (table->length == 0 && table->tail == NULL)
    {
        table->head = new_symbol;
    }
    else
    {
        tail->next = new_symbol;
    }
    table->tail = new_symbol;
    table->length = table->length + 1;

    return 1;
}

int analyze_ast(analyzer_main *analyzer)
{
    declaration_node *current = analyzer->root;
    while (current != NULL)
    {
        if (!check_declaration(current, analyzer->root_scope))
        {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

int analyzer_write_file(analyzer_main *analyzer)
{
    FILE *file = NULL;
    if(!open_file(analyzer->out_file_name, &file))
    {
        return -1;
    }

    if(analyzer->root != NULL)
    {
        printAST(analyzer->root, file);
    }

    if(!close_file(&file))
    {
        return -1;
    }

    return 1;
}

#ifdef MODE_ANALYZER

int main(int argc, char *argv[])
{
    analyzer_main analyzer;
    scanner_main scanner;
    scanner_init(&scanner);
    analyzer_init(&analyzer);
    if (!analyzer_cli_parser(&scanner, &analyzer, argc, argv))
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

    analyzer_from_parser(&analyzer, &parser);

    if (!analyze_ast(&analyzer))
    {
        LogError(__FUNCTION__, __LINE__, "Error Analyzing");
        goto end;
    }

    analyzer_write_file(&analyzer);

end:
    parser_cleanup(&parser);
    scanner_cleanup(&scanner);
    return 0;
}

#endif
