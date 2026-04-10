#include "world.h"
#include "../global.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "chunk.h"
#include "wire.h"

void world_worldgen(struct world *world) {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
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
    world_draw_wires();
    for (size_t i = 0; i < world->chunks_size; i++) {
        chunk_draw(&world->chunks[i]);
    }
}

struct world_get_at_info world_get_at(struct world *world, float x, float y, float z) {
    // TODO: bug
    // if you try to get somewhere in negative x, y or z
    // it segfaults

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

void world_place_at(struct world *world, int x, int y, int z, block_t block) {
    int cx = x / CHUNK_X * CHUNK_X;
    int cz = z / CHUNK_Z * CHUNK_Z;
    int i;
    for (i = 0; i < world->chunks_size; i++) {
        if (world->chunks[i].x == cx && world->chunks[i].z == cz)
            break;
    }
    if (i == world->chunks_size) {
        app_error("Position out of bounds: %d, %d, %d.\n", x, y, z);
    }    
    world->chunks[i].blocks[x][y][z] = block;
}
