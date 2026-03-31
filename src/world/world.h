#pragma once

#include "chunk.h"

struct world {
    chunk_t *chunks;
    size_t chunks_size;
};

void world_worldgen(struct world *world);
void world_add_chunk(struct world *world, chunk_t chunk);
void world_draw(struct world *world);