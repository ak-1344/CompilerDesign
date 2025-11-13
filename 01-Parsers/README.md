# A3 - Parser Generators and Grammar Analysis

This folder contains implementations of various parsing algorithms and grammar analysis tools for compiler design.

## Contents

### LR Parser Generators
- **clr.cpp** - Canonical LR(1) parser generator
- **slr.cpp** - Simple LR parser generator  
- **lalr.cpp** - Look-Ahead LR parser generator
- **slrpar.cpp** - SLR parser with string parsing capabilities
- **Ex9.cpp** - SLR table generator (corrected implementation)
- **Ex10.cpp** - CLR table generator (modified version)
- **Ex11.cpp** - LALR table generator (modified version)

### Grammar Analysis Tools
- **ex1.cpp** - Predictive LL(1) parser with FIRST/FOLLOW sets
- **ex2.cpp** - FIRST and FOLLOW set calculator
- **ex3.cpp** - FIRST set calculator
- **ex4.cpp** - FOLLOW set calculator

### Text Files
- **clr.txt**, **slr.txt**, **lalr.txt** - Sample grammar inputs
- **clrpar.txt**, **slrpar.txt** - Sample grammars with parsing examples

## How to Compile and Run

### General Pattern
All C++ files follow the same build pattern:
```bash
g++ <filename>.cpp -o <filename>
./<filename>
```

### Examples

**Build and run CLR parser:**
```bash
g++ clr.cpp -o clr
./clr
```

**Build and run LL(1) parser:**
```bash
g++ ex1.cpp -o ex1
./ex1
```

**Build and run SLR parser:**
```bash
g++ slr.cpp -o slr
./slr
```

### LALR parser:
```bash
g++ lalr.cpp -o lalr
./lalr
```

### FIRST/FOLLOW calculator:
```bash
g++ ex2.cpp -o ex2
./ex2
```

## Input Format

Most programs accept grammar productions in the format:
- `A->xyz` (where A is a non-terminal and xyz is a string of terminals/non-terminals)
- Use `e` for epsilon (ε) productions
- Example: `E->E+T` or `S->e`

## Usage

1. Run the compiled executable
2. Enter the number of productions when prompted
3. Input each production in the specified format
4. The program will compute and display:
   - FIRST and FOLLOW sets (where applicable)
   - Parse tables (ACTION and GOTO tables)
   - States of the automaton
   - Conflict detection results

## Notes

- Programs using `#include <bits/stdc++.h>` require a GCC/G++ compiler
- Some programs are hardcoded for specific grammar examples
- Text files contain sample inputs for testing the parsers
