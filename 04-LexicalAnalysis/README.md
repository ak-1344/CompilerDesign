# lAB2 - Lexical Analysis

This folder contains implementations of lexical analyzers using both manual C++ implementation and Flex.

## Contents

### Manual Lexical Analyzer
- **i.cpp** - Hand-written lexical analyzer in C++
  - Tokenizes C-like source code
  - Identifies keywords, identifiers, operators, numbers, strings, and comments
  - Categorizes tokens and validates syntax

### Flex-based Lexical Analyzer
- **ii.l** - Flex specification for lexical analysis
  - Rule-based tokenization using regular expressions
  - Generates lex.yy.c when processed with Flex

### Supporting Files
- **test.c** - Sample C source code for testing the lexical analyzers
- **input.txt** - Additional test input
- **output.txt** - Sample output from lexical analysis
- **lex.yy.c** - Generated lexer code from Flex
- **2i**, **a.out** - Compiled executables

## Prerequisites

**For i.cpp:**
- C++ compiler (g++)

**For ii.l:**
- Flex (lexical analyzer generator)

Install on Ubuntu/Debian:
```bash
sudo apt-get install g++ flex
```

Install on macOS:
```bash
brew install gcc flex
```

## How to Build and Run

### i.cpp - Manual Lexical Analyzer

**Build:**
```bash
g++ i.cpp -o i
```

**Run with test file:**
```bash
./i < test.c
```

**Run interactively:**
```bash
./i
# Type or paste C code
# Press Ctrl+D (Unix) or Ctrl+Z (Windows) when done
```

### ii.l - Flex Lexical Analyzer

**Build:**
```bash
flex ii.l
gcc lex.yy.c -o ii
```

**Run with test file:**
```bash
./ii < test.c
```

**Run interactively:**
```bash
./ii
# Enter input text
# Press Ctrl+D when done
```

## Features

### i.cpp Capabilities
The manual lexical analyzer can identify:
- **Keywords:** if, else, while, for, int, float, char, return, etc.
- **Identifiers:** Variable and function names
- **Operators:** +, -, *, /, =, <, >, ==, !=, etc.
- **Numbers:** Integers and floating-point numbers
- **String Literals:** Text enclosed in quotes
- **Comments:** Single-line (//) and multi-line (/* */)
- **Special Symbols:** Parentheses, braces, brackets, semicolons

### ii.l Capabilities
The Flex-based analyzer uses pattern matching to:
- Recognize tokens based on regular expressions
- Categorize input into token classes
- Handle whitespace and comments
- Support extensible rule-based tokenization

## Input/Output Example

### Sample Input (test.c):
```c
int main() {
    int x = 10;
    float y = 3.14;
    if (x > 5) {
        return x + y;
    }
}
```

### Sample Output:
```
Keyword: int
Identifier: main
Operator: (
Operator: )
Operator: {
Keyword: int
Identifier: x
Operator: =
Number: 10
Operator: ;
...
```

## Testing

**Test the manual analyzer:**
```bash
echo "int x = 10;" | ./i
```

**Test the Flex analyzer:**
```bash
echo "int x = 10;" | ./ii
```

**Use provided test file:**
```bash
./i < test.c
./ii < test.c
```

## File Descriptions

- **i.cpp** - C++ implementation with manual state machine
- **ii.l** - Flex specification file with token rules
- **test.c** - Sample C program for testing
- **input.txt** - Additional test cases
- **output.txt** - Expected output examples
- **lex.yy.c** - Auto-generated C code from Flex

## Notes

- i.cpp provides more detailed token categorization
- ii.l is more concise and easier to modify
- Both analyzers read from stdin and write to stdout
- The manual analyzer is educational; Flex is more practical for real compilers
- Output format may differ between the two implementations

## Comparison

| Feature | i.cpp | ii.l |
|---------|-------|------|
| Implementation | Manual C++ | Flex rules |
| Flexibility | Hardcoded logic | Pattern-based |
| Ease of modification | Medium | Easy |
| Learning value | High | Medium |
| Production use | No | Yes |

## Troubleshooting

- **Compilation errors:** Ensure C++11 or later
- **Flex not found:** Install flex package
- **No output:** Check input redirection or pipe
- **Wrong tokens:** Verify input syntax and token rules
