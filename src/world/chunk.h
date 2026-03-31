#pragma once

#include "block.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

// #define CHUNK_X 5
// #define CHUNK_Y 5
// #define CHUNK_Z 5

typedef struct {
    int x, z;
    block_t blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
} chunk_t;

chunk_t chunk_gen(int x, int z);
void chunk_draw(chunk_t *chunk);