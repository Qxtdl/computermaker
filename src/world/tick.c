#include "chunk.h"
#include "world.h"

void world_tick(struct world *world) {
    static int tickspeed = 0;
    if (tickspeed++ == 1)
        tickspeed = 0;
    else return;
    for (int i = 0; i < world->chunks_size; i++) {
        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    if (is_logic_block(world->chunks[i].blocks[x][y][z])) {
                        block_tick(&world->chunks[i].blocks[x][y][z]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < world->chunks_size; i++) {
        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    if (is_logic_block(world->chunks[i].blocks[x][y][z])) {
                        if (world->chunks[i].blocks[x][y][z].gate.state != world->chunks[i].blocks[x][y][z].gate.new_state) {
                            world->chunks[i].blocks[x][y][z].gate.state = world->chunks[i].blocks[x][y][z].gate.new_state;
                            chunk_bake_at(&world->chunks[i], x, y, z);
                        }
                    }
                }
            }
        }
    }    
}
