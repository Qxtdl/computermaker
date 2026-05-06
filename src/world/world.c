#include "world.h"
#include "../global.h"
#include "../util.h"
#include "../state.h"
#include "../config.h"
#include "../gfx/renderer.h"
#include "chunk.h"
#include "wire.h"

void world_worldgen(struct world *world) {
    int ax = atoi(config_get("CHUNK_AMOUNT_X")),
        az = atoi(config_get("CHUNK_AMOUNT_Z"));
    for (int x = 0; x < ax; x++) {
        for (int z = 0; z < az; z++) {
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
    world_draw_wires();
}

struct world_get_at_info world_get_at(struct world *world, float x, float y, float z) {
    struct world_get_at_info info;
    int index = 0;
    int cx = (int)floor(x) / CHUNK_X * CHUNK_X;
    int cz = (int)floor(z) / CHUNK_Z * CHUNK_Z;
    if (floor(x)<0) cx -= 16;
    if (floor(z)<0) cz -= 16;
    for (; index < world->chunks_size; index++) {
        if (world->chunks[index].x == cx && world->chunks[index].z == cz)
            break;
    }
    if (index == world->chunks_size) {
        index--;
    }
    info.chunk = &world->chunks[index];
    info.x = (int)round(x) % CHUNK_X;
    info.y = (int)round(y) % CHUNK_Y;
    info.z = (int)round(z) % CHUNK_Z;
    if (info.x < 0) info.x += 16;
    if (info.y < 0) info.y += 16;
    if (info.z < 0) info.z += 16;
    return info;
}

struct world_get_at_relative_info world_get_at_relative(struct world_get_at_info theirinfo) {
    struct world_get_at_relative_info info;
    info.x = theirinfo.chunk->x + theirinfo.x;
    info.y = theirinfo.y;
    info.z = theirinfo.chunk->z + theirinfo.z;
    return info;
}

void world_place_at(struct world *world, int x, int y, int z, block_t block) {
	struct world_get_at_info info = world_get_at(world, x, y, z);
	info.chunk->blocks[info.x][info.y][info.z] = block;
}
