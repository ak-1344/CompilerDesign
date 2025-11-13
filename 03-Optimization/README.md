# A5 - Intermediate Code Generation and Optimization

This folder contains programs for intermediate code generation and optimization techniques.

## Contents

### Intermediate Code Generators
- **ex15.l** - Lexer for intermediate code generation
- **ex15.y** - Parser that generates three-address code
- **ex15** - Compiled executable
- **ex15.tab.c**, **ex15.tab.h** - Generated parser files

### Code Optimizers
- **ex16.cpp** - Three-address code optimizer (constant folding and propagation)
- **ex17.cpp** - Dead code elimination optimizer
- **ex16**, **ex17** - Compiled executables

### Input Files
- **input.tac** - Sample three-address code for optimization
- **input1.tac** - Additional test input
- **test.in** - Test input file

### Generated Files
- **lex.yy.c** - Generated lexer code

## Prerequisites

- **For ex15:** Flex and Bison
- **For ex16, ex17:** C++ compiler (g++)

Install tools:
```bash
# Ubuntu/Debian
sudo apt-get install flex bison g++

# macOS
brew install flex bison gcc
```

## How to Build and Run

### ex15 - Intermediate Code Generator

**Build:**
```bash
flex ex15.l
bison -d ex15.y
gcc ex15.tab.c lex.yy.c -o ex15
```

**Run:**
```bash
./ex15
# Enter source code, and it will generate three-address code
```

### ex16 - Constant Folding/Propagation Optimizer

**Build:**
```bash
g++ ex16.cpp -o ex16
```

**Run with input file:**
```bash
./ex16 < input.tac
```

**Run interactively:**
```bash
./ex16
# Enter three-address code, press Ctrl+D when done
```

### ex17 - Dead Code Elimination

**Build:**
```bash
g++ ex17.cpp -o ex17
```

**Run with input file:**
```bash
./ex17 < input.tac
```

**Run interactively:**
```bash
./ex17
# Enter three-address code, press Ctrl+D when done
```

## Input Format

### Three-Address Code Format
The optimizers expect three-address code in the format:
```
t1 = 5
t2 = 10
t3 = t1 + t2
x = t3
```

**Supported operations:**
- Arithmetic: `+`, `-`, `*`, `/`
- Assignment: `variable = value`
- Comparisons: `<`, `>`, `<=`, `>=`, `==`, `!=`
- Control flow: `if`, `goto`, `label:`

## Examples

### Example: Constant Folding
**Input (input.tac):**
```
t1 = 5
t2 = 10
t3 = t1 + t2
x = t3
```

**Run optimizer:**
```bash
./ex16 < input.tac
```

**Output:** The optimizer will fold constants and propagate values, simplifying to `x = 15`

### Example: Dead Code Elimination
**Input:**
```
x = 5
y = 10
z = x + y
x = 20
print z
```

**Run optimizer:**
```bash
./ex17 < input.tac
```

**Output:** Removes unused assignment `x = 5` if it's dead code

## Optimization Techniques

### ex16 - Constant Folding and Propagation
- **Constant Folding:** Evaluates constant expressions at compile time
- **Constant Propagation:** Replaces variable uses with their constant values
- **Example:** `t1 = 2; t2 = 3; t3 = t1 + t2` → `t3 = 5`

### ex17 - Dead Code Elimination
- Removes assignments to variables that are never used
- Identifies unreachable code
- Performs liveness analysis

## File Descriptions

- **.l files** - Flex lexer specifications
- **.y files** - Bison parser specifications with semantic actions
- **.cpp files** - C++ optimization programs
- **.tac files** - Three-address code input samples

## Notes

- ex16 and ex17 read from stdin, so you can pipe input or redirect files
- The optimizers output optimized three-address code to stdout
- Generated code preserves program semantics while reducing instructions
- Test files (input.tac, input1.tac) contain sample intermediate code

## Troubleshooting

- **Compilation errors:** Ensure g++ supports C++11 or later
- **Missing flex/bison:** Install development tools package
- **Runtime errors:** Check input format matches three-address code syntax
