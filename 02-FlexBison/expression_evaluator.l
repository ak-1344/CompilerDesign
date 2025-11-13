/* Lexer for infix expression evaluator. Build: flex infix.l && bison -d infix.y && gcc infix.tab.c lex.yy.c -o infix && ./infix */
%{
#include "infix.tab.h"
%}

%%
[a-zA-Z]      { yylval.c = yytext[0]; return ID; }
[ \t]         ; /* ignore whitespace */
\n            { return '\n'; }
.             { return yytext[0]; }
%%

int yywrap(void) {
    return 1;
}
