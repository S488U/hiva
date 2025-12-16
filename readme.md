# Espira Programming Language 🚀

Welcome to **Espira**, a modern, versatile, and evolving programming language designed to be intuitive, lightweight, and powerful. Espira bridges the gap between the simplicity of scripting languages and the structure of typed languages.

---

## 📜 Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Current Status](#current-status)
- [Installation & Usage](#installation--usage)
- [Advantages](#advantages)
- [Disadvantages](#disadvantages)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

---

## Introduction

Espira is a modern programming language that draws inspiration from scripting languages while maintaining type safety. Its design philosophy is simplicity, modularity, and ease of learning, making it ideal for prototyping, mathematical computation, and automation.

---

## Features

Here are the primary features of the Espira programming language:

- ✅ **Strong Typing with Scripting Syntax**: Clear variable declarations (`int`, `float`, `string`, `bool`) to prevent errors.
- ✅ **Advanced Math Engine**: Implements the **Shunting Yard Algorithm** to handle operator precedence (`*` before `+`), parentheses `()`, and negative numbers correctly.
- ✅ **String Interpolation**: Embed expressions directly into strings using `{}` (e.g., `"Value: {x + 1}"`).
- ✅ **Robust Parsing**: Handles multi-word strings and quoted text intelligently.
- ✅ **Interpreted Nature**: Runs directly from source without complex compilation steps.
- ✅ **Error Handling**: Provides specific error messages with line number tracking.

---

## Current Status

**Espira is currently in active development (v2.0).** 
The core interpreter has been recently refactored to support complex parsing logic.

### Implemented Features
- [x] Variable declarations (`let int`, `let string`, etc.).
- [x] Complex mathematical expressions with correct order of operations.
- [x] Recursive variable resolution (using variables inside expressions).
- [x] Comments support (`#` and `//`).
- [x] Type enforcement and conversion.

### Upcoming Features
- [ ] Control flow (`if/else` statements).
- [ ] Loops (`for`, `while`).
- [ ] User-defined functions.
- [ ] File I/O operations.

---

## Installation & Usage

Follow these steps to get started:

### Prerequisites
1. **C++ Compiler**: You need **GCC/G++** with C++17 support.
2. **Environment**: Linux, macOS, or Windows (via MinGW or WSL).

### Build Process
1. Clone the repository:
   ```bash
   git clone https://github.com/S488U/espira.git
   cd espira
   ```

2. Compile the interpreter:
   ```bash
   g++ -std=c++17 -o espira espira.cpp
   ```

### Usage

1. Create a script file (e.g., `example.esp`):
   ```javascript
   let int x = 5
   let float y = 6.8
   echo "Hello, x is {x} and y is {y}"
   echo "The result of 3 + 4 is {3 + 4}"
   echo "Complex math: {10 + 5 * 2}"
   ```

2. Run the script:
   ```bash
   ./espira example.esp
   ```

**Output:**
```text
Hello, x is 5 and y is 6.8
The result of 3 + 4 is 7
Complex math: 20
```

---

## Advantages

- **Simplicity:** Minimal boilerplate makes it beginner-friendly.
- **Accuracy:** Unlike basic interpreters, Espira respects mathematical rules (BODMAS/PEMDAS).
- **Type Safety:** Prevents common bugs by enforcing types at assignment (e.g., prevents assigning text to an integer).
- **Cross-Platform:** Written in standard C++, runs anywhere.

## Disadvantages

- **Performance:** As an interpreted language, it is slower than compiled languages like C++ or Rust.
- **Early Stage:** Missing control flow (if/else) and loops, limiting its use for complex algorithms currently.

---

## Examples

### 1. Variables and Strings
```javascript
let string name = "John Doe"
let int age = 25
echo "User {name} is {age} years old."
```

### 2. Complex Math (Shunting Yard)
Espira correctly handles order of operations:
```javascript
let int a = 10
let int b = 5
# Multiplication happens before addition
echo "Result: {a + b * 2}" 
# Output: 20 (not 30)
```

### 3. Parentheses and Negatives
```javascript
let int result = (10 + 2) * (5 - 3)
echo "Calculation: {result}" 
# Output: 24
```

### 4. Planned Syntax (Control Flow)
*Note: This feature is currently in development.*
```javascript
try {
    let int result = 10 / 0
} catch (error) {
    echo "Division by zero encountered!"
}
```

---

## Contributing

We welcome contributions! If you are interested in helping the Espira project grow, follow these steps:

1. Fork this repository.
2. Create a branch: `git checkout -b feature/your-feature-name`
3. Make changes and commit.
4. Push your branch: `git push origin feature/your-feature-name`
5. Open a pull request.

## License

This project is licensed under the [BSD 3-Clause License](https://opensource.org/licenses/BSD-3-Clause).

**Key Terms:**
- **Free to Use:** You are free to use, modify, and distribute this project for personal or non-commercial purposes.
- **Commercial Restrictions:** Redistribution or selling the software under your name is prohibited without written permission.

## Contact

**Author:** Shahabas Abdul Hameed  
**Email:** shahabassabbu12@gmail.com  
**GitHub Issues:** [Report a Bug](https://github.com/S488U/espira/issues)

### Build it. Test it. Evolve it. 
**Espira is just the beginning of a new programming journey.**