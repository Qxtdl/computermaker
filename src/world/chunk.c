#include "chunk.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "block.h"

chunk_t chunk_gen(int x, int z) {
    chunk_t chunk;
    chunk.x = x;
    chunk.z = z;
    for (int x = 0; x < CHUNK_X; x++) {
        for (int z = 0; z < CHUNK_Z; z++) {
            chunk.blocks[x][0][z] = (block_t){.type = BLOCK_TYPE_VISIBLE, .texture = RENDERER_TEXTURE_STUD};
        }
    }
    return chunk;
}

static bool has_neighbours(chunk_t *chunk, int x, int y, int z) {
    if (chunk->blocks[x + 1][y][z].type == BLOCK_TYPE_VISIBLE)
        return true;
    if (chunk->blocks[x - 1][y][z].type == BLOCK_TYPE_VISIBLE)
        return true;
    if (chunk->blocks[x][y + 1][z].type == BLOCK_TYPE_VISIBLE)
        return true;
    if (chunk->blocks[x][y - 1][z].type == BLOCK_TYPE_VISIBLE)
        return true;
    if (chunk->blocks[x][y][z + 1].type == BLOCK_TYPE_VISIBLE)
        return true;
    if (chunk->blocks[x][y][z - 1].type == BLOCK_TYPE_VISIBLE)
        return true;    
    return false;
}

void chunk_draw(chunk_t *chunk) {
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                if (has_neighbours(chunk, x, y, z)) {
                    continue;
                }

                renderer_box(&state.renderer, (vec3){
                    chunk->x + x,
                    y,
                    chunk->z + z
                }, chunk->blocks[x][y][z].texture);
            }
        }
    }
}