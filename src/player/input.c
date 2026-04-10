#include "../state.h"

void input_handle(void) {
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

    if (window.mouse.scrolled) {
        state.renderer.camera.perspective.fovy += glm_rad(window.mouse.scroll.y);
        window.mouse.scrolled = false;
    }

    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down) {
        struct world_get_at_info info = world_get_at(
            &state.world, 
            state.renderer.camera.target[0],
            state.renderer.camera.target[1],
            state.renderer.camera.target[2]
        );
        if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down)
            info.chunk->blocks[info.x][info.y][info.z] = (block_t){.id = state.player.selected_block, .gate.state = STATE_OFF};

        chunk_bake(info.chunk);

        window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_Q].down) {
        if (state.player.selected_block++ == BLOCKID_LAST) 
            state.player.selected_block = 0;

        window.keyboard.keys[GLFW_KEY_Q].down = false;
    }    

    if (window.keyboard.keys[GLFW_KEY_O].down) {
        state.renderer.wireframe = !state.renderer.wireframe;
        
        window.keyboard.keys[GLFW_KEY_O].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_R].down) {
        memset(&state.renderer.camera.origin, 0, sizeof(state.renderer.camera.origin));
    }
}
