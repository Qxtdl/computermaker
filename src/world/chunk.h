#pragma once

#include "block/block.h"
#include "../gfx/renderer.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

typedef struct {
    vec3 vertices;
    vec2 texture;
} vertex_t;

typedef struct {
    int x, z;
    block_t blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

    vao_t vao;
    vbo_t vbo, ebo;

    vertex_t *vertices;
    unsigned int *indices;
    size_t vertices_size;
    size_t indices_size;
} chunk_t;

chunk_t chunk_gen(int x, int z);
void chunk_bake(chunk_t *chunk);
void chunk_draw(chunk_t *chunk);
