/* Flex lexical analyzer: tokenizes C-like code. Build: flex ii.l && gcc lex.yy.c -o ii && ./ii */
%{
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap
identifier [a-zA-Z_][a-zA-Z0-9_]*
number [0-9]+
string \"([^\"\n]*)\"

%%
"#".* { printf("\n%s is a Preprocessor Directive", yytext); }

"int"|"float"|"char"|"if"|"else"|"for"|"while"|"do"|"return"|"void"|"main" { printf("\n%s is a Keyword", yytext); }

{identifier}\( { printf("\nFunction: %s", yytext); }
"{" { printf("\nBlock Begins"); }
"}" { printf("\nBlock Ends"); }

{identifier}(\[[0-9]*\])? { printf("\n%s is an Identifier", yytext); }

{string} { printf("\n%s is a String", yytext); }

{number} { printf("\n%s is a Number", yytext); }

"=" { printf("\n%s is an Assignment Operator", yytext); }
"<="|">="|"=="|"!="|"<"|">" { printf("\n%s is a Relational Operator", yytext); }

";" { printf("\nSemicolon"); }

[ \t\n]+ ;
. ;
%%

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            printf("\nCould not open %s", argv[1]);
            exit(1);
        }
        yyin = file;
    }
    yylex();
    printf("\n\n");
    return 0;
}
