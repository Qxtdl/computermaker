/*
 * wire.c
 *
 * place wires in the physical world
 * so players know what block is connected to what
*/

#include "wire.h"
#include "../util.h"
#include "../gfx/vao.h"

static wire_t *wires = NULL;
static size_t wires_size = 0;

static vao_t vao;
static vbo_t vbo, ebo;

void world_wire_init(void) {
    vao = vao_create();
    vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

void world_create_wire(wire_t wire) {
    wires = srealloc(wires, ++wires_size * sizeof(wire_t));
    wires[wires_size - 1] = wire;
}

void world_destroy_wire(wire_t wire) {

}
