# A4 - Flex and Bison Parser Implementations

This folder contains lexer-parser implementations using Flex (lexical analyzer) and Bison (parser generator).

## Contents

### Calculator (`calc`)
- **calc.l** - Lexer for arithmetic calculator
- **calc.y** - Parser for arithmetic expressions
- **calc** - Compiled executable
- **calc.tab.c**, **calc.tab.h** - Generated parser files

### Infix Expression Evaluator (`infix`)
- **infix.l** - Lexer for infix expressions
- **infix.y** - Parser with operator precedence
- **infix** - Compiled executable
- **infix.tab.c**, **infix.tab.h** - Generated parser files

### Language Parser 1 (`lang1`)
- **lang1.l** - Lexer for simple language
- **lang1.y** - Parser for simple language constructs
- **lang1** - Compiled executable
- **lang1.tab.c**, **lang1.tab.h** - Generated parser files

### Language Parser 2 (`lang2`)
- **lang2.l** - Lexer for language parser
- **lang2.y** - Parser for language constructs
- **lang2** - Compiled executable
- **lang2.tab.c**, **lang2.tab.h** - Generated parser files

### Generated Files
- **lex.yy.c** - Generated lexer C code (from most recent flex run)

## Prerequisites

Install Flex and Bison:
```bash
# Ubuntu/Debian
sudo apt-get install flex bison

# Fedora
sudo dnf install flex bison

# macOS
brew install flex bison
```

## How to Build and Run

### General Build Pattern
For each project (calc, infix, lang1, lang2):

```bash
flex <name>.l               # Generate lexer
bison -d <name>.y          # Generate parser with header
gcc <name>.tab.c lex.yy.c -o <name>   # Compile
./<name>                    # Run
```

### Specific Examples

**Calculator:**
```bash
flex calc.l
bison -d calc.y
gcc calc.tab.c lex.yy.c -o calc
./calc
```

**Infix Expression Evaluator:**
```bash
flex infix.l
bison -d infix.y
gcc infix.tab.c lex.yy.c -o infix
./infix
```

**Language Parser 1:**
```bash
flex lang1.l
bison -d lang1.y
gcc lang1.tab.c lex.yy.c -o lang1
./lang1
```

**Language Parser 2:**
```bash
flex lang2.l
bison -d lang2.y
gcc lang2.tab.c lex.yy.c -o lang2
./lang2
```

## Usage

### Calculator
Enter arithmetic expressions interactively:
```
1 + 2 * 3
5 - 2
10 / 2
```
Press `Ctrl+D` (Unix) or `Ctrl+Z` (Windows) to exit.

### Infix/Language Parsers
Follow the on-screen prompts or provide input according to the grammar defined in the `.y` files.

## File Descriptions

- **.l files** - Flex lexer specifications (tokenization rules)
- **.y files** - Bison parser specifications (grammar rules and actions)
- **.tab.c/.tab.h** - Generated parser C code and header
- **lex.yy.c** - Generated lexer C code

## Notes

- The `-d` flag in bison generates the header file needed by the lexer
- Compiled executables may already be present in the directory
- To clean generated files: `rm *.tab.c *.tab.h lex.yy.c`
- Link math library if needed: `gcc ... -lm`

## Troubleshooting

If you get compilation errors:
- Ensure Flex and Bison are installed
- Check that file names match in `#include` statements
- Some systems may require `-lfl` flag: `gcc calc.tab.c lex.yy.c -lfl -o calc`
