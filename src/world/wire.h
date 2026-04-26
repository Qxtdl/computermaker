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