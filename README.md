# Quasar Compiler

A complete, multi-phase compiler for the Quasar programming language, translating a custom C-like high-level language into executable 32-bit x86 assembly code.

## Overview

The Quasar Compiler is a comprehensive implementation of modern compiler theory, demonstrating the entire compilation pipeline from source code tokenization to machine code generation. This project serves as an educational tool for computer science students, a portfolio piece for aspiring systems programmers, and a reference for language design enthusiasts.

## The Quasar Language

Quasar is a small, statically-typed procedural language with syntax familiar to C, Java, and JavaScript developers. It emphasizes clarity and simplicity while maintaining robust type safety.

### Language Features

#### Variables and Types
- **Data Types**: `Int` (integers), `Dub` (double-precision floating-point), `Bool` (true/false)
- **Variable Declaration**: `let my_var = 10;` (mutable)
- **Constant Declaration**: `const PI = 3.14;` (immutable)
- **String Literals**: Supported for output via `print`

#### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`
- **Relational & Equality**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical**: `&&` (AND with short-circuiting), `||` (OR), `!` (NOT)
- **Assignment**: `=`, `+=`, `-=`, `*=`, `/=`, `%=`
- **Increment/Decrement**: `++`, `--` (prefix and postfix)

#### Control Flow
- **Conditionals**: `if-else` statements with nesting support
- **Loops**: `for` and `while` loops
- **Loop Control**: `break` and `continue` statements

#### Built-in Functions
- **Output**: `print "Hello, World!";`
- **Program Exit**: `return` statement with integer exit code

## Sample Programs

### Example 1: for Loop with break

```quasar
// File: examples/for_loop_break.lang

print "--- Starting Calculation ---";

const THRESHOLD = 100;
let final_result = 0;

for (let i = 1; i <= 10; i = i + 1) {
    final_result = final_result + (i * 5);

    if (final_result > THRESHOLD) {
        print "Threshold exceeded!";
        break;
    }
}

print "--- Calculation Complete ---";
return final_result;
```

### Example 2: Nested if-else with Logical Operators

```quasar
// File: examples/nested_if.lang

print "--- Program Start ---";

const score_math: Int = 90;
const score_science: Int = 78;
const passing_grade: Int = 60;
let extra_credit: Int = 5;
let final_status: Int = 0;

if (score_math >= passing_grade) {
    print "Math score is passing.";

    if ((score_science + extra_credit) > 80 && score_math > 85) {
        print "Science score with extra credit is also excellent.";
        final_status = (score_math * 2) - (score_science % 10);
    } else {
        print "Science score is good, but not in the top tier.";
        final_status = -1;
    }
} else {
    print "Math score is failing.";
    final_status = -99;
}

print "--- Program End ---";
return final_status;
```

## Compiler Architecture

The Quasar Compiler implements a classic five-phase compilation pipeline:

```
Source Code → [Lexer] → Tokens → [Parser] → AST → [Semantic Analyzer] → 
Annotated AST → [ICG] → TAC → [Code Generator] → Assembly
```

### Compilation Phases

1. **Lexical Analysis (Flex)**: Scans source text and converts it into a token stream
2. **Syntax Analysis (Bison)**: Validates tokens against language grammar and builds an Abstract Syntax Tree (AST)
3. **Semantic Analysis**: Performs type checking and scope resolution using a Symbol Table, annotating the AST
4. **Intermediate Code Generation (ICG)**: Translates annotated AST into machine-independent Three-Address Code (TAC)
5. **Final Code Generation**: Converts TAC into executable 32-bit x86 assembly code (AT&T syntax)

## Building and Running

### Prerequisites

- 32-bit MinGW environment (MSYS2 recommended for Windows)
- Flex (lexical analyzer generator)
- Bison (parser generator)
- GCC (GNU Compiler Collection)

### Build Instructions

Execute the following commands in the project's root directory:

#### Step 1: Generate Parser and Scanner

```bash
bison -dy parser.y
flex scanner.l
```

#### Step 2: Build the Quasar Compiler

```bash
gcc y.tab.c lex.yy.c symtab.c semantic.c ast_printer.c icg.c codegen.c
```

This produces `a.exe` (the Quasar compiler executable).

#### Step 3: Compile a Quasar Program

```bash
./a.exe
```

This reads `input.lang` and generates `output.s` (assembly file).

#### Step 4: Build the Final Executable

```bash
gcc -m32 -o final_program.exe main.c output.s
```

#### Step 5: Run Your Program

```bash
./final_program.exe
```

## Project Structure

```
quasar-compiler/
├── parser.y              # Bison grammar specification
├── scanner.l             # Flex lexical specification
├── symtab.c             # Symbol table implementation
├── semantic.c           # Semantic analysis
├── ast_printer.c        # AST visualization
├── icg.c                # Intermediate code generation
├── codegen.c            # x86 assembly code generation
├── main.c               # Host program wrapper
└── examples/            # Sample Quasar programs
```

## License

This project is licensed under the MIT License - see below for details.

```
MIT License

Copyright (c) 2025 Quasar Compiler Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Contributing

### How to Contribute

Contributions are welcome and appreciated! This project follows a standard fork-and-pull-request workflow. If you wish to contribute, please adhere to the following steps to ensure a smooth process.

#### 1. Fork the Repository

Click the "Fork" button at the top-right corner of this page to create your own copy of the repository.

#### 2. Clone Your Fork

Clone your forked repository to your local machine.

```bash
git clone https://github.com/YOUR_USERNAME/Compiler.git
```

#### 3. Create a New Branch

Before making any changes, create a new branch for your feature or bug fix. Do not commit directly to the `main` branch. This is a critical step.

```bash
# For a new feature:
git checkout -b feature/MyNewFeature

# For a bug fix:
git checkout -b fix/BugDescription
```

#### 4. Make Your Changes

Implement your changes, additions, or bug fixes. Ensure that your code builds correctly by following the build steps outlined above.

#### 5. Commit Your Changes

Commit your work with a clear and descriptive commit message.

```bash
git commit -m "Add: Implement a cool new feature"
```

#### 6. Push to Your Branch

Push your new branch to your forked repository on GitHub.

```bash
git push origin feature/MyNewFeature
```

#### 7. Open a Pull Request

Navigate to the original repository and click "New Pull Request". Provide a clear description of your changes and submit the pull request for review.

---

**Happy Compiling! 🚀** We appreciate your contributions to making Quasar better!