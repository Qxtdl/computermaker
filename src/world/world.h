#pragma once

#include "chunk.h"
#include "skybox.h"

struct world {
    chunk_t *chunks;
    size_t chunks_size;
    skybox_t skybox;
};

void world_worldgen(struct world *world);
void world_add_chunk(struct world *world, chunk_t chunk);
void world_draw(struct world *world);
struct world_get_at_info {
    chunk_t *chunk;
    int x, y, z; // relative to chunk
};
struct world_get_at_info world_get_at(struct world *world, float x, float y, float z);
void world_place_at(struct world *world, int x, int y, int z, block_t block);
