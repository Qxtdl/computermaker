#include <math.h>
#include "../state.h"
#include "../config.h"
#include "../world/wire.h"
#include "../world/save.h"
#include "../gfx/raycast.h"

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

        if (state.renderer.camera.perspective.fovy > M_PI-0.1) {
            state.renderer.camera.perspective.fovy = M_PI-0.1;
        }
        if (state.renderer.camera.perspective.fovy < 0.1) {
            state.renderer.camera.perspective.fovy = 0.1;
        }
    }

    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down) {
        vec3 raycast_direction;
        glm_vec3_sub(state.renderer.camera.target,state.renderer.camera.origin,raycast_direction);
        struct raycast_info raycast_info = raycast(state.renderer.camera.origin, raycast_direction);
        struct world_get_at_info info = world_get_at(
            &state.world, 
            raycast_info.x,
            raycast_info.y,
            raycast_info.z
        );
        struct world_get_at_relative_info relative_info = world_get_at_relative(info);
        switch (state.player.mode) {
            case MODE_BLOCK_PLACE:
                if (state.player.selected_block!=AIR)
                switch (raycast_info.face) {
                    case FACE_TOP: raycast_info.y++; break;
                    case FACE_BOTTOM: raycast_info.y--; break;
                    case FACE_RIGHT: raycast_info.x++; break;
                    case FACE_LEFT: raycast_info.x--; break;
                    case FACE_FRONT: raycast_info.z++; break;
                    case FACE_BACK: raycast_info.z--; break;
                }
                info = world_get_at(&state.world, raycast_info.x,raycast_info.y,raycast_info.z);
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
