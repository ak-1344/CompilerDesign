/* Lexer for simple language parser (lang1). Build: flex lang1.l && bison -d lang1.y && gcc lang1.tab.c lex.yy.c -o lang1 && ./lang1 */
%{
#include "lang1.tab.h"
%}

%%
a           { return A; }
b           { return B; }
\n          { return '\n'; }
.           { return yytext[0]; }
%%

int yywrap(void) {
    return 1;
}
