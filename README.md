# Compiler Design - Complete Implementation Repository

![Compiler Design](https://img.shields.io/badge/Course-Compiler%20Design-blue)
![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-orange)
![Tools](https://img.shields.io/badge/Tools-Flex%2FBison-green)
![Status](https://img.shields.io/badge/Status-Complete-success)

A comprehensive collection of compiler design implementations covering the complete compilation pipeline from lexical analysis to code optimization. This repository contains **20+ working programs** with detailed documentation, perfect for learning and exam preparation.

---

## 📚 Table of Contents

- [Overview](#overview)
- [Repository Structure](#repository-structure)
- [Quick Start](#quick-start)
- [Detailed Modules](#detailed-modules)
- [Build Instructions](#build-instructions)
- [Documentation](#documentation)
- [Learning Path](#learning-path)
- [Prerequisites](#prerequisites)
- [Contributing](#contributing)

---

## 🎯 Overview

This repository implements all major phases of a compiler:

1. **Lexical Analysis** - Tokenizing source code
2. **Syntax Analysis** - Parsing with LL(1), SLR, CLR, LALR
3. **Semantic Analysis** - Type checking and symbol tables
4. **Intermediate Code Generation** - Three-address code
5. **Code Optimization** - Constant folding, propagation, dead code elimination
6. **Code Generation** - Assembly code generation

### Key Features

✅ **Complete Working Code** - All programs compile and run  
✅ **Interactive Examples** - User-friendly input/output  
✅ **Step-by-Step Execution** - Detailed traces for learning  
✅ **Comprehensive Documentation** - 3000+ lines in `final.md`  
✅ **Exam Ready** - Covers all important concepts  

---

## 📁 Repository Structure

```
CompilerDesign/
│
├── 01-Parsers/                    # Parser Implementations
│   ├── ll1_parser.cpp             # LL(1) Predictive Parser
│   ├── predictive_parser.cpp      # Predictive Parser with String Parsing
│   ├── operator_precedence_parser.cpp  # Operator Precedence Parser
│   ├── slr_parser_complete.cpp    # Complete SLR(1) Parser
│   ├── slr.cpp                    # SLR Parser Generator
│   ├── clr.cpp                    # CLR(1) Parser
│   ├── lalr.cpp                   # LALR Parser
│   ├── first_follow_calculator.cpp # FIRST/FOLLOW Set Calculator
│   ├── grammar_analyzer.cpp       # Grammar Analysis Tools
│   └── left_recursion_eliminator.cpp # Left Recursion Removal
│
├── 02-FlexBison/                  # Lexer/Parser with Flex & Bison
│   ├── calculator.*               # Simple Calculator
│   ├── expression_evaluator.*     # Expression Evaluator
│   ├── language_anbn_unequal.*    # Language {a^n b^m | n≠m}
│   └── language_pattern.*         # Complex Pattern Language
│
├── 03-Optimization/               # Code Generation & Optimization
│   ├── tac_generator.*            # Three-Address Code Generator
│   ├── constant_folding_optimizer.cpp  # Optimizer with Multiple Techniques
│   └── tac_to_assembly.cpp        # TAC to 8086 Assembly Converter
│
├── 04-LexicalAnalysis/            # Lexical Analyzers
│   ├── manual_lexer.cpp           # Hand-written Lexer in C++
│   ├── flex_lexer.l               # Flex-based Lexer
│   └── sample_input.c             # Test Input File
│
├── 05-PatternMatching/            # Advanced Pattern Matching
│   ├── word_frequency_counter.l   # Word Counter
│   ├── email_validator.l          # Email Pattern Matcher
│   ├── number_pattern_matcher.l   # Number Format Recognizer
│   ├── comment_remover.l          # C/C++ Comment Remover
│   └── advanced_pattern_matcher.l # Multi-pattern Matcher
│
├── final.md                       # Complete Documentation (3000+ lines)
└── README.md                      # This file
```

---

## 🚀 Quick Start

### Prerequisites

Install required tools:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential flex bison

# macOS
brew install flex bison

# Verify installation
flex --version
bison --version
g++ --version
```

### Clone and Build

```bash
# Clone the repository
git clone https://github.com/ak-1344/CompilerDesign.git
cd CompilerDesign

# Build a C++ parser
cd 01-Parsers
g++ ll1_parser.cpp -o ll1_parser
./ll1_parser

# Build a Flex/Bison program
cd ../02-FlexBison
flex calculator.l
bison -d calculator.y
gcc calculator.tab.c lex.yy.c -o calculator
./calculator

# Build an optimizer
cd ../03-Optimization
g++ constant_folding_optimizer.cpp -o optimizer
./optimizer
```

---

## 📖 Detailed Modules

### 1️⃣ Parsers (01-Parsers/)

Implements various parsing techniques:

#### **LL(1) Predictive Parser** (`ll1_parser.cpp`)
- Computes FIRST and FOLLOW sets
- Builds predictive parse table
- Detects LL(1) conflicts
- **Run**: `g++ ll1_parser.cpp -o ll1 && ./ll1`

#### **SLR Parser** (`slr_parser_complete.cpp`)
- Constructs LR(0) items
- Builds canonical collection
- Generates ACTION/GOTO tables
- Step-by-step parsing simulation
- **Run**: `g++ slr_parser_complete.cpp -o slr && ./slr`

#### **Operator Precedence Parser** (`operator_precedence_parser.cpp`)
- Computes LEADING/TRAILING sets
- Builds precedence table
- Parses arithmetic expressions
- **Run**: `g++ operator_precedence_parser.cpp -o opprec && ./opprec`

**Example Input:**
```
Enter number of productions: 3
E->E+T
E->T
T->i
```

### 2️⃣ Flex/Bison (02-FlexBison/)

Real-world lexer/parser implementations:

#### **Calculator** (`calculator.*`)
Evaluates arithmetic expressions with proper precedence.

```bash
flex calculator.l
bison -d calculator.y
gcc calculator.tab.c lex.yy.c -o calc
./calc
```

**Example:**
```
2 + 3 * 4
Result: 14
```

#### **Language Recognizer** (`language_anbn_unequal.*`)
Recognizes strings with unequal a's and b's: {a^n b^m | n ≠ m}

```bash
flex language_anbn_unequal.l
bison -d language_anbn_unequal.y
gcc language_anbn_unequal.tab.c lex.yy.c -o lang
./lang
```

### 3️⃣ Optimization (03-Optimization/)

Code generation and optimization techniques:

#### **Three-Address Code Generator** (`tac_generator.*`)
Converts high-level code to TAC with control flow.

```bash
flex tac_generator.l
bison -d tac_generator.y
gcc tac_generator.tab.c lex.yy.c -o tac
./tac
```

**Input:**
```c
x = 5;
y = 10;
if (x < y) {
    z = x + y;
}
```

**Output:**
```
x = 5
y = 10
t1 = x < y
if t1 == 0 goto L1
t2 = x + y
z = t2
L1:
```

#### **Constant Folding Optimizer** (`constant_folding_optimizer.cpp`)
Performs multiple optimizations:
- Constant folding: `2 + 3` → `5`
- Constant propagation
- Algebraic simplification: `x * 0` → `0`
- Strength reduction: `x * 8` → `x << 3`

```bash
g++ constant_folding_optimizer.cpp -o optimizer
./optimizer
```

#### **TAC to Assembly** (`tac_to_assembly.cpp`)
Converts TAC to 8086 assembly code.

```bash
g++ tac_to_assembly.cpp -o asm
./asm
```

### 4️⃣ Lexical Analysis (04-LexicalAnalysis/)

Two approaches to lexical analysis:

#### **Manual Lexer** (`manual_lexer.cpp`)
Hand-written lexer recognizing:
- Keywords (if, while, for, int, etc.)
- Identifiers
- Numbers
- Operators
- Comments

```bash
g++ manual_lexer.cpp -o lexer
./lexer < sample_input.c
```

#### **Flex Lexer** (`flex_lexer.l`)
Pattern-based lexer using regular expressions.

```bash
flex flex_lexer.l
gcc lex.yy.c -o flexlexer
./flexlexer < sample_input.c
```

### 5️⃣ Pattern Matching (05-PatternMatching/)

Advanced pattern recognition with Flex:

#### **Word Frequency Counter** (`word_frequency_counter.l`)
```bash
flex word_frequency_counter.l
gcc lex.yy.c -o wordcount
./wordcount
```

#### **Email Validator** (`email_validator.l`)
Validates email addresses using regex patterns.

#### **Comment Remover** (`comment_remover.l`)
Removes C/C++ single and multi-line comments.

---

## 🔨 Build Instructions

### C++ Programs

```bash
# Generic build command
g++ <filename>.cpp -o <output>
./<output>

# With debugging symbols
g++ -g <filename>.cpp -o <output>

# With optimizations
g++ -O2 <filename>.cpp -o <output>
```

### Flex/Bison Programs

```bash
# Step 1: Generate lexer
flex <name>.l

# Step 2: Generate parser
bison -d <name>.y

# Step 3: Compile
gcc <name>.tab.c lex.yy.c -o <output>

# Step 4: Run
./<output>

# One-liner
flex <name>.l && bison -d <name>.y && gcc <name>.tab.c lex.yy.c -o <output> && ./<output>
```

### Common Issues

**Problem**: `flex: command not found`
```bash
sudo apt-get install flex bison
```

**Problem**: `undefined reference to 'yywrap'`
```
Solution: Add %option noyywrap to flex file
```

**Problem**: `shift/reduce conflict`
```
Solution: Add precedence declarations in bison file:
%left '+' '-'
%left '*' '/'
```

---

## 📘 Documentation

### Main Documentation

📄 **[final.md](final.md)** - Complete documentation (3000+ lines) including:
- Full source code for all programs
- Detailed explanations
- Example sessions with input/output
- Theory concepts (FIRST/FOLLOW, parse tables, etc.)
- Optimization techniques
- Assembly code generation
- Troubleshooting guide
- Exam preparation checklist

### Quick Reference Tables

#### Parser Comparison

| Parser | States | Power | Conflicts | Used By |
|--------|--------|-------|-----------|---------|
| LL(1) | N/A | Weakest | Common | Recursive Descent |
| SLR | Fewest | Weak | Many | Educational |
| LALR | Medium | Strong | Few | Yacc/Bison |
| CLR | Most | Strongest | Rare | Theoretical |

#### Optimization Techniques

1. **Constant Folding**: `2 + 3` → `5`
2. **Constant Propagation**: Replace vars with constants
3. **Dead Code Elimination**: Remove unused code
4. **Strength Reduction**: `x * 8` → `x << 3`
5. **Algebraic Simplification**: `x + 0` → `x`
6. **Common Subexpression Elimination**
7. **Loop Optimization**

---

## 🎓 Learning Path

### Recommended Study Order

#### Week 1-2: Lexical Analysis
- Start with `04-LexicalAnalysis/manual_lexer.cpp`
- Learn `05-PatternMatching/` programs
- Understand tokens, patterns, regular expressions

#### Week 3-4: Top-Down Parsing
- Study `01-Parsers/ll1_parser.cpp`
- Understand FIRST/FOLLOW sets
- Practice predictive parsing

#### Week 5-6: Bottom-Up Parsing
- Learn `01-Parsers/slr_parser_complete.cpp`
- Understand LR(0) items
- Study ACTION/GOTO tables

#### Week 7-8: Flex/Bison
- Practice all programs in `02-FlexBison/`
- Build custom grammars
- Understand semantic actions

#### Week 9-10: Code Generation
- Study `03-Optimization/tac_generator.*`
- Understand three-address code
- Learn control flow translation

#### Week 11-12: Optimization
- Work through `03-Optimization/constant_folding_optimizer.cpp`
- Implement various optimizations
- Understand data flow analysis

### Practice Exercises

1. **Parser Design**: Given grammar, build LL(1) or SLR parser
2. **FIRST/FOLLOW**: Calculate sets for given grammar
3. **TAC Generation**: Convert C code to three-address code
4. **Optimization**: Apply constant folding to TAC
5. **Flex/Bison**: Write lexer/parser for simple language

---

## 🧪 Testing

### Sample Test Cases

#### For Parsers
```
Grammar:
E -> E + T | T
T -> T * F | F  
F -> ( E ) | id

Test strings:
✓ id + id * id
✓ ( id + id ) * id
✗ id + + id
✗ ( id * id
```

#### For Lexer
```c
// Input
int main() {
    int x = 10;
    float y = 3.14;
    if (x > 5) return x;
}

// Expected tokens
KEYWORD: int
IDENTIFIER: main
OPERATOR: (
OPERATOR: )
...
```

#### For Optimizer
```
// Input TAC
t1 = 2
t2 = 3
t3 = t1 + t2
x = t3 * 0

// Expected output
x = 0
```

---

## 💡 Key Concepts

### Parsing Algorithms

**LL(1) Parsing**:
- Top-down
- Left-to-right scan
- Leftmost derivation
- 1 symbol lookahead

**SLR Parsing**:
- Bottom-up
- Simple LR(0) items
- Uses FOLLOW sets for reductions

**Operator Precedence**:
- Specialized for expressions
- Uses precedence relations: `<`, `>`, `=`

### Code Optimization

**Local Optimization** (within basic block):
- Constant folding
- Algebraic simplification
- Strength reduction

**Global Optimization** (across blocks):
- Constant propagation
- Dead code elimination
- Common subexpression elimination

### Three-Address Code

Standard form: `x = y op z`

Benefits:
- Easy to optimize
- Easy to generate assembly
- Explicit control flow

---

## 🛠️ Development Tools

### Recommended IDEs
- **VS Code** with C/C++ extension
- **CLion** (JetBrains)
- **Vim/Neovim** with LSP
- **Eclipse CDT**

### Debugging Tools
```bash
# GDB debugger
g++ -g program.cpp -o program
gdb ./program

# Valgrind for memory leaks
valgrind --leak-check=full ./program

# Print parse tree
bison -v parser.y  # Creates parser.output
```

### Useful Commands
```bash
# View generated parser states
bison -v -d parser.y
cat parser.output

# Trace flex execution
flex -d lexer.l

# Pretty print code
clang-format -i *.cpp

# Check for errors
gcc -Wall -Wextra -Werror program.c
```

---

## 📊 Statistics

- **Total Programs**: 20+
- **Lines of Code**: 5000+
- **Documentation**: 3000+ lines
- **Languages**: C, C++, Flex, Bison
- **Parsing Algorithms**: 7 (LL, SLR, CLR, LALR, etc.)
- **Optimization Techniques**: 7+
- **Pattern Matchers**: 5

---

## 🤝 Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-parser`)
3. Commit changes (`git commit -am 'Add new parser'`)
4. Push to branch (`git push origin feature/new-parser`)
5. Create Pull Request

### Guidelines
- Follow existing code style
- Add comments and documentation
- Include example inputs/outputs
- Test thoroughly before submitting

---

## 📝 License

This project is created for educational purposes as part of a Compiler Design course.

---

## 👨‍💻 Author - GOD

**Course**: Compiler Design  
**Year**: 2024-2025

---

## 📞 Support

For questions or issues:
- Open an issue on GitHub
- Check `final.md` for detailed documentation
- Review code comments in source files

---

## 🌟 Acknowledgments

- Course instructors for guidance
- Dragon Book (Compilers: Principles, Techniques, and Tools)
- Flex and Bison documentation
- Open source compiler projects

---

## 🎯 Exam Preparation

### Important Topics
- [ ] FIRST and FOLLOW set computation
- [ ] LL(1) parse table construction
- [ ] LR(0) item sets and closures
- [ ] SLR/LALR/CLR differences
- [ ] Three-address code generation
- [ ] Optimization techniques
- [ ] Regular expressions and DFA
- [ ] Semantic analysis

### Practice Questions
1. Given grammar, compute FIRST/FOLLOW
2. Build LL(1) or SLR parse table
3. Parse given string step-by-step
4. Generate TAC for code snippet
5. Apply optimizations to TAC
6. Convert TAC to assembly

### Useful Formulas

**FIRST Set**:
```
FIRST(a) = {a}  if a is terminal
FIRST(ε) = {ε}
FIRST(X₁X₂...Xₙ) = FIRST(X₁) - {ε} ∪ ... (if X₁ derives ε)
```

**FOLLOW Set**:
```
$ ∈ FOLLOW(S)  for start symbol S
A → αBβ: FIRST(β) - {ε} ⊆ FOLLOW(B)
A → αB or β →* ε: FOLLOW(A) ⊆ FOLLOW(B)
```

---

## 🔗 Quick Links

- **Documentation**: [final.md](final.md)
- **Parsers**: [01-Parsers/](01-Parsers/)
- **Flex/Bison**: [02-FlexBison/](02-FlexBison/)
- **Optimization**: [03-Optimization/](03-Optimization/)
- **Lexical Analysis**: [04-LexicalAnalysis/](04-LexicalAnalysis/)
- **Pattern Matching**: [05-PatternMatching/](05-PatternMatching/)

---

**Last Updated**: December 2024  
**Version**: 2.0  
**Status**: ✅ Complete and Ready for Finals

---

⭐ **Star this repository if you find it helpful!**

Happy Compiling! 🚀
