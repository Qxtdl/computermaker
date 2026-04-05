#include "world.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "chunk.h"
#include "skybox.h"

void world_worldgen(struct world *world) {
    for (int x = 0; x < CHUNK_X; x++) {
        for (int z = 0; z < CHUNK_Z; z++) {
            world_add_chunk(world, chunk_gen(x * CHUNK_X, z * CHUNK_Z));
        }
    }
}

void world_add_chunk(struct world *world, chunk_t chunk) {
    world->chunks = srealloc(world->chunks, ++world->chunks_size * sizeof(chunk_t));
    world->chunks[world->chunks_size - 1] = chunk;
}

void world_draw(struct world *world) {
    renderer_prepare(&state.renderer, RENDERER_PASS_3D);
    for (size_t i = 0; i < world->chunks_size; i++) {
        chunk_draw(&world->chunks[i]);
    }
}

struct world_get_at_info world_get_at(struct world *world, float x, float y, float z) {
    struct world_get_at_info info;
    info.chunk = &world->chunks[0];
    info.x = round(x);
    info.y = round(y);
    info.z = round(z);
    printf("%d %d %d\n", info.x, info.y, info.z);

    return info;
}