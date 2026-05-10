/*
 * wire.c
 *
 * place wires in the physical world
 * so players know what block is connected to what
*/

#include "wire.h"
#include "../util.h"
#include "../state.h"
#include "../config.h"
#include "../gfx/renderer.h"
#include "../gfx/vao.h"
#include "../gfx/vertex.h"
#include "block/uv.h"
#include "cglm/affine-pre.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/vec3.h"
#include "world.h"
#include "blockmesh.h"

wire_t *wires = NULL;
int wires_size = 0;

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
        ((float)wire.ox + (float)wire.dx)/2,
        ((float)wire.oy + (float)wire.dy)/2,
        ((float)wire.oz + (float)wire.dz)/2
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
    
    //goes bottom to top
    glm_translate(m, (vec3){translation[0]+0.5,translation[1]+0.5,translation[2]+0.5});
    if (direction[2] != 1) {
        mat4 rotation1;
        mat4 rotation0;
        vec3 direction0;
        vec3 rotation1_axis;
        vec3 rotation0_axis;
        
        glm_vec3_normalize_to((vec3){wire.dx-wire.ox, 0, wire.dz-wire.oz}, direction0);
        
        float angle1;
        angle1 = glm_vec3_angle((vec3){0, 1, 0}, direction);
        glm_vec3_cross((vec3){0, 1, 0}, direction, rotation1_axis);

        float angle0 = glm_vec3_angle(direction0, (vec3){0, 0, 1});

        glm_rotate_make(rotation1, angle1, rotation1_axis);
        glm_mat4_mul(m, rotation1, m);

        if (direction0[0] != 0 || direction0[2] != 0) {
            if (direction0[2] == -1) {
                rotation0_axis[0] = 0;
                rotation0_axis[1] = 1;
                rotation0_axis[2] = 0;
            } else {
                glm_vec3_cross((vec3){0, 0, 1}, direction0,  rotation0_axis);
            }
            glm_rotate_make(rotation0, angle0, rotation0_axis);
            glm_mat4_mul(m, rotation0, m);
        }
    }
    glm_scale(m, (vec3){atof(config_get("WIRE_THICKNESS")), length, atof(config_get("WIRE_THICKNESS"))});
    glm_translate(m, (vec3){-(0.5),-(0.5),-(0.5)});

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

    blockmesh_push(
        push_vertex,
        push_index,
        NULL,
        NULL, 
        &vertexes_size,
        UV_NULL,
        UV_NULL,
        (0), 
        (0),
        (0)
        // wire.ox,
        // wire.oy,
        // wire.oz
    );
    
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
