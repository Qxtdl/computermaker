// TODO: use window.h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "chunk.h"
#include "world.h"
#include "chunk.h"
#include "building/building.h"

double last_tick_time = 0;
double tick_interval = 1.0 / 10.0;

void world_tick(struct world *world) {
    // TODO: use window.h
    double now = glfwGetTime();

    if (now - last_tick_time < tick_interval) {
        return;
    }
    last_tick_time += tick_interval;

    for (int i = 0; i < world->chunks_size; i++) {
        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    if (is_logic_block(world->chunks[i]->blocks[x][y][z]) &&
                        !world->chunks[i]->blocks[x][y][z].gate.poked
                    ) {
                        block_tick(&world->chunks[i]->blocks[x][y][z]);
                    }
                }
            }
        }
    }
    // tick buildings
	buildings_tick();
    
    for (int i = 0; i < world->chunks_size; i++) {
        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    if (is_logic_block(world->chunks[i]->blocks[x][y][z])) {
                        world->chunks[i]->blocks[x][y][z].gate.poked = false;
                        if (world->chunks[i]->blocks[x][y][z].gate.state != world->chunks[i]->blocks[x][y][z].gate.new_state) {
                            world->chunks[i]->blocks[x][y][z].gate.state = world->chunks[i]->blocks[x][y][z].gate.new_state;
                            chunk_bake_at(world->chunks[i], x, y, z);
                        }
                    }
                }
            }
        }
    }    
}
