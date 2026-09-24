# CodeTime Standard Library

## Overview

The CodeTime standard library provides comprehensive modules for common programming tasks. All modules are imported using the `use` keyword.

## System Module

### Functions

- `system exit(code as number)` - Exit the program with a status code
- `system args()` - Return command-line arguments as a list
- `system env(name as text)` - Get environment variable
- `system set_env(name as text, value as text)` - Set environment variable

```codetime
use system

let args = system args()
print "Program name: {args at 0}"
```

## Text Module

### Functions

- `text length(s as text)` - Return string length
- `text upper(s as text)` - Convert to uppercase
- `text lower(s as text)` - Convert to lowercase
- `text trim(s as text)` - Remove whitespace
- `text split(s as text, delimiter as text)` - Split string into list
- `text join(parts as list of text, delimiter as text)` - Join list into string
- `text contains(s as text, substring as text)` - Check if string contains substring
- `text replace(s as text, old as text, new as text)` - Replace occurrences
- `text substring(s as text, start as number, end as number)` - Extract substring

```codetime
use text

let message = "Hello, World!"
let length = text length message
print "Length: {length}"
```

## Math Module

### Functions

- `math abs(x as number)` - Absolute value
- `math min(a as number, b as number)` - Minimum of two numbers
- `math max(a as number, b as number)` - Maximum of two numbers
- `math sqrt(x as number)` - Square root
- `math pow(x as number, y as number)` - Power function
- `math sin(x as decimal)` - Sine
- `math cos(x as decimal)` - Cosine
- `math tan(x as decimal)` - Tangent
- `math floor(x as decimal)` - Floor
- `math ceil(x as decimal)` - Ceiling
- `math round(x as decimal)` - Round
- `math random()` - Random number between 0 and 1
- `math random_int(min as number, max as number)` - Random integer in range

```codetime
use math

let value = math sqrt 25
print "Square root of 25: {value}"
```

## Collections Module

### Functions

- `collections length(collection)` - Return collection size
- `collections is_empty(collection)` - Check if collection is empty
- `collections contains(collection, item)` - Check if item exists
- `collections first(list)` - Get first element
- `collections last(list)` - Get last element
- `collections reverse(list)` - Reverse list
- `collections sort(list)` - Sort list
- `collections map(list, function)` - Apply function to each element
- `collections filter(list, predicate)` - Filter elements
- `collections reduce(list, function, initial)` - Reduce list to single value

```codetime
use collections

let numbers = [3, 1, 4, 1, 5, 9]
let sorted = collections sort numbers
print "Sorted: {sorted}"
```

## Files Module

### Functions

- `files read(path as text)` - Read file contents as text
- `files write(path as text, content as text)` - Write content to file
- `files append(path as text, content as text)` - Append content to file
- `files exists(path as text)` - Check if file exists
- `files delete(path as text)` - Delete file
- `files copy(source as text, destination as text)` - Copy file
- `files move(source as text, destination as text)` - Move file
- `files list directory as text)` - List directory contents

```codetime
use files

let content = files read "data.txt"
print content
```

## Network Module

### Functions

- `network get(url as text)` - HTTP GET request
- `network post(url as text, data as text)` - HTTP POST request
- `network put(url as text, data as text)` - HTTP PUT request
- `network delete(url as text)` - HTTP DELETE request

```codetime
use network

let response = network get "https://api.example.com/data"
print response
```

## Time Module

### Functions

- `time now()` - Current timestamp
- `time format(timestamp, format as text)` - Format timestamp
- `time parse(string as text, format as text)` - Parse string to timestamp
- `time sleep(seconds as decimal)` - Sleep for specified seconds

```codetime
use time

let current = time now()
print "Current time: {current}"
```

## Random Module

### Functions

- `random number()` - Random number
- `random range(min as number, max as number)` - Random number in range
- `random choice(list)` - Random element from list
- `random shuffle(list)` - Shuffle list in place
- `random string(length as number)` - Random string

```codetime
use random

let value = random range 1 and 100
print "Random value: {value}"
```

## JSON Module

### Functions

- `json parse(string as text)` - Parse JSON string to object
- `json stringify(object)` - Convert object to JSON string
- `json from_file(path as text)` - Read and parse JSON file
- `json to_file(path as text, object)` - Write object to JSON file

```codetime
use json

let data = json parse '{"name": "Alice", "age": 25}'
print data name
```

## Process Module

### Functions

- `process execute(command as text)` - Execute shell command
- `process spawn(command as text)` - Spawn background process
- `process wait(pid as number)` - Wait for process to complete

```codetime
use process

let result = process execute "ls -la"
print result
```

## Usage Examples

### Reading a File

```codetime
use files

let content = files read "input.txt"
let lines = text split content using "\n"

for each line inside lines
    print line
```

### JSON Processing

```codetime
use json
use files

let json_content = files read "data.json"
let data = json parse json_content

print "Name: {data name}"
print "Age: {data age}"
```

### HTTP Request

```codetime
use network

let response = network get "https://api.example.com/users"
let users = json parse response

for each user inside users
    print user name
```

### File Operations

```codetime
use files

when not (files exists "output.txt")
    files write "output.txt" using "Hello, World!"

let content = files read "output.txt"
print content
```

## Module Conventions

1. All module functions use the format `module_name function_name`
2. Functions return `nothing` for void operations
3. Error handling should use `attempt/recover` blocks
4. File paths use forward slashes on all platforms
5. All text encoding is UTF-8

## Performance Considerations

- Large file operations should be done in chunks
- Network operations may timeout
- Use appropriate data structures for performance
- Consider memory usage for large collections
