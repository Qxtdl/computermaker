#include "world.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "chunk.h"
#include "skybox.h"

void world_worldgen(struct world *world) {
    for (int x = 0; x < 5; x++) {
        for (int z = 0; z < 5; z++) {
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
    int index = 0;
    int cx = (int)floor(x) / CHUNK_X * CHUNK_X;
    int cz = (int)floor(z) / CHUNK_Z * CHUNK_Z;
    for (; index < world->chunks_size; index++) {
        if (world->chunks[index].x == cx && world->chunks[index].z == cz)
            break;
    }
    if (index == world->chunks_size) {
        index--;
        printf("Position out of bounds %f, %f, %f\n", x, y, z);
    }
    info.chunk = &world->chunks[index];
    info.x = (int)round(x) % CHUNK_X;
    info.y = (int)round(y) % CHUNK_Y;
    info.z = (int)round(z) % CHUNK_Z;
    return info;
}
