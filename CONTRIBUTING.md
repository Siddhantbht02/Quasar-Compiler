# Contributing to the Quasar Compiler Project

First off, thank you for considering contributing to the Quasar Compiler! Your help is greatly appreciated. This project is a collaborative effort, and we welcome contributions from everyone.

This document provides a set of guidelines for contributing to the project. Following these guidelines helps to ensure a smooth and effective process for everyone involved.

## Code of Conduct

This project and everyone participating in it is governed by the Quasar Compiler Code of Conduct. By participating, you are expected to uphold this code. Please report any unacceptable behavior to the project maintainers.

## How Can I Contribute?

There are many ways to contribute, from reporting bugs to implementing new language features.

### Reporting Bugs

If you find a bug in the source code or a mistake in the documentation, you can help us by submitting an issue to our GitHub Repository.

When you are creating a bug report, please include as many details as possible:

* **A clear and descriptive title** for the issue.
* **Steps to reproduce the bug** in a clear, step-by-step fashion.
* **The exact `input.lang` code** that causes the bug.
* **What you expected to happen** versus **what actually happened**.
* **The full output** from the compiler and the final program, including any error messages.
* **Your system environment** (e.g., Windows with MSYS2, Linux, macOS) and the versions of your build tools (`gcc --version`, `bison --version`, `flex --version`).

### Suggesting Enhancements

If you have an idea for a new feature for the Quasar language or an improvement to the compiler's architecture, you can also submit an issue. Please provide:

* **A clear and descriptive title** for the enhancement suggestion.
* A detailed description of the proposed feature and why it would be useful.
* A sample of what the Quasar code might look like with the new feature.

## Your First Code Contribution

Ready to contribute code? Here is the standard workflow for submitting a pull request.

### 1. Fork the Repository

Start by clicking the "Fork" button at the top-right corner of the repository page. This will create a personal copy of the project under your own GitHub account.

### 2. Clone Your Fork

Clone your forked repository to your local machine to start making changes.

```bash
git clone https://github.com/YOUR_USERNAME/Compiler.git
cd Compiler
```

### 3. Create a New Branch

This is a critical step. Do not make changes directly on your main branch. Create a new, descriptive branch for your work. This keeps your changes organized and separate from the main codebase.

* For a new feature:

```bash
git checkout -b feature/add-string-concatenation
```

* For a bug fix:

```bash
git checkout -b fix/incorrect-codegen-for-gte
```

### 4. Make Your Changes

Implement your feature or bug fix. As you work, ensure your code:

* Follows the existing coding style.
* Builds successfully without errors. Run the build process described in the `README.md` frequently to check your work.
* Is tested with relevant examples in the `examples/` directory.

### 5. Commit Your Changes

Commit your work with a clear, concise commit message. A good commit message explains the "what" and the "why" of your change.

```bash
git add .
git commit -m "Fix: Corrected operand order for GTE operator in codegen"
```

### 6. Push to Your Branch

Push your new branch to your forked repository on GitHub.

```bash
git push origin fix/incorrect-codegen-for-gte
```

### 7. Open a Pull Request

Navigate to the original Quasar Compiler repository on GitHub. You should see a prompt to create a pull request from your newly pushed branch. Click "Compare & pull request" and provide:

* A clear title for your pull request.
* A detailed description of what your changes do and why they are needed.
* A reference to any related issues (e.g., "Fixes #42").

The project maintainers will review your pull request and may request changes or provide feedback. Be prepared to make revisions based on their comments.

## Style Guidelines

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature").
* Use the imperative mood ("Move cursor to..." not "Moves cursor to...").
* Limit the first line to 72 characters or less.
* Reference issues and pull requests liberally after the first line.
* Consider starting the commit message with an applicable prefix:
  * `feat:` for new features
  * `fix:` for bug fixes
  * `docs:` for documentation changes
  * `refactor:` for code refactoring
  * `test:` for adding or updating tests

### Code Style

* Follow the existing code formatting and naming conventions in the project.
* Write clear, self-documenting code with meaningful variable and function names.
* Add comments for complex logic or non-obvious implementations.
* Keep functions focused and modular.

## Testing

Before submitting your pull request, please:

* Test your changes thoroughly with various input examples.
* Ensure the compiler builds without warnings or errors.
* Verify that existing functionality is not broken by your changes.
* Add new test cases in the `examples/` directory if applicable.

## Questions?

If you have any questions about contributing, feel free to:

* Open an issue on GitHub with the "question" label.
* Reach out to the project maintainers.

Thank you for contributing to the Quasar Compiler! 🚀
