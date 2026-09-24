# CodeTime Language Specification

## Overview

CodeTime is a modern, high-level programming language designed for readability, simplicity, and expressiveness. It features clean English-based syntax, strong static typing, and compiles to native code via C.

## Language Philosophy

CodeTime prioritizes:
- **Readability**: Code should read like natural language
- **Simplicity**: Minimal syntax and clear concepts
- **Expressiveness**: Powerful features with clean syntax
- **Type Safety**: Strong static typing with type inference
- **Modern Features**: Pattern matching, generics, contracts
- **Performance**: Native compilation for efficiency

## Hello World

```codetime
start
    print "Hello, CodeTime!"
```

## Program Structure

A CodeTime program consists of:
- Optional module declaration
- Import statements
- Declarations (functions, objects, contracts, choices)
- Entry point (`start` block)

### Module Declaration

```codetime
module myapp
```

### Imports

```codetime
use math
use users
use graphics as gfx
use users Person, User
```

## Variables

### Declaration with Type Annotation

```codetime
let username as text = "Alice"
let age as number = 25
let active as boolean = true
```

### Type Inference

```codetime
let name = "CodeTime"        # Inferred as text
let version = 1              # Inferred as number
let enabled = true           # Inferred as boolean
```

### Mutable Variables

```codetime
let counter as number = 0
change counter to counter + 1
```

### Constants

```codetime
fixed max_users as number = 1000
```

## Types

### Primitive Types

- `number`: Integer values
- `decimal`: Floating-point values
- `text`: String values
- `character`: Single character
- `boolean`: true/false values
- `nothing`: Null/void type

### Collection Types

- `list`: Ordered sequences
- `map`: Key-value pairs
- `set`: Unique values

### Optional Types

```codetime
let username as maybe text
```

## Functions

### Definition

```codetime
define add using a as number and b as number
    give a + b
```

### Calling

```codetime
let result = add using 10 and 20
```

### Void Functions

```codetime
define greet using name as text
    print "Hello, {name}"
```

## Control Flow

### Conditional Statements

```codetime
when temperature > 30
    print "It is hot"
otherwise when temperature > 20
    print "It is warm"
otherwise
    print "It is cold"
```

### Repeat Loop

```codetime
repeat number from 1 until 10
    print number
```

### For Each Loop

```codetime
for each user inside users
    print user
```

### While Loop

```codetime
while counter < 10
    change counter to counter + 1
```

## Collections

### Lists

```codetime
let names = ["Alice", "Bob", "Charlie"]
print names at 0
names add "David"
names remove "Bob"
```

### Maps

```codetime
let user = {
    name: "Alice"
    age: 25
    active: true
}
print user name
```

## Objects

### Definition

```codetime
object Person
    property name as text
    property age as number

    create using name as text and age as number
        set self name to name
        set self age to age

    action introduce
        print "My name is {self name}"
```

### Instantiation

```codetime
let person = create Person using "Alice" and 25
person introduce
```

## Contracts (Interfaces)

```codetime
contract Drawable
    action draw

object Circle follows Drawable
    action draw
        print "Drawing circle"
```

## Enumerations (Choices)

```codetime
choice Direction
    North
    South
    East
    West

let direction = Direction North
```

## Error Handling

```codetime
attempt
    let result = divide 10 by 0
    print result
recover error
    print error message
```

### Raising Errors

```codetime
raise "Something went wrong"
```

## Pattern Matching

```codetime
choose value
    case 1
        print "One"
    case 2
        print "Two"
    otherwise
        print "Something else"
```

## String Interpolation

```codetime
let name = "Alice"
let age = 25
print "My name is {name} and I am {age} years old"
```

### Multiline Strings

```codetime
let message = """
Welcome to CodeTime.
This is a multiline message.
"""
```

## Anonymous Functions

```codetime
let double = value => value * 2
```

## Generics

```codetime
object Box of Item
    property value as Item

let box as Box of text
```

## Comments

### Single-line

```codetime
# This is a comment
```

### Multi-line

```codetime
###
This is a
multiline comment
###
```

## Operators

### Arithmetic
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo
- `**` Power

### Comparison
- `==` Equal
- `!=` Not equal
- `<` Less than
- `<=` Less than or equal
- `>` Greater than
- `>=` Greater than or equal

### Logical
- `and` Logical AND
- `or` Logical OR
- `not` Logical NOT

## Type System

CodeTime is statically typed with type inference. The compiler infers types from initializers when possible, but explicit type annotations are supported and recommended for function parameters and return values.

### Type Checking

The compiler performs comprehensive type checking:
- Variable declarations
- Function arguments and return values
- Assignment compatibility
- Operator validity
- Member access

## Memory Management

CodeTime uses automatic memory management. Users do not need to manually allocate or free memory. The runtime handles memory cleanup automatically.

## Standard Library

The CodeTime standard library includes modules for:
- `system`: System operations
- `text`: String manipulation
- `math`: Mathematical functions
- `collections`: Advanced collections
- `files`: File I/O
- `network`: Network operations
- `time`: Date and time
- `random`: Random number generation
- `json`: JSON parsing and generation
- `process`: Process management

## Compilation

CodeTime compiles to native code via C:

```bash
codetime build program.cdt
codetime run program.cdt
codetime check program.cdt
```

## Language Design Principles

1. **English-based syntax**: Keywords and constructs use natural English
2. **Minimal punctuation**: Avoid unnecessary symbols
3. **Indentation-based blocks**: Clean, readable structure
4. **Strong typing**: Catch errors at compile time
5. **Type inference**: Reduce boilerplate when types are obvious
6. **Explicit entry point**: Clear program start with `start`
7. **Original identity**: Distinct from Python, C, and other languages
