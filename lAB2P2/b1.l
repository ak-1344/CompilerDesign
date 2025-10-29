%{
#include <stdio.h>
int state = 0;
%}
%option noyywrap
%%
b {
    if (state == 0) state = 1;
    else if (state == 1) state = 1;
    else if (state == 2) state = 1;
    else if (state == 3) state = 3;
}
c {
    if (state == 1) state = 2;
    else if (state == 2) state = 0;
    else if (state == 3) state = 3;
}
a {
    if (state == 2) state = 3;
    else if (state == 3) state = 3;
}
[abc] {
    if (state != 3) state = 0;
}
\n {
    if (state == 3)
        printf("Accepted (contains bca)\n");
    else
        printf("Rejected\n");
    state = 0;
}
.|\t|  ;
%%
int main() {
    printf("Enter strings (Ctrl+D to end):\n");
    yylex();
    return 0;
}
