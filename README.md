# CodeTime

[![License](https://img.shields.io/badge/license-GPL%20v3-blue.svg)](LICENSE)
[![Codetime v0.0.1](https://img.shields.io/badge/version-v0.0.1-orange.svg)](#changelog)
[![CI](https://img.shields.io/badge/CI-not%20configured-lightgrey.svg)](#continuous-integration)

CodeTime is a modern, high-level programming language designed for
readability, simplicity, and expressiveness. It features clean English-based
syntax, strong static typing, and compiles to native code.

**Current version: v0.0.1** (first public release — see
[CHANGELOG.md](CHANGELOG.md) for what's included and what's coming next)

## Features

- **Clean Syntax**: English-based keywords with minimal punctuation
- **Strong Static Typing**: Type inference with explicit type support
- **Multi-Paradigm**: Supports functional, object-oriented, and procedural programming
- **Modern Features**: Pattern matching, generics, optional types, contracts
- **Standard Library**: Comprehensive modules for common tasks
- **Native Compilation**: Compiles to efficient native code via C

## Installation

```bash
make
sudo make install
```

## Quick Start

Create a file `hello.cdt`:

```codetime
start
    print "Hello, CodeTime!"
```

Compile and run:

```bash
codetime build hello.cdt
./hello
```

## Documentation

| Document | Description |
|---|---|
| [Language Specification](docs/language.md) | Full reference: types, functions, objects, contracts, error handling |
| [Syntax Reference](docs/syntax.md) | Lexical structure, grammar rules, operator precedence |
| [Standard Library](docs/standard-library.md) | All built-in modules and functions |
| [Learning Guide](docs/learning-guide.md) | Guided tutorial from beginner to advanced, with projects & exercises |

See the [Documentation Index](docs/README.md) for the complete overview.

## Examples

The [examples](examples/) directory contains sample CodeTime programs
demonstrating various language features.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines.

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for release notes.

## License

GNU General Public License v3.0 — see [LICENSE](LICENSE) for details.
This is free software; see the source for copying conditions. There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
