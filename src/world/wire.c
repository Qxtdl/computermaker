/*
 * wire.c
 *
 * place wires in the physical world
 * so players know what block is connected to what
*/

#include "wire.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "../gfx/vao.h"
#include "../gfx/vertex.h"
#include "world.h"

static wire_t *wires = NULL;
static size_t wires_size = 0;

static vao_t vao;
static vbo_t vbo;
static vertex_t *vertexes = NULL;
static size_t vertexes_size = 0, vertexes_count = 1;

void world_wire_init(void) {
    vao = vao_create();
    vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
}

static void push_vertex(vertex_t vertex) {
    vertexes_size++;
    while (vertexes_count <= vertexes_size) {
        vertexes_count <<= 1;
        vertexes = srealloc(vertexes, vertexes_count * sizeof(vertex_t));
    }
    vertexes[vertexes_size - 1] = vertex;
}

void world_create_wire(wire_t wire) {
    wires = srealloc(wires, ++wires_size * sizeof(wire_t));
    wires[wires_size - 1] = wire;

    struct world_get_at_info info  = world_get_at(&state.world, wire.ox, wire.oy, wire.oz),
                             info2 = world_get_at(&state.world, wire.dx, wire.dy, wire.dz);
    logic_block_add_input(&info.chunk->blocks[info.x][info.y][info.z], &info2.chunk->blocks[info2.x][info2.y][info2.z]);

    // free(vertexes);
    // free(indices);
    // vertexes = NULL;
    // indices = NULL;
    // indices_count = 1;
    // vertexes_count = 1;
    // vao_destroy(vao);
    // vbo_destroy(vbo);
    // vbo_destroy(ebo);

    push_vertex((vertex_t){{wire.dx+0.5, wire.dy+0.5, wire.dz+0.25}, {0, 0}});
    push_vertex((vertex_t){{wire.ox+0.5, wire.oy+0.5, wire.oz+0.5}, {1, 0}});
    push_vertex((vertex_t){{wire.dx+0.5, wire.dy+0.5, wire.dz+0.5}, {1, 1}});
    vao_bind(vao);
    vbo_buffer(&vbo, vertexes, 0, vertexes_size * sizeof(vertex_t));
}

void world_destroy_wire(wire_t wire) {

}

void world_draw_wires(void) {
    if (wires_size == 0)
        return;
    glDisable(GL_CULL_FACE);
    renderer_wire(&state.renderer, vao, vbo, RENDERER_TEXTURE_WIRE);
}
