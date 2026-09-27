# CodeTime Changelog

All notable changes to the CodeTime language are documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/).

## [v0.0.1] - First Public Release

> Original single source of truth: `CODETIME_LEARNING_GUIDE.md` (now
> `docs/learning-guide.md`). See [Learning Guide](docs/learning-guide.md)
> for the full tutorial.

## What v0.0.1 includes

### Added — Files and Program Structure

- File extension **`.cdt`**.
- Entry point **`start`**.
- Blocks defined by **indentation** (no `{ }` braces, no `;`).
- Single-line comments `#` and multi-line comments `### … ###`.
- Optional module declaration `module name`.
- Optional imports `use module_name`.

Minimal example:

```codetime
start
    print "Hello, CodeTime!"
```

### Added — Data Types

- Primitives: `number`, `decimal`, `text`, `character`, `boolean`, `nothing`.
- Collections: `list` (ordered sequence), `map` (key-value).
- Optional types: `maybe T` and `exists`.
- Generics over objects: `object X of T`.

### Added — Variables

- `let` with inference or annotation: `let x as T = …`
- Mutable variables: `change x to …`
- Constants: `fixed`

### Added — Operators

- Arithmetic: `+ - * / % **`
- Comparison: `== != < <= > >=`
- Logical: `and or not`

### Added — Control Flow

- `when / otherwise when / otherwise`
- `repeat i from … until …`
- `for each … inside …`
- `while …`

### Added — Functions

- Definition: `define name using param as T …`
- Return: `give`
- Procedures (no return value)

### Added — Objects

- `object`, `property`, `create`, `action`, `self`, `set`
- Constructors: `create using …`

### Added — Contracts (Interfaces)

- `contract` and `object … follows …`

### Added — Enumerations

- `choice Name` with constructors `Name value`

### Added — Error Handling

- Capture: `attempt … recover error …`
- Raise: `raise "message"`

### Added — Pattern Matching

- `choose … case … otherwise`

### Added — Strings

- Interpolation: `"Text {variable}"`
- Multiline strings: `"""…"""`

### Added — Compilation and Commands

- `codetime help`
- `codetime version`
- `codetime check file.cdt`
- `codetime build file.cdt`
- `codetime run file.cdt`

## Not Included in v0.0.1 (Coming Soon)

> The English spec in `docs/` mentions some features that **do not yet
> exist** in this version:
>
> - **Anonymous functions / lambdas (`=>`)**: the parser responds *"Lambda
>   expressions not yet implemented"*.
> - **Parametrized collection types** (`list of text`, `map of …`,
>   `set of …`): not recognized by the parser.
> - **Concrete standard-library modules** (`math`, `text`, `files`, …):
>   `use` is recognized but none are resolved.
