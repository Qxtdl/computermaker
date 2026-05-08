#pragma once

#include "../gfx/renderer.h"
#include "block/block.h"
#include "../gfx/vertex.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

typedef struct 
__attribute__((packed)) 
/*
what if some compiler adds struct padding to the memory layout of the world and
one person saves it then the other who compiled it with a different compiler loads the save but its different
*/
{
    int x, y, z;
    block_t blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

    vao_t vao;
    vbo_t vbo, ebo, bbo;

    vertex_t *vertexes;
    unsigned int *indexes;
    ivec3 *blockinfo;

    size_t vertexes_size;
    size_t indexes_size;
    size_t blockinfo_size;

    size_t vertexes_count;
    size_t indexes_count;
    size_t blockinfo_count;
} chunk_t;

chunk_t chunk_gen(int x, int y, int z);
void chunk_bake(chunk_t *chunk);
void chunk_bake_at(chunk_t *chunk, int x, int y, int z);
void chunk_draw(chunk_t *chunk);

