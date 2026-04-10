#pragma once

#include "block/block.h"
#include "../gfx/renderer.h"
#include "../gfx/vertex.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

typedef struct {
    int x, z;
    block_t blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

    vao_t vao;
    vbo_t vbo, ebo;

    vertex_t *vertexes;
    unsigned int *indexes;
    size_t vertexes_size;
    size_t indexes_size;
    size_t vertexes_count;
    size_t indexes_count;
} chunk_t;

chunk_t chunk_gen(int x, int z);
void chunk_bake(chunk_t *chunk);
void chunk_bake_at(chunk_t *chunk, int x, int y, int z);
void chunk_draw(chunk_t *chunk);

