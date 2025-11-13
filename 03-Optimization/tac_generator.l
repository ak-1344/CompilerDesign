%option noyywrap

/* Lexer for ex15: intermediate code generation. Build: flex ex15.l && bison -d ex15.y && gcc ex15.tab.c lex.yy.c -o ex15 && ./ex15 */
%{
#include "ex15.tab.h"
#include <stdlib.h>
#include <string.h>
%}

digit   [0-9]
id      [A-Za-z_][A-Za-z0-9_]*
ws      [ \t\r\n]+

%%
{ws}                {}
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
{digit}+            { yylval.intval = atoi(yytext); return NUMBER; }
{id}                { yylval.id = strdup(yytext); return ID; }
"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"<"                 { return '<'; }
">"                 { return '>'; }
"="                 { return '='; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
";"                 { return ';'; }
.                   { return yytext[0]; }
%%
