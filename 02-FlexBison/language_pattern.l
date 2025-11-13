/* Lexer for language parser (lang2). Build: flex lang2.l && bison -d lang2.y && gcc lang2.tab.c lex.yy.c -o lang2 && ./lang2 */
%{
#include "lang2.tab.h"
%}

%%
a           { return A; }
b           { return B; }
\n          { return '\n'; }
.           { return yytext[0]; }
%%

int yywrap(void) { return 1; }
