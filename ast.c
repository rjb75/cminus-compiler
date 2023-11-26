#include "ast.h"
#include "common.h"

statement_node *createStatement(statement_type type, statement_node *next, statement_node *child, expression_node *expression, declaration_node *declaration, statement_node *else_child, int linenumber)
{
    statement_node *t = malloc(sizeof(statement_node));
    t->statementType = type;
    t->declaration = declaration;
    t->next = next;
    t->child = child;
    t->expression = expression;
    t->else_child = else_child;
    t->linenumber = linenumber;
    return t;
}

expression_node *createExpression(expression_type expressionType, expression_node *left, expression_node *right, char* id, int value, int linenumber)
{
    expression_node *t = malloc(sizeof(expression_node));
    t->expressionType = expressionType;
    t->left = left;
    t->right = right;
    t->id = id;
    t->value = value;
    t->linenumber = linenumber;
    return t;
}

declaration_node *createDeclaration(declaration_type declarationType, char* id, int num, expression_node *expression, declaration_node *next, declaration_node *params, statement_node *child, type_node *type, int linenumber)
{
    declaration_node *t = malloc(sizeof(declaration_node));
    t->child = child;
    t->declarationType = declarationType;
    t->next = next;
    t->num = num;
    t->type = type;
    t->id = id;
    t->params = params;
    t->expression = expression;
    t->linenumber = linenumber;
    return t;
}

type_node *createType(data_type kind, type_node *subtype, int linenumber)
{
    type_node *t = malloc(sizeof(type_node));
    t->kind = kind;
    t->subtype = subtype;
    t->linenumber = linenumber;
    return t;
}

void printDeclaration(declaration_node *declaration, int indent, FILE *out)
{
    if (declaration == NULL)
    {
        return;
    }
    if(declaration->declarationType == VOID_DECL)
    {
        // Nothing is printed for void
        return;
    }
    indent++;
    printIndent(indent, out);
    switch (declaration->declarationType)
    {
    case VAR_DECL:
        fprintf(out, "var_decl @ line %d\n", declaration->linenumber);
        printType(declaration->type, indent, out);
        printIndent(indent+1, out);
        fprintf(out, "new_id [%s] @ line %d\n", declaration->id, declaration->linenumber);
        break;
    case VAR_ARR_DECL:
        fprintf(out, "array_decl @ line %d\n", declaration->linenumber);
        printType(declaration->type, indent, out);
        printIndent(indent+1, out);
        fprintf(out, "new_id [%s] @ line %d\n", declaration->id, declaration->linenumber);
        printIndent(indent+1, out);
        fprintf(out, "int [%d] @ line %d\n", declaration->num, declaration->linenumber); 
        break;
    case FUNC_DECL:
        // fund decl line shows as the last line of the function, so use it's type instead
        fprintf(out, "func @ line %d\n", 
        declaration->type->linenumber);
        printType(declaration->type, indent, out);
        printIndent(indent+1, out);
        fprintf(out, "new_id [%s] @ line %d\n", declaration->id, declaration->type->linenumber);
        indent++;
        printIndent(indent, out);
        fprintf(out, "parameters\n");
        printDeclaration(declaration->params, indent, out);
        printStatement(declaration->child, indent-1, out);
        indent--;
        break;
    case PARAM_DECL:
        fprintf(out, "var_param @ line %d\n", declaration->linenumber);
        printType(declaration->type, indent, out);
        printIndent(indent+1, out);
        fprintf(out, "new_id [%s] @ line %d\n", declaration->id, declaration->linenumber);
        break;
    case PARAM_LIST_DECL:
        fprintf(out, "array_param @ line %d\n", declaration->linenumber);
        printType(declaration->type, indent, out);
        printIndent(indent+1, out);
        fprintf(out, "new_id [%s] @ line %d\n", declaration->id, declaration->linenumber);
        break;
    default:
        break;
    }
    printDeclaration(declaration->next, indent-1, out);
}

void printType(type_node *type, int indent, FILE *out)
{
    if (type == NULL)
    {
        return;
    }
    indent++;
    printIndent(indent, out);
    switch (type->kind)
    {
    case INT_TYPE:
        fprintf(out, "type [int] @ line %d\n", type->linenumber);
        break;
    case VOID_TYPE:
        fprintf(out, "type [void] @ line %d\n", type->linenumber); 
        break;
    default:
        break;
    }
    printType(type->subtype, indent, out);
}

void printExpression(expression_node *expression, int indent, FILE *out)
{
    if (expression == NULL)
    {
        return;
    }
    indent++;
    printIndent(indent, out);
    switch (expression->expressionType)
    {
    case ID_EXPR:
        fprintf(out, "var [%s] @ line %d\n", expression->id, expression->linenumber);
        indent--;
        break;
    case IDAT_EXPR:
        fprintf(out, "array_var [%s] @ line %d\n", expression->id, expression->linenumber);
        break;
    case CALL_EXPR:
        fprintf(out, "funccall @ line %d\n", expression->linenumber);
        indent++;
        printIndent(indent, out);
        fprintf(out, "var [%s] @ line %d\n", expression->id, expression->linenumber);
        printIndent(indent, out);
        fprintf(out, "arguments\n");
        break;
    case NUM_EXPR:
        fprintf(out, "int [%d] @ line %d\n", expression->value, expression->linenumber);
        break;
    case ADD_EXPR:
        fprintf(out, "+ @ line %d\n", expression->linenumber);
        break;
    case SUB_EXPR:
        fprintf(out, "- @ line %d\n", expression->linenumber);
        break;
    case MUL_EXPR:
        fprintf(out, "* @ line %d\n", expression->linenumber);
        break;
    case DIV_EXPR:
        fprintf(out, "/ @ line %d\n", expression->linenumber);
        break;
    case LT_EXPR:
        fprintf(out, "< @ line %d\n", expression->linenumber);
        break;
    case LEQ_EXPR:
        fprintf(out, "<= @ line %d\n", expression->linenumber);
        break;
    case GT_EXPR:
        fprintf(out, "> @ line %d\n", expression->linenumber);
        break;
    case GEQ_EXPR:
        fprintf(out, ">= @ line %d\n", expression->linenumber);
        break;
    case EQ_EXPR:
        fprintf(out, "== @ line %d\n", expression->linenumber);
        break;
    case NEQ_EXPR:
        fprintf(out, "|= @ line %d\n", expression->linenumber);
        break;
    case SET_EXPR:
        fprintf(out, "= @ line %d\n", expression->linenumber);
        break;
    default:    
        break;
    }
    
    printExpression(expression->left, indent, out);
    printExpression(expression->right, indent, out);
}

void printStatement(statement_node *statement, int indent, FILE *out)
{
    if (statement == NULL)
    {
        return;
    }
    indent++;
    printIndent(indent, out);
    switch (statement->statementType)
    {
    case COMPOUND_STMT:
        fprintf(out, "block\n");
        indent++;
        printIndent(indent, out);
        fprintf(out, "declarations\n");
        printDeclaration(statement->declaration, indent, out);
        printIndent(indent, out);
        fprintf(out, "statements\n");
        printStatement(statement->child, indent, out);
        break;
    case EXPRESSION_STMT:
        fprintf(out, "exprstmt @ line %d\n", statement->linenumber);
        printExpression(statement->expression, indent, out);
        indent--;
        break;
    case SELECTION_STMT:
        if(statement->else_child != NULL) {
            fprintf(out, "if_else @ line %d\n", statement->expression->linenumber);
        } else {
            fprintf(out, "if @ line %d\n", statement->expression->linenumber);   
        }
        printExpression(statement->expression, indent, out);
        printStatement(statement->child, indent, out);
        if(statement->else_child != NULL) {
            printStatement(statement->else_child, indent, out);
        } else {
            indent--;
        }
        break;
    case ITERATION_STMT:
        fprintf(out, "while @ line %d\n", statement->expression->linenumber);
        printExpression(statement->expression, indent, out);
        printStatement(statement->child, indent, out);
        indent--;
        break;
    case RETURN_STMT:
        fprintf(out, "return @ line %d\n", statement->linenumber);
        printExpression(statement->expression, indent, out);
        break;
    default:
        break;
    }
    printStatement(statement->next, indent, out);
}

void printIndent(int indent, FILE *out)
{
    for (int i = 0; i < indent; i++)
    {
        fprintf(out, "    ");
    }
}

void printAST(declaration_node *root, FILE *out)
{
    fprintf(out, "program\n");
    printDeclaration(root, 0, out);
}

void cleanupAST(declaration_node *root)
{
    return;
}
