#ifndef ANALYZER_H
#define ANALYZER_H

#include "common.h"
#include "ast.h"
#include "parser.h"

typedef struct analyzer_main_s analyzer_main;
typedef struct analyzer_scope_s analyzer_scope;
typedef struct analyzer_symbol_s analyzer_symbol;
typedef struct analyzer_symbol_table_s analyzer_symbol_table;

struct analyzer_main_s {
    analyzer_scope *root_scope;
    declaration_node *root;
    int debug_mode;
    char file_name[128];
    char out_file_name[128];
};

struct analyzer_scope_s {
    int scope_id;
    char *scope_name;
    analyzer_symbol_table *symbol_table;
    analyzer_scope *parent_scope;
};

struct analyzer_symbol_s {
    char *id;
    analyzer_symbol *next;
    declaration_type decl_type;
    data_type data_type;
};

struct analyzer_symbol_table_s {
    analyzer_symbol *head;
    analyzer_symbol *tail;
    int length;
};

int analyzer_init(analyzer_main *analyzer, parser_main *parser);
int check_compound_statement(statement_node *node, analyzer_scope *scope);
int check_statement(statement_node *node, analyzer_scope *scope);
int check_declaration(declaration_node *node, analyzer_scope *scope);
data_type check_function_call(expression_node *node, analyzer_scope *scope);
data_type check_expression(expression_node *node, analyzer_scope *scope);
data_type type_check_expression(expression_node *left, expression_node *right, analyzer_scope *scope);
data_type lookup_symbol(const char *id, analyzer_scope *scope, int recursive, int callable, char **name);
analyzer_scope *create_scope(analyzer_scope *parent, char *name);
analyzer_symbol_table *create_symbol_table();
void print_symbol_table(analyzer_scope *scope);
int add_to_symbol_table(analyzer_scope *scope, declaration_node *node);
int analyze_ast(analyzer_main *analyzer);
int analyzer_write_file(analyzer_main *analyzer);

#endif
