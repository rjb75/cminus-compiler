#ifndef BISON_H
#define BISON_H

#include "parser.h"
#include "pr.h"
#include "scanner.h"
#include "string.h"

int yylex(YYSTYPE *lvalp, parser_main *parser);

int32_t parse(parser_main *parser);

#endif