# lAB2P2 - Flex Pattern Matching Programs

This folder contains various Flex programs demonstrating pattern matching and text processing capabilities.

## Contents

### Flex Programs
- **ex6.l** - Word frequency counter
  - Searches for occurrences of a specific word in a file
  - Demonstrates file input and string matching

- **ex7.l** - Lexical analyzer program
  - Pattern matching for specific lexical patterns

- **ex8.l** - Pattern matching program
  - Custom pattern recognition

- **ex9.l** - Text processing program
  - Advanced text manipulation

- **b1.l** - Specialized lexical analysis task

### Executables
- **P2ex6**, **P2ex7**, **P2ex10** - Compiled Flex programs
- **a.out** - Default compiled output

### Supporting Files
- **test.txt** - Sample text file for testing
- **lex.yy.c** - Generated C code from Flex

## Prerequisites

Install Flex:
```bash
# Ubuntu/Debian
sudo apt-get install flex

# Fedora
sudo dnf install flex

# macOS
brew install flex
```

## How to Build and Run

### General Build Pattern
For any Flex program:
```bash
flex <filename>.l
gcc lex.yy.c -o <output_name>
./<output_name>
```

### Specific Examples

**ex6 - Word Frequency Counter:**
```bash
flex ex6.l
gcc lex.yy.c -o P2ex6
./P2ex6
# You'll be prompted to:
# 1. Enter the word to search
# 2. Enter the filename to search in
```

**Example session:**
```
$ ./P2ex6
Enter the word to search: hello
Enter the filename to search in: test.txt
The word 'hello' appeared 3 times in file 'test.txt'.
```

**ex7:**
```bash
flex ex7.l
gcc lex.yy.c -o P2ex7
./P2ex7
```

**ex8:**
```bash
flex ex8.l
gcc lex.yy.c -o ex8
./ex8
```

**ex9:**
```bash
flex ex9.l
gcc lex.yy.c -o ex9
./ex9
```

**b1:**
```bash
flex b1.l
gcc lex.yy.c -o b1
./b1
```

## Usage Examples

### Word Counter (ex6)
Create a test file:
```bash
echo "hello world hello flex hello programming" > test.txt
./P2ex6
# Enter: hello
# Enter: test.txt
# Output: The word 'hello' appeared 3 times in file 'test.txt'.
```

### Pattern Matching
Most programs read from stdin:
```bash
echo "sample input text" | ./ex7
```

Or from a file:
```bash
./ex8 < test.txt
```

## Features

### ex6 - Word Frequency Counter
- Prompts user for search word
- Prompts user for filename
- Opens and scans the file
- Counts occurrences of the exact word
- Case-sensitive matching

### General Flex Capabilities
- Pattern recognition using regular expressions
- Token counting and categorization
- File I/O operations
- String manipulation
- Custom actions on pattern matches

## Input/Output

### Interactive Mode
Programs read from:
- Standard input (keyboard)
- File (specified by user or command line)

Programs output to:
- Standard output (console)
- Can be redirected to files

### Redirect Output Example
```bash
./P2ex6 > output.txt
```

## File Descriptions

- **.l files** - Flex specifications with pattern rules and actions
- **lex.yy.c** - Auto-generated C code (rewritten with each flex command)
- **P2ex*** - Compiled executables (naming convention for Part 2 exercises)
- **test.txt** - Sample text file for testing programs

## Common Patterns in Flex

### Basic Structure of .l files:
```flex
%{
/* C declarations */
%}

/* Definitions */
%%
/* Rules */
pattern { action }
%%

/* User code */
int main() {
    yylex();
    return 0;
}
```

## Troubleshooting

**Program doesn't compile:**
- Ensure Flex is installed: `flex --version`
- Check for syntax errors in .l file

**File not found:**
- Ensure input file exists in current directory
- Check file permissions

**No output:**
- Verify input format matches expected patterns
- Check if program expects file input vs stdin

**yylex() undefined:**
- Ensure `int yywrap() { return 1; }` is defined
- Or use `%option noyywrap` in Flex file

## Advanced Usage

### With Input Redirection:
```bash
./P2ex7 < input.txt > output.txt
```

### With Pipes:
```bash
cat test.txt | ./ex8
```

### Multiple Files:
```bash
for file in *.txt; do
    echo "Processing $file"
    ./P2ex6 < "$file"
done
```

## Notes

- Programs use `yylex()` to start lexical analysis
- `yywrap()` handles end-of-file behavior
- Generated `lex.yy.c` is overwritten each time flex runs
- Most programs are designed for specific pattern matching tasks
- Refer to individual .l files for specific functionality

## Learning Resources

- Flex documentation: https://github.com/westes/flex
- Regular expressions: Essential for writing pattern rules
- Study the .l files to understand pattern-action syntax
