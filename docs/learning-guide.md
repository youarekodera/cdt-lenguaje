# CodeTime — Complete Learning Guide
## From Zero to Advanced Level

> **Navigation:** [Documentation Index](README.md) · [Language Spec](language.md) · [Syntax Reference](syntax.md) · [Standard Library](standard-library.md) · [Learning Guide](learning-guide.md)

---

## Table of Contents

1. [Introduction to CodeTime](#1-introduction-to-codetime)
2. [Installation and Setup](#2-installation-and-setup)
3. [Language Fundamentals](#3-language-fundamentals)
4. [Variables and Data Types](#4-variables-and-data-types)
5. [Operators and Expressions](#5-operators-and-expressions)
6. [Control Flow](#6-control-flow)
7. [Functions](#7-functions)
8. [Collections](#8-collections)
9. [Object-Oriented Programming](#9-object-oriented-programming)
10. [Error Handling](#10-error-handling)
11. [Advanced Features](#11-advanced-features)
12. [Standard Library](#12-standard-library)
13. [Practical Projects](#13-practical-projects)
14. [Best Practices](#14-best-practices)
15. [Additional Resources](#15-additional-resources)
16. [Learning Path](#learning-path)
17. [Exercises](#exercises)

---

## 1. Introduction to CodeTime

CodeTime is a modern programming language designed to be:

- Easy to read and write
- Statically typed
- Multi-paradigm
- Compiled to native code
- With English-based syntax

**Why learn CodeTime?**

- Clean and expressive syntax
- Gentle learning curve
- Growing community
- High-performance applications
- Ideal for beginners and experts alike

---

## 2. Installation and Setup

**System requirements:**

- Linux (currently)
- GCC or Clang
- Make or CMake

**Installation from source:**

1. Clone or download the repository
2. Navigate to the project directory
3. Run: `make`
4. (Optional) Run: `sudo make install`

**Verify installation:**

```bash
codetime version
codetime help
```

**Your first program:**

Create a file called `hello.cdt`:

```codetime
start
    print "Hello, CodeTime!"
```

Compile and run:

```bash
codetime build hello.cdt
./hello
```

---

## 3. Language Fundamentals

**Basic program structure:**

A CodeTime program consists of:

- Module declarations (optional)
- Imports
- Declarations (variables, functions, objects)
- Entry point (`start` block)

**Basic syntax:**

- No semicolons at end of lines
- No braces for blocks
- Indentation defines blocks (like Python)
- Keywords in English

**Comments:**

```codetime
# Single-line comment

###
Multiline
comment
###
```

---

## 4. Variables and Data Types

**Variable declaration:**

```codetime
let name as text = "Juan"
let age as number = 25
let active as boolean = true
```

**Type inference:**

```codetime
let name = "Maria"      # Inferred as text
let age = 30            # Inferred as number
let price = 19.99       # Inferred as decimal
```

**Mutable variables:**

```codetime
let counter as number = 0
change counter to counter + 1
```

**Constants:**

```codetime
fixed PI as decimal = 3.14159
fixed MAX_USERS as number = 1000
```

**Primitive data types:**

- `number`: Integer values (42, -10, 1000)
- `decimal`: Floating-point values (3.14, -0.5, 2.0)
- `text`: String values ("Hello", "World")
- `character`: Individual characters ('a', 'Z')
- `boolean`: True/false values (true, false)
- `nothing`: Represents absence of value

**Practical example:**

```codetime
start
    let name as text = "Carlos"
    let age as number = 28
    let is_student as boolean = true

    print "Name: {name}"
    print "Age: {age}"
    print "Is student: {is_student}"
```

---

## 5. Operators and Expressions

**Arithmetic operators:**

| Operator | Description |
|----------|-------------|
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division |
| `%` | Modulo (remainder) |
| `**` | Power |

```codetime
let a = 10
let b = 3
let sum = a + b         # 13
let rest = a - b        # 7
let product = a * b     # 30
let division = a / b    # 3
let modulo = a % b      # 1
let power = a ** b      # 1000
```

**Comparison operators:**

| Operator | Description |
|----------|-------------|
| `==` | Equal to |
| `!=` | Not equal to |
| `<` | Less than |
| `<=` | Less than or equal to |
| `>` | Greater than |
| `>=` | Greater than or equal to |

```codetime
let x = 5
let y = 10
x < y       # true
x == y      # false
x != y      # true
```

**Logical operators:**

| Operator | Description |
|----------|-------------|
| `and` | Logical AND |
| `or` | Logical OR |
| `not` | Logical NOT |

```codetime
let a = true
let b = false
a and b     # false
a or b      # true
not a       # false
```

**Operator precedence (highest to lowest):**

1. `**` (power)
2. `*`, `/`, `%`
3. `+`, `-`
4. `<`, `<=`, `>`, `>=`
5. `==`, `!=`
6. `not`
7. `and`
8. `or`

---

## 6. Control Flow

**Conditional statements (`when/otherwise`):**

```codetime
when age >= 18
    print "You are an adult"
otherwise
    print "You are a minor"
```

**Multiple conditions:**

```codetime
when temperature > 30
    print "It is hot"
otherwise when temperature > 20
    print "It is warm"
otherwise
    print "It is cold"
```

**Repeat loop (range-based):**

```codetime
repeat i from 1 until 10
    print "Iteration: {i}"
```

**For each loop (iterate over collections):**

```codetime
for each name inside names
    print "Hello, {name}"
```

**While loop (conditional):**

```codetime
while counter < 10
    change counter to counter + 1
```

**Practical example — Even numbers:**

```codetime
start
    repeat number from 1 until 20
        when number % 2 == 0
            print "{number} is even"
```

---

## 7. Functions

**Function definition:**

```codetime
define greet using name as text
    print "Hello, {name}"
```

**Functions with return value:**

```codetime
define add using a as number and b as number
    give a + b
```

**Calling functions:**

```codetime
greet using "Ana"
let result = add using 5 and 3
```

**Functions with multiple parameters:**

```codetime
define calculate_average using a as number and b as number and c as number
    give (a + b + c) / 3

let avg = calculate_average using 10 and 20 and 30
```

**Void functions (procedures):**

```codetime
define print_message
    print "This is a message"

print_message
```

**Practical example — Simple calculator:**

```codetime
define add using a as number and b as number
    give a + b

define subtract using a as number and b as number
    give a - b

define multiply using a as number and b as number
    give a * b

define divide using a as number and b as number
    give a / b

start
    let x = 10
    let y = 5

    print "Sum: {add using x and y}"
    print "Difference: {subtract using x and y}"
    print "Product: {multiply using x and y}"
    print "Quotient: {divide using x and y}"
```

---

## 8. Collections

**Lists (arrays):**

```codetime
let fruits = ["apple", "banana", "orange"]
print fruits at 0              # "apple"

fruits add "grape"              # Add element
fruits remove "banana"          # Remove element
```

**Maps (dictionaries):**

```codetime
let person = {
    name: "Pedro"
    age: 35
    city: "Madrid"
}

print person name               # "Pedro"
print person age                # 35
```

**Common list operations:**

```codetime
let numbers = [1, 2, 3, 4, 5]
numbers add 6               # [1, 2, 3, 4, 5, 6]
numbers remove 3            # [1, 2, 4, 5, 6]
```

**Practical example — Inventory management:**

```codetime
let inventory = {
    apples: 10
    oranges: 15
    pears: 8
}

print "Apples: {inventory apples}"
change inventory apples to inventory apples + 5
print "Updated apples: {inventory apples}"
```

---

## 9. Object-Oriented Programming

**Object definition:**

```codetime
object Person
    property name as text
    property age as number

    create using name as text and age as number
        set self name to name
        set self age to age

    action introduce
        print "My name is {self name} and I am {self age} years old"
```

**Object instantiation:**

```codetime
let person = create Person using "Laura" and 30
person introduce
```

**Objects with multiple actions:**

```codetime
object Rectangle
    property width as number
    property height as number

    create using width as number and height as number
        set self width to width
        set self height to height

    action area
        give self width * self height

    action perimeter
        give 2 * (self width + self height)

let rect = create Rectangle using 10 and 5
print "Area: {rect area}"
print "Perimeter: {rect perimeter}"
```

**Contracts (interfaces):**

```codetime
contract Drawable
    action draw

object Circle follows Drawable
    property radius as number

    action draw
        print "Drawing circle of radius {self radius}"
```

---

## 10. Error Handling

**Error handling with `attempt/recover`:**

```codetime
attempt
    let result = divide 10 by 0
    print result
recover error
    print "An error occurred"
```

**Raising errors:**

```codetime
raise "Custom error message"
```

**Practical example — Input validation:**

```codetime
define process_age using age as number
    when age < 0
        raise "Age cannot be negative"
    otherwise when age > 150
        raise "Age is not realistic"
    otherwise
        print "Valid age: {age}"

start
    attempt
        process_age using 25
    recover error
        print "Error: {error}"
```

---

## 11. Advanced Features

**Enumerations (choices):**

```codetime
choice DayOfWeek
    Monday
    Tuesday
    Wednesday
    Thursday
    Friday
    Saturday
    Sunday

let today = DayOfWeek Monday
```

**Pattern matching (`choose/case`):**

```codetime
choose value
    case 1
        print "One"
    case 2
        print "Two"
    case 3
        print "Three"
    otherwise
        print "Other value"
```

**Optional types:**

```codetime
let user as maybe text

when user exists
    print "User: {user}"
otherwise
    print "No user"
```

**Generics:**

```codetime
object Container of Type
    property value as Type

let text_box as Container of text
let number_box as Container of number
```

**String interpolation:**

```codetime
let name = "Maria"
let age = 28
print "My name is {name} and I am {age} years old"
```

**Multiline strings:**

```codetime
let message = """
This is a message
with multiple lines
in CodeTime
"""
```

---

## 12. Standard Library

**Module imports:**

```codetime
use math
use text
use files
```

**Mathematical functions:**

```codetime
use math

let root = math sqrt 25
let random = math random
```

**Text manipulation:**

```codetime
use text

let text = "Hello World"
let length = text length text
let upper = text upper text
```

**File operations:**

```codetime
use files

let content = files read "data.txt"
files write "output.txt" with content
```

---

## 13. Practical Projects

### Project 1: Calculator

- Implement basic operations
- Error handling (division by zero)
- Simple user interface

### Project 2: Task Manager

- List of pending tasks
- Add, delete, mark as completed
- File persistence

### Project 3: Number Guessing Game

- Generate random number
- User interaction
- Attempt counter

### Project 4: Unit Converter

- Conversion between different units
- Support for length, weight, temperature
- User-friendly interface

### Project 5: Text Analyzer

- Count words, characters, lines
- Find most frequent words
- Basic statistics

---

## 14. Best Practices

**Descriptive names:**

```codetime
# Bad
let x = 10
let y = 20

# Good
let user_age = 10
let person_height = 20
```

**Proper comments:**

```codetime
# Calculates the area of a rectangle
let area = width * height
```

**Small, focused functions:**

```codetime
# Good
define calculate_area using width as number and height as number
    give width * height

# Avoid very long functions
```

**Consistent indentation:**

```codetime
# Always use 4 spaces
when condition
    print "Action"
```

**Error handling:**

```codetime
# Always validate inputs
attempt
    process data
recover error
    print "Error processing"
```

---

## 15. Additional Resources

**Official documentation:**

- [docs/language.md](language.md) — Language specification
- [docs/syntax.md](syntax.md) — Syntax reference
- [docs/standard-library.md](standard-library.md) — Standard library

**Code examples:**

- `examples/hello.cdt` — Hello world
- `examples/variables.cdt` — Variables and types
- `examples/functions.cdt` — Functions
- `examples/conditions.cdt` — Conditionals
- `examples/loops.cdt` — Loops
- `examples/objects.cdt` — Objects
- `examples/collections.cdt` — Collections

**Compiler commands:**

```
codetime help               - Show help
codetime version            - Show version
codetime check file.cdt     - Check for errors
codetime build file.cdt     - Compile program
codetime run file.cdt       - Compile and run
```

**Community and support:**

- GitHub Issues — Report bugs
- Documentation — Guides and tutorials
- Examples — Reference code

---

## Learning Path

### Level 1: Fundamentals (1-2 weeks)

1. Installation and first program
2. Variables and data types
3. Basic operators
4. Simple input/output
5. Basic conditionals

### Level 2: Control Structures (2-3 weeks)

1. Loops (`repeat`, `for each`, `while`)
2. Basic functions
3. Arrays and lists
4. String handling
5. Small projects

### Level 3: Modular Programming (3-4 weeks)

1. Advanced functions
2. Modules and imports
3. Basic object-oriented programming
4. Error handling
5. Intermediate projects

### Level 4: Advanced Features (4-6 weeks)

1. Advanced OOP (inheritance, polymorphism)
2. Contracts and interfaces
3. Generics
4. Pattern matching
5. Complex projects

### Level 5: Mastery (ongoing)

1. Code optimization
2. Complete standard library
3. Integration with C
4. Contributing to the project
5. Production projects

---

## Exercises

### Exercise 1: Prime Numbers

Write a program that determines whether a number is prime.

### Exercise 2: FizzBuzz

Write a program that prints:

- "Fizz" for multiples of 3
- "Buzz" for multiples of 5
- "FizzBuzz" for multiples of both
- The number in all other cases

### Exercise 3: Palindromes

Write a program that determines whether a word is a palindrome.

### Exercise 4: Fibonacci

Write a program that generates the Fibonacci sequence.

### Exercise 5: Sorting

Implement a sorting algorithm for a list.

---

## Conclusion

CodeTime is a modern and powerful language with a clean and expressive syntax. With this guide, you have everything you need to begin your journey from beginner to a competent CodeTime programmer.

Remember:

- Practice makes progress
- Errors are learning opportunities
- The community is here to help you
- Enjoy programming!

Welcome to the world of CodeTime!

---

© 2026 CodeTime Project — Learning Guide
