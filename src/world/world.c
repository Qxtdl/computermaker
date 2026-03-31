#include "world.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "chunk.h"

void world_worldgen(struct world *world) {
    for (int x = 0; x < 1; x++) {
        for (int z = 0; z < 2; z++) {
            world_add_chunk(world, chunk_gen(x * CHUNK_X, z * CHUNK_Z));
        }
    }
}

void world_add_chunk(struct world *world, chunk_t chunk) {
    world->chunks = srealloc(world->chunks, ++world->chunks_size * sizeof(chunk_t));
    world->chunks[world->chunks_size - 1] = chunk;
}

void world_draw(struct world *world) {
    renderer_prepare(&state.renderer);
    for (size_t i = 0; i < world->chunks_size; i++) {
        chunk_draw(&world->chunks[i]);
    }
}