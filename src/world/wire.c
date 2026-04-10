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
#include "block/block.h"

static wire_t *wires = NULL;
static size_t wires_size = 0;

static vao_t vao;
static vbo_t vbo, ebo;
vertex_t *vertices = NULL;
unsigned int *indices = NULL;
static size_t vertices_size = 0, vertices_count = 1;
static size_t indices_size = 0, indices_count = 1;

void world_wire_init(void) {
    vao = vao_create();
    vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

static void push_vertex(vertex_t vertex) {
    vertices_size++;
    while (vertices_count <= vertices_size) {
        vertices_count <<= 1;
        vertices = srealloc(vertices, vertices_count * sizeof(vertex_t));
    }
    vertices[vertices_size - 1] = vertex;
}

static void push_indice(unsigned int indice) {
    indices_size++;
    while (indices_size >= indices_count) {
        indices_count <<= 1;
        indices = srealloc(indices, indices_count * sizeof(unsigned int));
    }
    indices[indices_size - 1] = indice;
}

static void get_face(enum Face face, wire_t wire, vertex_t *mesh) {
    switch (face) {
        case FACE_FRONT: 
            mesh[0] = (vertex_t){{wire.ox,   wire.oy,   wire.oz+1}, {0, 0}};
            mesh[1] = (vertex_t){{wire.ox,   wire.oy+1, wire.oz+1}, {1, 0}};
            mesh[2] = (vertex_t){{wire.ox+1, wire.oy+1, wire.oz+1}, {1, 1}};
            mesh[3] = (vertex_t){{wire.ox+1, wire.oy,   wire.oz+1}, {0, 1}};   
            break;
        case FACE_BACK:
            mesh[0] = (vertex_t){{wire.ox,   wire.oy,   wire.oz}, {0, 0}};
            mesh[1] = (vertex_t){{wire.ox+1, wire.oy,   wire.oz}, {1, 0}};
            mesh[2] = (vertex_t){{wire.ox+1, wire.oy+1, wire.oz}, {1, 1}};
            mesh[3] = (vertex_t){{wire.ox,   wire.oy+1, wire.oz}, {0, 1}};
            break;
        case FACE_RIGHT:
            mesh[0] = (vertex_t){{wire.ox+1, wire.oy,   wire.oz},   {0, 0}};
            mesh[1] = (vertex_t){{wire.ox+1, wire.oy,   wire.oz+1}, {1, 0}};
            mesh[2] = (vertex_t){{wire.ox+1, wire.oy+1, wire.oz+1}, {1, 1}};
            mesh[3] = (vertex_t){{wire.ox+1, wire.oy+1, wire.oz},   {0, 1}};
            break;
        case FACE_LEFT:
            mesh[0] = (vertex_t){{wire.ox, wire.oy,   wire.oz+1}, {0, 0}};
            mesh[1] = (vertex_t){{wire.ox, wire.oy,   wire.oz},   {1, 0}};
            mesh[2] = (vertex_t){{wire.ox, wire.oy+1, wire.oz},   {1, 1}};
            mesh[3] = (vertex_t){{wire.ox, wire.oy+1, wire.oz+1}, {0, 1}};
            break;
        case FACE_TOP:
            mesh[0] = (vertex_t){{wire.ox,   wire.oy+1, wire.oz},   {0, 0}};
            mesh[1] = (vertex_t){{wire.ox+1, wire.oy+1, wire.oz},   {1, 0}};
            mesh[2] = (vertex_t){{wire.ox+1, wire.oy+1, wire.oz+1}, {1, 1}};
            mesh[3] = (vertex_t){{wire.ox,   wire.oy+1, wire.oz+1}, {0, 1}};
            break;
        case FACE_BOTTOM:
            mesh[0] = (vertex_t){{wire.ox,   wire.oy, wire.oz+1}, {0, 0}};
            mesh[1] = (vertex_t){{wire.ox+1, wire.oy, wire.oz+1}, {1, 0}};
            mesh[2] = (vertex_t){{wire.ox+1, wire.oy, wire.oz},   {1, 1}};
            mesh[3] = (vertex_t){{wire.ox,   wire.oy, wire.oz},   {0, 1}};            
    }
}

void world_create_wire(wire_t wire) {
    // free(vertices);
    // free(indices);
    // vertices = NULL;
    // indices = NULL;
    // indices_count = 1;
    // vertices_count = 1;
    // vao_destroy(vao);
    // vbo_destroy(vbo);
    // vbo_destroy(ebo);

    wires = srealloc(wires, ++wires_size * sizeof(wire_t));
    wires[wires_size - 1] = wire;
    
    for (enum Face face = 0; face < FACE_LAST; face++) {
        vertex_t vertex[4];
        get_face(face, wire, vertex);

        for (int i = 0; i < 4; i++)
            push_vertex(vertex[i]);

        const size_t start = vertices_size - 4;
        push_indice(start + 0);
        push_indice(start + 1);
        push_indice(start + 2);
        push_indice(start + 2);
        push_indice(start + 3);
        push_indice(start + 0);
    }

    vao_bind(vao);
    vbo_buffer(&vbo, vertices, 0, vertices_size * sizeof(vertex_t));
    vbo_buffer(&ebo, indices, 0, indices_size * sizeof(unsigned int));
}

void world_destroy_wire(wire_t wire) {

}

void world_draw_wires(void) {
    renderer_prepare(&state.renderer, RENDERER_PASS_3D);
    renderer_mesh(&state.renderer, vao, vbo, ebo, (vec3){0, 0, 0}, RENDERER_TEXTURE_WIRE);
}
