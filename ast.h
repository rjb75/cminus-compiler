#ifndef AST_H
#define AST_H

#include "common.h"

typedef enum
{
    ID_EXPR,
    IDAT_EXPR,
    CALL_EXPR,
    NUM_EXPR,
    ADD_EXPR,
    SUB_EXPR,
    MUL_EXPR,
    DIV_EXPR,
    LT_EXPR, 
    LEQ_EXPR,
    GT_EXPR,
    GEQ_EXPR,
    EQ_EXPR,
    NEQ_EXPR,
    SET_EXPR,
} expression_type;

typedef enum
{
    INT_TYPE,
    VOID_TYPE,
    INT_ARRAY_TYPE,
} data_type;

typedef enum
{
    EXPRESSION_STMT,
    COMPOUND_STMT,
    SELECTION_STMT,
    ITERATION_STMT,
    RETURN_STMT,
} statement_type;

typedef enum
{
    VAR_DECL,
    VAR_ARR_DECL,
    FUNC_DECL,
    PARAM_DECL,
    PARAM_LIST_DECL,
    VOID_DECL,
} declaration_type;

typedef enum
{
    DECLARATION_NODE,
    STATEMENT_NODE,
    EXPRESSION_NODE,
} node_type;

typedef struct type_node_s {
    data_type kind;
    struct type_node_s *subtype;
    int linenumber;
} type_node;

typedef struct declaration_node_s {
    char* id;
    struct expression_node_s *expression;
    struct declaration_node_s *next;
    struct declaration_node_s *params;
    struct statement_node_s *child;
    declaration_type declarationType;
    struct type_node_s *type;
    int num;
    int linenumber;
} declaration_node;

typedef struct statement_node_s {
    statement_type statementType;
    struct statement_node_s *next;
    struct statement_node_s *child;
    struct statement_node_s *else_child;
    struct expression_node_s *expression;
    struct declaration_node_s *declaration;
    int linenumber;
} statement_node;

typedef struct expression_node_s {
    expression_type expressionType;
    struct expression_node_s *left;
    struct expression_node_s *right;
    char* id;
    int value;
    int linenumber;
} expression_node;

statement_node *createStatement(statement_type type, statement_node *next, statement_node *child, expression_node *expression, declaration_node *declaration, statement_node *else_child, int linenumber);
expression_node *createExpression(expression_type type, expression_node *left, expression_node *right, char* id, int value, int linenumber);
type_node *createType(data_type type, type_node *subtype, int linenumber);
declaration_node *createDeclaration(declaration_type declarationType, char* id, int num, expression_node *expression, declaration_node *next, declaration_node *params, statement_node *child, type_node *type, int linenumber);

void printStatement(statement_node *statement, int indent, FILE *out);
void printExpression(expression_node *expression, int indent, FILE *out);
void printDeclaration(declaration_node *declaration, int indent, FILE *out);
void printType(type_node *type, int indent, FILE *out);
void printIndent(int indent, FILE *out);
void printAST(declaration_node *root, FILE *out);
void cleanupAST(declaration_node *root);

#endif
