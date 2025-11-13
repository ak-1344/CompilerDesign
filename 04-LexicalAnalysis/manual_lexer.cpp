// Lexical analyzer: tokenizes C code into keywords, identifiers, operators, literals. Compile: g++ i.cpp -o i && ./i
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
bool isDelimiter(char ch)
{
    return (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
            ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
            ch == '[' || ch == ']' || ch == '{' || ch == '}');
}
bool isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '>' || ch == '<' ||
            ch == '=');
}
bool validIdentifier(char *str)
{
    return !(str[0] >= '0' && str[0] <= '9') && !isDelimiter(str[0]);
}
bool isKeyword(char *str)
{
    return (!strcmp(str, "if") || !strcmp(str, "else") ||
            !strcmp(str, "while") || !strcmp(str, "do") || !strcmp(str, "break") || !strcmp(str, "continue") ||
            !strcmp(str, "int") || !strcmp(str, "double") ||
            !strcmp(str, "float") || !strcmp(str, "return") ||
            !strcmp(str, "char") || !strcmp(str, "case") ||
            !strcmp(str, "sizeof") || !strcmp(str, "long") ||
            !strcmp(str, "short") || !strcmp(str, "typedef") ||
            !strcmp(str, "switch") || !strcmp(str, "unsigned") ||
            !strcmp(str, "void") || !strcmp(str, "static") ||
            !strcmp(str, "struct") || !strcmp(str, "goto"));
}
bool isInteger(char *str)
{
    int i, len = strlen(str);
    if (len == 0)
        return false;
    for (i = 0; i < len; i++)
    {

        if (!isdigit(str[i]) || (str[i] == '-' && i > 0))

            return false;
    }
    return true;
}
bool isRealNumber(char *str)
{
    int i, len = strlen(str);
    bool hasDecimal = false;
    if (len == 0)
        return false;
    for (i = 0; i < len; i++)
    {

        if (!isdigit(str[i]) && str[i] != '.' ||

            (str[i] == '-' && i > 0))

            return false;

        if (str[i] == '.')

            hasDecimal = true;
    }
    return hasDecimal;
}
char *subString(char *str, int left, int right)
{
    int i;
    char *subStr = (char *)malloc(sizeof(char) * (right - left + 2));
    for (i = left; i <= right; i++)

        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return subStr;
}
void parse(char *str, FILE *outputFile)
{
    int left = 0, right = 0;
    int len = strlen(str);
    bool expectingIdentifier = false;
    while (right <= len && left <= right)
    {

        if (!isDelimiter(str[right]))

            right++;

        if (isDelimiter(str[right]) && left == right)
        {

            if (isOperator(str[right]))

                fprintf(outputFile, "'%c' IS AN OPERATOR\n", str[right]);

            right++;

            left = right;
        }
        else if (isDelimiter(str[right]) && left != right || (right == len && left != right))
        {

            char *subStr = subString(str, left, right - 1);

            if (isKeyword(subStr))
            {

                fprintf(outputFile, "'%s' IS A KEYWORD\n", subStr);

                expectingIdentifier = true;
            }
            else if (expectingIdentifier)
            {

                if (validIdentifier(subStr))
                {

                    fprintf(outputFile, "'%s' IS A VALID IDENTIFIER\n", subStr);
                }
                else
                {

                    fprintf(outputFile, "'%s' IS NOT A VALID IDENTIFIER\n",
                            subStr);
                }

                expectingIdentifier = false;
            }
            else if (isInteger(subStr))
            {

                fprintf(outputFile, "'%s' IS AN INTEGER\n", subStr);
            }
            else if (isRealNumber(subStr))
            {

                fprintf(outputFile, "'%s' IS A REAL NUMBER\n", subStr);
            }
            else if (validIdentifier(subStr) && !isDelimiter(str[right - 1]))
            {

                fprintf(outputFile, "'%s' IS A VALID IDENTIFIER\n", subStr);
            }
            else if (!validIdentifier(subStr) && !isDelimiter(str[right - 1]))
            {

                fprintf(outputFile, "'%s' IS NOT A VALID IDENTIFIER\n", subStr);
            }

            free(subStr);

            left = right;
        }
    }
    return;
}
void parseAndPrint(char *str)
{
    int left = 0, right = 0;
    int len = strlen(str);
    bool expectingIdentifier = false;
    while (right <= len && left <= right)
    {

        if (!isDelimiter(str[right]))

            right++;

        if (isDelimiter(str[right]) && left == right)
        {

            if (isOperator(str[right]))

                printf("'%c' IS AN OPERATOR\n", str[right]);

            right++;

            left = right;
        }
        else if (isDelimiter(str[right]) && left != right || (right == len && left != right))
        {

            char *subStr = subString(str, left, right - 1);

            if (isKeyword(subStr))
            {

                printf("'%s' IS A KEYWORD\n", subStr);

                expectingIdentifier = true;
            }
            else if (expectingIdentifier)
            {

                if (validIdentifier(subStr))
                {

                    printf("'%s' IS A VALID IDENTIFIER\n", subStr);
                }
                else
                {

                    printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
                }

                expectingIdentifier = false;
            }
            else if (isInteger(subStr))
            {

                printf("'%s' IS AN INTEGER\n", subStr);
            }
            else if (isRealNumber(subStr))
            {

                printf("'%s' IS A REAL NUMBER\n", subStr);
            }
            else if (validIdentifier(subStr) && !isDelimiter(str[right - 1]))
            {

                printf("'%s' IS A VALID IDENTIFIER\n", subStr);
            }
            else if (!validIdentifier(subStr) && !isDelimiter(str[right - 1]))
            {

                printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
            }

            free(subStr);

            left = right;
        }
    }
    return;
}
void tokenizeFromArray(char *statement)
{
    parseAndPrint(statement);
}
void tokenizeFromFile(char *filename)
{
    FILE *file, *outputFile;
    char line[256];
    file = fopen(filename, "r");
    outputFile = fopen("output.txt", "w");
    if (file == NULL || outputFile == NULL)
    {

        printf("Could not open file.\n");

        return;
    }
    while (fgets(line, sizeof(line), file))
    {

        parse(line, outputFile);
    }
    fclose(file);
    fclose(outputFile);
    printf("Tokenized Output is stored in output.txt\n");
}
void tokenizeFromUserInput()
{
    FILE *file;
    char line[256];
    int lineCount = 0;
    file = fopen("input.txt", "w");
    if (file == NULL)
    {

        printf("Could not open file.\n");

        return;
    }
    printf("Enter a small C program (up to 5 lines):\n");
    while (lineCount < 5 && fgets(line, sizeof(line), stdin))
    {

        fprintf(file, "%s", line);

        lineCount++;
    }
    fclose(file);
    tokenizeFromFile("input.txt");
}
int main()
{

    int choice;
    char statement[256];
    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Tokenize a simple statement stored in a linear array\n");
        printf("2. Tokenize a small program stored in a text file\n");
        printf("3. Tokenize a small program input by the user and stored in a text file\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:

            printf("Enter a simple statement:\n");

            fgets(statement, sizeof(statement), stdin);

            statement[strcspn(statement, "\n")] = '\0';

            tokenizeFromArray(statement);

            break;
        case 2:

            tokenizeFromFile("input.txt");

            break;
        case 3:

            tokenizeFromUserInput();

            break;
        case 4:

            exit(0);
        default:

            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}