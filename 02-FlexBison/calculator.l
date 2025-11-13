/* Lexer for calculator: tokenizes numbers and operators. Build: flex calc.l && bison -d calc.y && gcc calc.tab.c lex.yy.c -o calc && ./calc */
%{
#include "calc.tab.h"
#include <stdlib.h>
%}
%%
[0-9]+             { yylval = atoi(yytext); return NUMBER; }
[ \t]              ; // ignore whitespace
\n                 { return '\n'; }
.                  { return yytext[0]; }
%%
int yywrap(void) { return 1; }
