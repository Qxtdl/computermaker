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
#include "block/uv.h"
#include "world.h"
#include "blockmesh.h"

static wire_t *wires = NULL;
static size_t wires_size = 0;

static vao_t vao;
static vbo_t vbo, ebo, ibo;
static vertex_t *vertexes = NULL;
static size_t vertexes_size = 0, vertexes_count = 1;
static unsigned int *indexes = NULL;
static size_t indexes_size = 0, indexes_count = 1;
static mat4 *instances = NULL;
static size_t instances_size = 0, instances_count = 1;

void world_wire_init(void) {
    vao = vao_create();
    vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    ibo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
}

static void push_vertex(void *args, vertex_t vertex) {
    vertexes_size++;
    while (vertexes_count <= vertexes_size) {
        vertexes_count <<= 1;
        vertexes = srealloc(vertexes, vertexes_count * sizeof(vertex_t));
    }
    vertexes[vertexes_size - 1] = vertex;
}

static void remove_vertex(size_t index, size_t n) {
    if (index >= vertexes_size) return;
    if (index + n > vertexes_size)
        n = vertexes_size - index;

    memmove(&vertexes[index],
            &vertexes[index + n],
            (vertexes_size - index - n) * sizeof(vertex_t));

    vertexes_size -= n;
}

static void push_index(void *args, unsigned int index) {
    indexes_size++;
    while (indexes_count <= indexes_size) {
        indexes_count <<= 1;
        indexes = srealloc(indexes, indexes_count * sizeof(unsigned int));
    }
    indexes[indexes_size - 1] = index;
}

static void remove_index(size_t index, size_t n) {
    if (index >= indexes_size) return;
    if (index + n > indexes_size)
        n = indexes_size - index;

    memmove(&indexes[index],
            &indexes[index + n],
            (indexes_size - index - n) * sizeof(unsigned int));

    indexes_size -= n;
}

static void push_instance(mat4 model) {
    instances_size++;
    while (instances_count <= instances_size) {
        instances_count <<= 1;
        instances = srealloc(instances, instances_count * sizeof(mat4));
    }
    memcpy(instances[instances_size - 1], model, sizeof(*model));
}

static inline void midpoint(float x, float y, float z, float x2, float y2, float z2, float *px, float *py, float *pz) {
    *px = (x + x2)/2;
    *py = (y + y2)/2;
    *pz = (z + z2)/2;
}

static inline void get_model(wire_t wire, mat4 model) {
    mat4 _model;
    glm_mat4_identity(_model);
    glm_translate(_model, (vec3){0, 5, 0});
    memcpy(model, _model, sizeof(_model));
}

void world_create_wire(wire_t wire) {
    wires = srealloc(wires, ++wires_size * sizeof(wire_t));
    wire.vertex_index = vertexes_size;
    wire.indice_index = indexes_size;
    wires[wires_size - 1] = wire;

    struct world_get_at_info info  = world_get_at(&state.world, wire.ox, wire.oy, wire.oz),
                             info2 = world_get_at(&state.world, wire.dx, wire.dy, wire.dz);
    logic_block_add_input(&info.chunk->blocks[info.x][info.y][info.z], &info2.chunk->blocks[info2.x][info2.y][info2.z]);

    float x, y, z;
    midpoint(wire.ox, wire.oy, wire.oz, wire.dx, wire.dy, wire.dz, &x, &y, &z);

    blockmesh_push(
        push_vertex,
        push_index,
        NULL,
        NULL, 
        &vertexes_size,
        UV_NULL,
        UV_NULL,
        x, 
        y,
        z
    );

    // mat4 model;
    // get_model(wire, model);
    // push_instance(model);


    vao_bind(vao);
    vbo_buffer(&vbo, vertexes, 0, vertexes_size * sizeof(vertex_t));
    vbo_buffer(&ebo, indexes, 0, indexes_size * sizeof(unsigned int));
}

static wire_t *get_wire(wire_t wire) {
    for (int i = 0; i < wires_size; i++) {
        if (wires[i].ox == wire.ox &&
            wires[i].oy == wire.oy &&
            wires[i].oz == wire.oz &&
            wires[i].dx == wire.dx &&
            wires[i].dy == wire.dy &&
            wires[i].dz == wire.dz         
        ) return &wires[i];
    }
    return NULL;
}

void world_destroy_wire(wire_t wire) {
    if (wires_size <= 0) return;

    wire_t *this = get_wire(wire);
    if (!this) return;

    struct world_get_at_info info  = world_get_at(&state.world, wire.ox, wire.oy, wire.oz),
                             info2 = world_get_at(&state.world, wire.dx, wire.dy, wire.dz);
    logic_block_remove_input(&info.chunk->blocks[info.x][info.y][info.z], &info2.chunk->blocks[info2.x][info2.y][info2.z]);

    blockmesh_remove(remove_vertex, remove_index, this->vertex_index, this->indice_index);

    vao_bind(vao);
    vbo_buffer(&vbo, vertexes, 0, vertexes_size * sizeof(vertex_t));
    vbo_buffer(&ebo, indexes, 0, indexes_size * sizeof(unsigned int));
}

void world_draw_wires(void) {
    if (wires_size == 0)
        return;
    renderer_mesh(&state.renderer, vao, vbo, ebo, (vec3){0, 0, 0}, RENDERER_TEXTURE_WIRE);
}
