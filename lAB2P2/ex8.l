%{
#include <stdio.h>
int max_len = 0;
FILE *yyin;
%}
%option noyywrap
%%
[a-zA-Z]+ {
if (yyleng > max_len) {
max_len = yyleng;
}
}
.|\n ;
%%
int main() {
char filename[100];
printf("Enter the filename to find longest word: ");
scanf("%99s", filename);
yyin = fopen(filename, "r");
if(yyin == NULL) {
perror("Error opening file");
return 1;
}
yylex();
printf("Length of the longest word: %d\n", max_len);
fclose(yyin);
return 0;
}
