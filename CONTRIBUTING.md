# Contribution

We accept everybody to contribute on the Project. \
There is a style guide to be followed:

# Contributing to computermaker

## Code Style Guide

### Includes
- System includes should be at the top of the file. (e.g. those with < >)
- Local includes should be after the system includes (e.g. those with " ")
### Example
```c
#include <math.h>

#include "../state.h"
#include "../config.h"
#include "../world/wire.h"
#include "../world/save.h"
#include "../gfx/raycast.h"
```

### Naming Conventions
- **Functions**: `snake_case` (e.g., `world_wire_init()`)
- **Variables**: `snake_case` (e.g., `wires_size`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_VERTICES`)
- **Types/Structs**: `snake_case` (followed by a _t) (e.g., `wire_t`)

### Functions
- If your function has no parameters `void func()`, turn it into `void func(void)`

### Formatting
- Use 4 spaces for indentation (no tabs)
- Opening braces on the same line: `if (condition) {`
- One space after keywords: `if (`, `while (`
- One space after operators aswell. `x + y`
- You may discard a space between operators if you feel like so. Example: `x+y`
- Pointers are on the right side `char *ptr`

### Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Comment complex algorithms and non-obvious logic
- Spaces between comments
```diff
// example for spaces between comments
-//bad
+// good
```

### File Organization
- Keep functions under 50 lines when possible
- Group related functions together
- Use header files (.h) for declarations, source files (.c) for implementations

### Memory Management
- Always check for NULL pointers after allocation
- Document ownership of allocated memory
- Use consistent patterns for cleanup
- Use the smalloc, srealloc, ..etc functions provided by util.c (header is util.h).

### Headers
- Put `#pragma once`, no macro header guards (like `#ifndef HEADER_FILE_H`)
- Definitions and typedefs at the top of the file
- Function prototypes come after

### Example

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

### Another example of everything summarized
```c
// Good example
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
