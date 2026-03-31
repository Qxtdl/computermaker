#include <stdlib.h>
#include <string.h>
    
#include "state.h"
#include "gfx/camera.h"
#include "gfx/renderer.h"
#include "world/chunk.h"

void init(void) {
    renderer_init(&state.renderer);
    state.renderer.wireframe = 0;

    world_worldgen(&state.world);
}

void destroy(void) {
    exit(0);
}

void tick(void) {
    if (window.mouse.moved) {
        camera_mouse_cb(&state.renderer.camera, window.mouse.x, window.mouse.y);
        window.mouse.moved = true;
    }

    if (window.keyboard.keys[GLFW_KEY_W].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_FORWARD);
    }
    if (window.keyboard.keys[GLFW_KEY_A].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_LEFT);
    }
    if (window.keyboard.keys[GLFW_KEY_S].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_BACK);
    }
    if (window.keyboard.keys[GLFW_KEY_D].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_RIGHT);
    }

    if (window.keyboard.keys[GLFW_KEY_O].down) {
        state.renderer.wireframe = !state.renderer.wireframe;
        window.keyboard.keys[GLFW_KEY_O].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_R].down) {
        memset(&state.renderer.camera.origin, 0, sizeof(state.renderer.camera.origin));
    }
}

void render(void) {
    world_draw(&state.world);
    
    // for (int i = 0; i < state.world.chunks_size; i++) {
    //     for (int x = 0; x < CHUNK_X; x++) {
    //         for (int y = 0; y < CHUNK_Y; y++) {
    //             for (int z = 0; z < CHUNK_Z; z++) {
    //                 renderer_box(&state.renderer, (vec3){
    //                     state.world.chunks[i].x + x,
    //                     y,
    //                     state.world.chunks[i].z + z
    //                 }, state.world.chunks[i].blocks[x][y][z].texture);
    //             }
    //         }
    //     }
    // }

    // for (int i = 0; i < 50; i++) {
    //     for (int j = 0; j < 50; j++) {
    //         renderer_box(&state.renderer, (vec3){i, 0, j}, RENDERER_TEXTURE_STUD);
    //     }
    // }
}

int main() {    
    window_init();
    window_create("comm", 800, 600, init, destroy, tick, render);
    window_loop();
}
