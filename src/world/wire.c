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

wire_t *wires = NULL;
int wires_size = 0;

static float wire_thickness;

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

void set_wire_thickness(float thickness) {
    wire_thickness = thickness;
}

static void push_vertex(void *args, vertex_t vertex) {
    vertexes_size++;
    while (vertexes_count <= vertexes_size) {
        vertexes_count <<= 1;
        vertexes = srealloc(vertexes, vertexes_count * sizeof(vertex_t));
    }
    vertexes[vertexes_size - 1] = vertex;
}

static void push_index(void *args, unsigned int index) {
    indexes_size++;
    while (indexes_count <= indexes_size) {
        indexes_count <<= 1;
        indexes = srealloc(indexes, indexes_count * sizeof(unsigned int));
    }
    indexes[indexes_size - 1] = index;
}   

static void push_instance(mat4 model) {
    instances_size++;
    while (instances_count <= instances_size) {
        instances_count <<= 1;
        instances = srealloc(instances, instances_count * sizeof(mat4));
    }
    memcpy(&instances[instances_size - 1], model, sizeof(mat4));
}

static void get_model(wire_t wire, mat4 model) {
    vec3 translation = {
        wire.ox + wire_thickness * 0.5f,
        wire.oy + wire_thickness * 0.5f,
        wire.oz
    };
    float length = sqrt(
        (wire.dx - wire.ox) * (wire.dx - wire.ox) +
        (wire.dy - wire.oy) * (wire.dy - wire.oy) +
        (wire.dz - wire.oz) * (wire.dz - wire.oz)
    );
    vec3 direction = {
        (wire.dx - wire.ox) / length,
        (wire.dy - wire.oy) / length,
        (wire.dz - wire.oz) / length
    };    

    mat4 m;
    glm_mat4_identity(m);
    
    glm_translate(m, translation);
    glm_scale(m, (vec3){wire_thickness, wire_thickness, length});
	
    
    memcpy(model, m, sizeof(mat4));
}

void wires_bake(wire_t *wires) {
    free(vertexes);
    free(indexes);
    free(instances);
    vertexes = NULL;
    indexes = NULL;
    instances = NULL;
    vertexes_size = 0;
    vertexes_count = 1;
    indexes_size = 0;
    indexes_count = 1;
    instances_size = 0;
    instances_count = 1;

    blockmesh_push(
        push_vertex,
        push_index,
        NULL,
        NULL, 
        &vertexes_size,
        UV_NULL,
        UV_NULL,
        0,
        0,
        0
    );

    for (int i = 0; i < wires_size; i++) {
        wire_t wire = wires[i];
        if (!wire.valid) continue;
        
        mat4 model;
        get_model(wire, model);
        push_instance(model);
    }    

    vao_bind(vao);
    vbo_buffer(&vbo, vertexes, 0, vertexes_size * sizeof(vertex_t));
    vbo_buffer(&ebo, indexes, 0, indexes_size * sizeof(unsigned int));
    vbo_buffer(&ibo, instances, 0, instances_size * sizeof(mat4));
}

void world_create_wire(wire_t wire) {
    struct world_get_at_info info  = world_get_at(&state.world, wire.ox, wire.oy, wire.oz),
                             info2 = world_get_at(&state.world, wire.dx, wire.dy, wire.dz);
    block_t *block = &info.chunk->blocks[info.x][info.y][info.z],
            *block2 = &info2.chunk->blocks[info2.x][info2.y][info2.z];

    if (block->id == AIR || block2->id == AIR)
        return;

    for (int i = 0; i < wires_size; i++) {
        if (!wires[i].valid) {
            wires[i] = wire;
            wires[i].valid = true;
            goto gen;
        }
    }

    wires = srealloc(wires, ++wires_size * sizeof(wire_t));
    wire.valid = true;
    wires[wires_size - 1] = wire;
gen:
    logic_block_add_input(block, block2);

    wires_bake(wires);
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

    this->valid = false;

    wires_bake(wires);
}

void world_draw_wires(void) {
    if (wires_size == 0)
        return;
    renderer_instanced_mesh(&state.renderer, vao, vbo, ebo, ibo, RENDERER_TEXTURE_WIRE, instances_size);
}
