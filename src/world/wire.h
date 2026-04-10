#pragma once

typedef struct {
    int ox, oy, oz, // origin in the world
        dx, dy, dz; // destination in the world
} wire_t;

void create_wire(wire_t wire);
void destroy_wire(wire_t wire);

