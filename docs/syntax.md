# CodeTime Syntax Reference

## Lexical Structure

### Identifiers

Identifiers start with a letter or underscore, followed by letters, digits, or underscores.

```
username
_user_name
value1
```

### Keywords

Reserved keywords in CodeTime:

```
start, let, change, fixed, as, to, define, using, give, when, otherwise,
repeat, until, while, for, each, inside, object, property, create, action,
set, self, contract, follows, choice, attempt, recover, raise, use, module,
case, choose, maybe, exists, of, nothing, true, false, and, or, not, print
```

### Literals

#### Number Literals

```
42
-10
1000
```

#### Decimal Literals

```
3.14
-0.5
2.0
```

#### String Literals

```
"Hello, World!"
"Line 1\nLine 2"
```

#### Multiline String Literals

```
"""
This is a
multiline string
"""
```

#### Character Literals

```
'a'
'Z'
'\n'
```

#### Boolean Literals

```
true
false
```

### Comments

#### Single-line Comments

```
# This is a comment
```

#### Multi-line Comments

```
###
This is a
multiline comment
###
```

## Grammar

### Program

```
program ::= module-declaration? import* declaration* start-block
```

### Module Declaration

```
module-declaration ::= "module" identifier
```

### Import Statement

```
import ::= "use" identifier ( "as" identifier )?
        | "use" identifier ( identifier ( "," identifier )* )?
```

### Variable Declaration

```
variable-declaration ::= ("let" | "fixed") identifier ("as" type)? ("=" expression)?
```

### Function Declaration

```
function-declaration ::= "define" identifier "using" parameter-list
                        ( "as" type )? block
parameter-list ::= parameter ( "and" parameter )*
parameter ::= identifier "as" type
```

### Object Declaration

```
object-declaration ::= "object" identifier ( "follows" identifier )? block
```

### Contract Declaration

```
contract-declaration ::= "contract" identifier block
```

### Choice Declaration

```
choice-declaration ::= "choice" identifier ( identifier )*
```

### Conditional Statement

```
conditional ::= "when" expression block
               ( "otherwise" "when" expression block )*
               ( "otherwise" block )?
```

### Loop Statements

```
repeat-loop ::= "repeat" identifier "from" expression "until" expression block
for-loop ::= "for" "each" identifier "inside" expression block
while-loop ::= "while" expression block
```

### Expression

```
expression ::= assignment
assignment ::= logical-or ( "=" assignment )?
logical-or ::= logical-and ( "or" logical-and )*
logical-and ::= equality ( "and" equality )*
equality ::= comparison ( ("==" | "!=") comparison )*
comparison ::= term ( ("<" | "<=" | ">" | ">=") term )*
term ::= factor ( ("+" | "-") factor )*
factor ::= unary ( ("*" | "/" | "%") unary )*
unary ::= ("not" | "-") unary | power
power ::= call ( "**" unary )?
call ::= primary ( "(" argument-list? ")" | "[" expression "]" | "." identifier )*
primary ::= literal | identifier | "(" expression ")" | list | map
```

### List

```
list ::= "[" expression ( "," expression )* "]"
```

### Map

```
map ::= "{" map-entry ( "," map-entry )* "}"
map-entry ::= identifier ":" expression
```

### Block

```
block ::= INDENT statement* DEDENT
```

### Statement

```
statement ::= variable-declaration
            | expression-statement
            | conditional
            | loop
            | "give" expression
            | "attempt" block "recover" identifier block
            | "raise" expression
            | "choose" expression ( "case" expression block )* ( "otherwise" block )?
```

## Operator Precedence

From highest to lowest:

1. `.` (member access), `[]` (index), `()` (call)
2. `**` (power)
3. `-` (unary minus), `not` (logical not)
4. `*`, `/`, `%`
5. `+`, `-`
6. `<`, `<=`, `>`, `>=`
7. `==`, `!=`
8. `and`
9. `or`
10. `=` (assignment)

## Indentation Rules

- Code blocks are defined by indentation
- Consistent indentation is required (spaces or tabs, but not mixed)
- Each level of indentation must be greater than the previous
- Dedenting closes the current block

## Type Annotations

### Basic Types

```
number
decimal
text
character
boolean
```

### Collection Types

```
list of text
map of text to number
set of number
```

### Optional Types

```
maybe text
maybe number
```

### Generic Types

```
Box of text
Result of number
```

## String Interpolation

String interpolation uses curly braces:

```
"Hello, {name}"
"Value: {value + 1}"
"Result: {calculate using x and y}"
```

## Escape Sequences

Supported escape sequences in strings:

```
\n  - Newline
\t  - Tab
\r  - Carriage return
\"  - Double quote
\'  - Single quote
\\  - Backslash
```

## Line Continuation

Long lines can be continued using indentation:

```
let result = some_function using
    argument1
    and argument2
```
