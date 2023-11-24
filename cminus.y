%require "3.2"
%define api.pure full

%code requires{
    #include "parser.h"
}

%{
    // C code
    #include<stdio.h>
    #include"ast.h"
    #include"bison.h"
    int yyerror(parser_main *parser, const char *msg);
    
    declaration_node * savedTree = 0; /* stores syntax tree for later return */
%}

%param   {parser_main *parser}
%define parse.error verbose
%token PLUS MINUS MULTIPLY DIVIDE LESSTHAN LESSTHANEQUAL GREATERTHAN GREATERTHANEQUAL EQUALEQUAL NOTEQUAL EQUAL SEMICOLON COMMA O_PAREN C_PAREN O_BRACK C_BRACK O_BRACE C_BRACE ID NUM ELSE IF INT RETURN VOID WHILE ERROR

%union {
    declaration_node *declaration;
    expression_node *expression;
    type_node *type;
    statement_node *statement;
    char *id;
    int num;
}

%type<declaration> program declaration_list declaration var_declaration fun_declaration param param_list params local_declarations

%type<id> ID

%type<num> NUM

%type<type> type_specifier

%type<expression> expression arg_list args term factor call var simple_expression additive_expression

%type<statement> statement statement_list expression_stmt selection_stmt iteration_stmt return_stmt compound_stmt

%precedence "then"
%precedence ELSE

%%

program: declaration_list    { savedTree = $1;}

declaration_list: declaration declaration_list {
        $$ = $1;
        $1->next = $2;
    }
    | declaration   {$$ = $1;}

declaration: var_declaration    {$$ = $1;}
    | fun_declaration   {$$ = $1;}

var_declaration: type_specifier ID SEMICOLON    {
        $$ = createDeclaration(VAR_DECL, $2, 0, 0, 0, 0, 0, $1, parser->line_no);
    }
    | type_specifier ID O_BRACK NUM C_BRACK SEMICOLON   {
        $$ = createDeclaration(VAR_ARR_DECL, $2, $4, 0, 0, 0, 0, $1, parser->line_no);
    }

type_specifier: INT   {
        $$ = createType(INT_TYPE, 0, parser->line_no);
    }
    | VOID  {
        $$ = createType(VOID_TYPE, 0, parser->line_no); 
    }

fun_declaration: type_specifier ID O_PAREN params C_PAREN compound_stmt {
        $$ = createDeclaration(FUNC_DECL, $2, 0, 0, 0, $4, $6, $1, parser->line_no); 
    }

params: param_list  {$$ = $1;}
    | VOID  {
        $$ = createDeclaration(VOID_DECL, 0, 0, 0, 0, 0, 0, 0, parser->line_no);
    }

param_list: param COMMA param_list {
        $$ = $1;
        $1->next = $3;
    }
    | param {$$ = $1;}

param: type_specifier ID    {
        $$ = createDeclaration(PARAM_DECL, $2, 0, 0, 0, 0, 0, $1, parser->line_no);  
    }
    | type_specifier ID O_BRACK C_BRACK {
        $$ = createDeclaration(PARAM_LIST_DECL, $2, 0, 0, 0, 0, 0, $1, parser->line_no);
    }

compound_stmt: O_BRACE local_declarations statement_list C_BRACE    {
        $$ = createStatement(COMPOUND_STMT, 0, $3, 0, $2, 0, parser->line_no);
    }

local_declarations: var_declaration local_declarations {
        $$ = $1;
        $1->next = $2; 
    }
    |   {$$=NULL;}

statement_list: statement statement_list    {
       $$ = $1;
       $1->next = $2;
    }
    |   {$$=NULL;}

statement: expression_stmt  {$$ = $1;}
    | compound_stmt         {$$ = $1;}
    | selection_stmt        {$$ = $1;}
    | iteration_stmt        {$$ = $1;}
    | return_stmt           {$$ = $1;}

expression_stmt: expression SEMICOLON {
        $$ = createStatement(EXPRESSION_STMT, 0, 0, $1, 0, 0, parser->line_no);
    }
    | SEMICOLON {$$ = NULL;}

selection_stmt: IF O_PAREN expression C_PAREN statement %prec "then" {
        $$ = createStatement(SELECTION_STMT, 0, $5, $3, 0, 0, parser->line_no);
    }
    | IF O_PAREN expression C_PAREN statement ELSE statement    {
        $$ = createStatement(SELECTION_STMT, 0, $5, $3, 0, $7, parser->line_no);
    } 

iteration_stmt: WHILE O_PAREN expression C_PAREN statement  {
        $$ = createStatement(ITERATION_STMT, 0, $5, $3, 0, 0, parser->line_no);
    }

return_stmt: RETURN SEMICOLON   {
        $$ = createStatement(RETURN_STMT, 0, 0, 0, 0, 0, parser->line_no); 
    }
    | RETURN expression SEMICOLON   {
        $$ = createStatement(RETURN_STMT, 0, 0, $2, 0, 0, parser->line_no); 
    }

expression: var EQUAL expression    {
        $$ = createExpression(SET_EXPR, $1, $3, 0, 0, parser->line_no);
    }
    | simple_expression {$$ = $1;}

var: ID {
        $$ = createExpression(ID_EXPR, 0, 0, $1, 0, parser->line_no); 
    }
    | ID O_BRACK expression C_BRACK {
        $$ = createExpression(IDAT_EXPR, $3, 0, $1, 0, parser->line_no);
    }

simple_expression: additive_expression LESSTHAN additive_expression {
        $$ = createExpression(LT_EXPR, $1, $3, 0, 0, parser->line_no);  
    }
    | additive_expression LESSTHANEQUAL additive_expression {
        $$ = createExpression(LEQ_EXPR, $1, $3, 0, 0, parser->line_no);  
    }
    | additive_expression GREATERTHAN additive_expression {
        $$ = createExpression(GT_EXPR, $1, $3, 0, 0, parser->line_no);  
    }
    | additive_expression GREATERTHANEQUAL additive_expression {
        $$ = createExpression(GEQ_EXPR, $1, $3, 0, 0, parser->line_no);  
    }
    | additive_expression EQUALEQUAL additive_expression {
        $$ = createExpression(EQ_EXPR, $1, $3, 0, 0, parser->line_no);  
    }
    | additive_expression NOTEQUAL additive_expression {
        $$ = createExpression(NEQ_EXPR, $1, $3, 0, 0, parser->line_no);  
    }
    | additive_expression   {$$ = $1;}

additive_expression: additive_expression PLUS term  {
        $$ = createExpression(ADD_EXPR, $1, $3, 0, 0, parser->line_no); 
    }
    | additive_expression MINUS term  {
        $$ = createExpression(SUB_EXPR, $1, $3, 0, 0, parser->line_no); 
    }
    | term  {$$ = $1;}

term: term MULTIPLY factor   {
        $$ = createExpression(MUL_EXPR, $1, $3, 0, 0, parser->line_no); 
    }
    | term DIVIDE factor   {
        $$ = createExpression(DIV_EXPR, $1, $3, 0, 0, parser->line_no); 
    }
    | factor    {$$ = $1;}

factor: O_PAREN expression C_PAREN {$$ = $2;}
    | var   {$$ = $1;}
    | call  {$$ = $1;}
    | NUM   {
        $$ = createExpression(NUM_EXPR, 0, 0, 0, $1, parser->line_no); 
    }

call: ID O_PAREN args C_PAREN  {
        $$ = createExpression(CALL_EXPR, $3, 0, $1, 0, parser->line_no);
    }

args: arg_list  {$$ = $1;}
    |   {$$ = NULL;}

arg_list: expression COMMA arg_list {$$ = $1; $1->left=$3;} | expression {$$ = $1;}

    
%%

// C code 

int yyerror(parser_main *parser, const char * message)
{ fprintf(stderr,"Syntax error at line %d: %s\n",parser->line_no,message);
  return 0;
}

declaration_node * parse2(parser_main *parser)
{ 
    if(yyparse(parser))
    {
        cleanupAST(savedTree);
        return NULL;
    }
  return savedTree;
}
