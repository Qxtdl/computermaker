# Contribution

> [!NOTE] 
> This page may be updated often so please look at it before you make a PR.

We accept all contributions on the Project. \
There is a style guide to be followed.

# Contributing to computermaker

## Code Style Guide

**Format**:
1. A list of rules each might have an example (never a code block).
2. Sometimes an example for the whole (sub-)category (always a code block).

**Contents**:
1. [Naming Conventions](#naming-conventions)
    1. [Functions](#functions)
    2. [Macros](#macros)
2. [Formatting](#formatting)
    1. [Comments](#comments)
3. [Includes](#includes)
4. [Newlines](#newlines)
5. [File Organization](#file-organization)
6. [Memory Management](#memory-management)
7. [Headers](#headers)
8. [Final Example](#final-example)

### Naming Conventions

- **Functions**: `snake_case` (eg. `world_wire_init()`)
- **Variables**: `snake_case` (eg. `wires_size`)
- **Constants**: `SCREAMING_SNAKE_CASE` (eg. `MAX_VERTICES`)
- **Types/Structs**: `snake_case` (followed by a \_t) (eg. `wire_t`)
- **Macros**: `SCREAMING_SNAKE_CASE` (eg. `#define BUILDING_ADD_PIN(x, y, z, foo)`)

#### Functions

- Function without parameters should be declared as: `void fn(void) { /* ... */ }`.

#### Macros

- Macro parameters always wrapped in braces.

```c
#define EXAMPLE_MACRO(x) do { \
    printf("%d\n", (x)); \
} while (0)
```

### Formatting

- 4-space indentation.
- Opening braces on the same line: `if (condition) {`
- One space after keywords: `if (`, `while (`
- One space around binary operators: `x + y`
- No space after unary operators: `!a`
- You may discard a space around operators if there is NO space on the line for your code editor: `x+y`
- Pointers are on the right side: `char *ptr`
- Pointers in typecasts like so: `(uint32_t *)`, not `(uint32_t*)`
- No spaces between typecasts: `(char)value`, not `(char) value`
- Labels should not be indentation.

#### Comments

- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Comment complex algorithms and non-obvious logic
- Spaces in comments as in the example.

```c
/*
    Multi-line comment
 */
// A single space before the text
```

### Includes

- System includes should be at the top of the file (ie. those using `<>`).
- Local includes should be after the system includes (ie. those using `""`).

```c
#include <stdio.h>

#include "../state.h"
#include "../config.h"
// ...
```

### Newlines

- Double newline between two blocks of code if they are not related.

```c
if (condition) {
    // do something
}

if (related_condition) {
    // do something
}


if (unrelated_condition) {
    // do something
}
```

### File Organization

- Keep functions under 50 lines when possible.
- Group related functions together.
- Use header files (.h) for declarations, and source files (.c) for implementations.

### Memory Management

- Always check for NULL pointers after allocation.
    - Use the smalloc, srealloc, etc. functions provided by "src/util.h".
- Document ownership of allocated memory.
- Use consistent patterns for cleanup.

### Headers

- Use `#pragma` instead of header guards.
- Type definitions and short `#define`s at the beginning.
    - Function declarations after.

```c
#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    bool valid; // in wire.c datastructures
    int ox, oy, oz, // origin in the world
        dx, dy, dz; // destination in the world
} wire_t;

void world_wire_init(void);
void world_create_wire(wire_t wire);
void world_destroy_wire(wire_t wire);
void world_draw_wires(void);
```

### Final Example

```c
#include "wire.h"

typedef struct {
    int ox, oy, oz,
        dx, dy, dz;
} wire_t;

void render_wire(wire_t *wire) {
    if (!wire) {
        return;
    }
    // the implementation
}
```

