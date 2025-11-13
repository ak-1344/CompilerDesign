/* Flex lexical analyzer program. Build: flex ex7.l && gcc lex.yy.c -o P2ex7 && ./P2ex7 */
%{
#include <stdio.h>
#include <string.h>
char oldword[100];
char newword[100];
FILE *yyin;
%}
%option noyywrap
%%
[a-zA-Z]+ {
if(strcmp(yytext, oldword) == 0)
printf("%s", newword);
else
printf("%s", yytext);
}
.|\n {
printf("%s", yytext);
}
%%
int main() {
char filename[100];
printf("Enter the word to replace: ");
scanf("%99s", oldword);
printf("Enter the replacement word: ");
scanf("%99s", newword);
printf("Enter the filename: ");
scanf("%99s", filename);
yyin = fopen(filename, "r");
if(yyin == NULL) {
perror("Error opening file");
return 1;
}
yylex();
fclose(yyin);
return 0;
}
