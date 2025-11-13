/* Flex text processing program. Build: flex ex9.l && gcc lex.yy.c -o ex9 && ./ex9 */
%{
#include <stdio.h>
#include <stdlib.h>
FILE *yyin;
%}
%option noyywrap
digit [0-9]
letter [a-zA-Z]
id {letter}({letter}|{digit})*
number {digit}+
%%
"if"         { printf("KEYWORD\t%s\n", yytext); }
"else"       { printf("KEYWORD\t%s\n", yytext); }
"while"      { printf("KEYWORD\t%s\n", yytext); }
"return"     { printf("KEYWORD\t%s\n", yytext); }

{id}         { printf("IDENTIFIER\t%s\n", yytext); }
{number}     { printf("NUMBER\t%s\n", yytext); }

"+"|"-"|"*"|"/"|"="              { printf("OPERATOR\t%s\n", yytext); }
";"|","|"("|")"|"{"|"}"          { printf("PUNCTUATION\t%s\n", yytext); }

[ \t\n]+     ;  /* ignore whitespace */

.            { printf("UNKNOWN\t%s\n", yytext); }
%%
int main() {
char filename[256];
printf("Enter the input filename: ");
if (scanf("%255s", filename) != 1) {
fprintf(stderr, "Failed to read filename\n");
return 1;
}
yyin = fopen(filename, "r");
if (!yyin) {
perror("Error opening file");
return 1;
}
yylex();
fclose(yyin);
return 0;
}
