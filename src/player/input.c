#include "../state.h"
#include "../config.h"
#include "../world/wire.h"
#include "../world/save.h"

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
        if (!(info.x < 0 || info.y < 0 || info.z < 0)) { // NOTE: dont segfault
            struct world_get_at_relative_info relative_info = world_get_at_relative(info);
            switch (state.player.mode) {
                case MODE_BLOCK_PLACE:
                    info.chunk->blocks[info.x][info.y][info.z] = (block_t){.id = state.player.selected_block, .gate.state = STATE_OFF};
                    chunk_bake(info.chunk);
                    break;
                case MODE_WIRE_PLACE:
                case MODE_WIRE_DESTROY:
                    if (!state.player.planout) {
                        state.player.wire_ox = relative_info.x;
                        state.player.wire_oy = relative_info.y;
                        state.player.wire_oz = relative_info.z;
                        state.player.planout = true;                    
                        break;
                    } else if (state.player.mode == MODE_WIRE_PLACE) {
                        world_create_wire((wire_t){
                            .ox = state.player.wire_ox,
                            .oy = state.player.wire_oy,
                            .oz = state.player.wire_oz,
                            .dx = relative_info.x,
                            .dy = relative_info.y,
                            .dz = relative_info.z
                        });
                    } else {
                        world_destroy_wire((wire_t){
                            .ox = state.player.wire_ox,
                            .oy = state.player.wire_oy,
                            .oz = state.player.wire_oz,
                            .dx = relative_info.x,
                            .dy = relative_info.y,
                            .dz = relative_info.z
                        });
                    }
                    state.player.planout = false;
                    break;
                case MODE_BLOCK_POKE: info.chunk->blocks[info.x][info.y][info.z].gate.new_state ^= 1; break;
            }
        }
        
        window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_E].down) {
        state.player.mode++;
        if (state.player.mode == MODE_LAST)
            state.player.mode = 0;

        window.keyboard.keys[GLFW_KEY_E].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_Q].down) {
        if (++state.player.selected_block == BLOCKID_LAST) 
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
    if (window.keyboard.keys[GLFW_KEY_Z].down) {
        save_save(config_get("SAVETO"));

        window.keyboard.keys[GLFW_KEY_Z].down = false;
    }
}
