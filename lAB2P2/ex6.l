%{
#include <stdio.h>
#include <string.h>
char word[100];
int count = 0;
FILE *yyin;
%}
%option noyywrap
%%
[a-zA-Z]+ {
if(strcmp(yytext, word) == 0)
count++;
}
.|\n ;
%%
int main() {
char filename[100];
printf("Enter the word to search: ");
scanf("%99s", word);
printf("Enter the filename to search in: ");
scanf("%99s", filename);
yyin = fopen(filename, "r");
if(yyin == NULL) {
perror("Error opening file");
return 1;
}
yylex();
printf("The word '%s' appeared %d times in file '%s'.\n", word, count, filename);
fclose(yyin);
return 0;
}
