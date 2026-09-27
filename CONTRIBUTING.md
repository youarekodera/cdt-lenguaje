# Contributing to CodeTime

Thank you for your interest in contributing to CodeTime! This document
outlines how to set up your development environment, run tests, and submit
changes.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Project Structure](#project-structure)
- [Running Tests](#running-tests)
- [Coding Standards](#coding-standards)
- [Submitting Changes](#submitting-changes)
- [Reporting Issues](#reporting-issues)
- [Documentation](#documentation)

## Code of Conduct

Be respectful and constructive in all interactions. Harassment or
discriminatory behavior of any kind will not be tolerated.

## Getting Started

1. Fork the repository on GitHub.
2. Clone your fork locally:

   ```bash
   git clone https://github.com/your-username/cdt-lenguaje.git
   cd cdt-lenguaje
   ```

3. Add the upstream remote:

   ```bash
   git remote add upstream https://github.com/youarekodera/cdt-lenguaje.git
   ```

## Development Setup

**Prerequisites:**

- Linux (currently the only supported platform)
- GCC or Clang
- Make or CMake

**Building the compiler:**

```bash
make
# or with CMake:
mkdir -p build-cmake && cd build-cmake
cmake .. && make
```

**Installing locally (optional):**

```bash
sudo make install
```

## Project Structure

```
cdt-lenguaje/
├── compiler/        # Compiler source code (lexer, parser, AST, codegen)
├── docs/            # Language documentation
├── examples/        # Sample CodeTime programs (.cdt files)
├── tests/           # Test suite
├── scripts/         # Build, install, and uninstall scripts
├── CMakeLists.txt   # CMake build configuration
└── Makefile         # Makefile build target
```

## Running Tests

```bash
./tests/run_tests.sh
```

For CMake-based builds, see `tests/CMakeLists.txt`.

## Coding Standards

- Follow the existing code style in the `compiler/` directory.
- Use clear, descriptive names for variables, functions, and types.
- Write comments explaining *why*, not *what*.
- Keep functions small and focused on a single responsibility.
- Ensure your code compiles cleanly with `-Wall -Wextra`.

## Submitting Changes

1. Create a new branch for your feature or bugfix:

   ```bash
   git checkout -b my-feature
   ```

2. Make your changes, keeping commits focused and messages clear.

3. Run the test suite to ensure nothing is broken:

   ```bash
   ./tests/run_tests.sh
   ```

4. Push to your fork and open a pull request:

   ```bash
   git push origin my-feature
   ```

5. In the PR description, explain:
   - What changes you made
   - Why they are needed
   - Any trade-offs or alternatives considered

## Reporting Issues

Use the [GitHub Issues](https://github.com/youarekodera/cdt-lenguaje/issues)
page to report bugs or request features. Please include:

- A clear title and description
- Steps to reproduce (for bugs)
- The expected and actual behavior
- Your OS, compiler version, and CodeTime version (`codetime version`)
- Relevant code snippets or error output

## Documentation

Improvements to documentation are just as valuable as code changes! Docs
live in `docs/` and cover:

- [Language Specification](docs/language.md)
- [Syntax Reference](docs/syntax.md)
- [Standard Library](docs/standard-library.md)
- [Learning Guide](docs/learning-guide.md)

If you add a new feature, please update the relevant documentation.

---

By contributing to CodeTime, you agree that your contributions will be
licensed under the GNU General Public License v3.0.
