#include <stdlib.h>
#include <string.h>
    
#include "cglm/cglm.h"
#include "gfx/camera.h"
#include "state.h"
#include "gfx/renderer.h"
#include "gfx/texture.h"

void init(void) {
    renderer_init(&state.renderer);
    state.renderer.wireframe = 0;
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
        //glm_vec3_add(state.renderer.camera.origin, (vec3){1.0, 0, 0.0f}, state.renderer.camera.origin);
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_FORWARD);
    }
    if (window.keyboard.keys[GLFW_KEY_A].down) {
        //glm_vec3_add(state.renderer.camera.origin, (vec3){-1.0f, 0.0f, 0.0f}, state.renderer.camera.origin);
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_LEFT);
    }
    if (window.keyboard.keys[GLFW_KEY_S].down) {
        //glm_vec3_add(state.renderer.camera.origin, (vec3){0.0f, 0.0f, -1.0f}, state.renderer.camera.origin);
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_BACK);
    }
    if (window.keyboard.keys[GLFW_KEY_D].down) {
        //glm_vec3_add(state.renderer.camera.origin, (vec3){0.0f, 0.0f, 1.0f}, state.renderer.camera.origin);
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
    renderer_prepare(&state.renderer);
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            renderer_box(&state.renderer, (vec3){i, 0, j}, RENDERER_TEXTURE_STUD);
        }
    }
}

int main() {    
    window_init();
    window_create("comm", 800, 600, init, destroy, tick, render);
    window_loop();
}
