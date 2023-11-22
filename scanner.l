%{
    #include <math.h>
    #include "parser2.h"
    char tokenString[MAXTOKENLEN+1];
%}

DIGIT       [0-9]
LETTER      [a-z|A-Z]
NUMBER      {DIGIT}+
ID          {LETTER}+
NEWLINE     \n
WHITESPACE  [ \t]+

%%
"-"     {return MINUS;}
"+"     {return PLUS;}
"*"     {return MULTIPLY;}
"/"     {return DIVIDE;}
"<"     {return LESSTHAN;}
"<="    {return LESSTHANEQUAL;}
">"     {return GREATERTHAN;}
">="    {return GREATERTHANEQUAL;}
"=="    {return EQUALEQUAL;}
"|="    {return NOTEQUAL;}
"="     {return EQUAL;}
";"     {return SEMICOLON;}
","     {return COMMA;}
")"     {return C_PAREN;}
"("     {return O_PAREN;}
"["     {return O_BRACK;}    
"]"     {return C_BRACK;}
"{"     {return O_BRACE;}
"}"     {return C_BRACE;}

"else"      {return ELSE;}
"if"        {return IF;}
"int"       {return INT;}
"return"    {return RETURN;}
"void"      {return VOID;}
"while"     {return WHILE;}

NUMBER      {printf("got num %d\n", atoi(yytext)); yylval.num = atoi(yytext); return NUM;}
ID          {printf("got id %s\n", strdup(yytext)); yylval.id = strdup(yytext); return ID;}
NEWLINE     {lineno++;}
WHITESPACE     /* skip whitespace */

"/*"        {
    char c;
    while((c = input()) != 0)
    {
        putchar(c);
        if(c == '*')
        {
            while((c = input()) == '*')
            {
                putchar(c);
            }
            if (c == 0) break;
            putchar(c);
            if (c == '/') return;
        }
        else if(c == '\n')
        {
            lineno++;
        }
    }
}

.   {printf("Bad inputL %s\n", yytext); return ERROR;}

%%

int yywrap() {
    return 1;
}

