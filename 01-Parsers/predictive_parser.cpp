// Grammar analyzer: computes and displays FIRST and FOLLOW sets. Compile: g++ ex2.cpp -o ex2 && ./ex2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TERMINALS 20
#define MAX_NON_TERMINALS 20
#define MAX_PRODUCTIONS 50
#define MAX_LEN 50

// Structure for parse table entries
typedef struct {
    char row;       // Non-terminal
    char col;       // Terminal
    char production[MAX_LEN];
} ParseEntry;

typedef struct {
    char terminals[MAX_TERMINALS];
    char non_terminals[MAX_NON_TERMINALS];
    ParseEntry table[MAX_PRODUCTIONS];
    int terminal_count;
    int non_terminal_count;
    int table_count;
    char start_symbol;
} PredictiveParser;

// Stack implementation
typedef struct {
    char items[100];
    int top;
} Stack;

void push(Stack *s, char c) {
    s->items[++s->top] = c;
}

char pop(Stack *s) {
    return (s->top >= 0) ? s->items[s->top--] : '\0';
}

char peek(Stack *s) {
    return (s->top >= 0) ? s->items[s->top] : '\0';
}

int is_terminal(PredictiveParser *p, char c) {
    for (int i = 0; i < p->terminal_count; i++)
        if (p->terminals[i] == c)
            return 1;
    return 0;
}

int is_non_terminal(PredictiveParser *p, char c) {
    for (int i = 0; i < p->non_terminal_count; i++)
        if (p->non_terminals[i] == c)
            return 1;
    return 0;
}

char* get_production(PredictiveParser *p, char row, char col) {
    for (int i = 0; i < p->table_count; i++) {
        if (p->table[i].row == row && p->table[i].col == col)
            return p->table[i].production;
    }
    return NULL;
}

void display_table(PredictiveParser *p) {
    printf("\n=== PARSE TABLE ===\n");
    printf("%10s", " ");
    for (int i = 0; i < p->terminal_count; i++) {
        printf("%10c", p->terminals[i]);
    }
    printf("\n---------------------------------------------\n");

    for (int i = 0; i < p->non_terminal_count; i++) {
        printf("%10c", p->non_terminals[i]);
        for (int j = 0; j < p->terminal_count; j++) {
            char *prod = get_production(p, p->non_terminals[i], p->terminals[j]);
            if (prod)
                printf("%10s", prod);
            else
                printf("%10s", "ERROR");
        }
        printf("\n");
    }
}

void parse_string(PredictiveParser *p, char *input) {
    Stack stack;
    stack.top = -1;
    push(&stack, '$');
    push(&stack, p->start_symbol);

    strcat(input, "$");
    int ip = 0;
    printf("\n=== PARSING STRING: %s ===\n", input);

    printf("%5s%15s%15s%25s\n", "Step", "Stack", "Input", "Action");
    printf("---------------------------------------------------------------\n");

    int step = 1;
    while (stack.top >= 0) {
        char top = peek(&stack);
        char curr = input[ip];

        // Display stack
        printf("%5d%15s%15s", step++, "", input + ip);
        for (int i = 0; i <= stack.top; i++)
            printf("%c", stack.items[i]);
        printf("%10s", "");

        if (is_terminal(p, top)) {
            if (top == curr) {
                pop(&stack);
                ip++;
                printf(" Match & advance (%c)\n", curr);
                if (top == '$')
                    break;
            } else {
                printf(" Error: Terminal mismatch (%c != %c)\n", top, curr);
                return;
            }
        } else if (is_non_terminal(p, top)) {
            char *prod = get_production(p, top, curr);
            if (prod) {
                pop(&stack);
                printf(" Expand %c->%s\n", top, prod);
                if (strcmp(prod, "e") != 0) {
                    for (int i = strlen(prod) - 1; i >= 0; i--)
                        push(&stack, prod[i]);
                }
            } else {
                printf(" Error: No production for %c,%c\n", top, curr);
                return;
            }
        } else {
            printf(" Error: Invalid symbol on stack\n");
            return;
        }
    }

    if (input[ip] == '$' && stack.top == -1)
        printf("\n*** STRING ACCEPTED ***\n");
    else
        printf("\n*** STRING REJECTED ***\n");
}

int main() {
    PredictiveParser parser;
    parser.terminal_count = 0;
    parser.non_terminal_count = 0;
    parser.table_count = 0;

    printf("=== PREDICTIVE PARSING ALGORITHM (C IMPLEMENTATION) ===\n\n");

    // Input terminals
    printf("Enter number of terminals (including $): ");
    scanf("%d", &parser.terminal_count);
    printf("Enter terminals (space separated): ");
    for (int i = 0; i < parser.terminal_count; i++)
        scanf(" %c", &parser.terminals[i]);

    // Input non-terminals
    printf("Enter number of non-terminals: ");
    scanf("%d", &parser.non_terminal_count);
    printf("Enter non-terminals (space separated): ");
    for (int i = 0; i < parser.non_terminal_count; i++)
        scanf(" %c", &parser.non_terminals[i]);

    parser.start_symbol = parser.non_terminals[0];
    printf("Start symbol set to: %c\n", parser.start_symbol);

    // Input parse table
    printf("\n=== Enter Parse Table Entries ===\n");
    printf("Format: row col production (use e for epsilon)\n");
    printf("Type END to stop\n");

    while (1) {
        char row[5], col[5], prod[MAX_LEN];
        printf("Entry: ");
        scanf("%s", row);
        if (strcmp(row, "END") == 0)
            break;
        scanf("%s %s", col, prod);
        parser.table[parser.table_count].row = row[0];
        parser.table[parser.table_count].col = col[0];
        strcpy(parser.table[parser.table_count].production, prod);
        parser.table_count++;
    }

    display_table(&parser);

    // Parsing multiple strings
    while (1) {
        char input[50];
        printf("\nEnter string to parse (or EXIT to quit): ");
        scanf("%s", input);
        if (strcmp(input, "EXIT") == 0)
            break;
        parse_string(&parser, input);
        printf("\n----------------------------------------------\n");
    }

    printf("\n=== PREDICTIVE PARSING COMPLETE ===\n");
    return 0;
}
