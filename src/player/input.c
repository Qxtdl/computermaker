#include <math.h>
#include <stddef.h>

#include "../state.h"
#include "../gfx/window.h"
#include "../config.h"
#include "../world/wire.h"
#include "../world/building/building.h"
#include "../world/save.h"
#include "../gfx/raycast.h"
#include "chat.h"
#include "keybinds.h"

#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)>(y)?(y):(x))

static bool mouse_free = true;
static bool fullscreen = false;

static inline bool get_key(int key) {
    return window.keyboard.keys[key].down;
}

static inline void set_key(int key, bool state) {
    window.keyboard.keys[key].down = state;
}

static inline bool get_mouse_button(int button) {
    return window.mouse.buttons[button].down;
}

static inline void set_mouse_button(int button, bool state) {
    window.mouse.buttons[button].down = state;
}

static void input_chat_handle(void) {
    if (get_key(GLFW_KEY_ENTER)) {
        chat_add_message("player", chat_input);
        if (chat_input[0] == '!') chat_handle_command(chat_input);

        chat_input[0] = '\0';
        chat_input_len = 0;
        chat_active = false;

        set_key(GLFW_KEY_ENTER, false);
    }

    if (get_key(GLFW_KEY_BACKSPACE)) {
        if (chat_input_len > 0) {
            chat_input[--chat_input_len] = '\0';
        }
        set_key(GLFW_KEY_BACKSPACE, false);
    }
}

void input_handle(void) {
    if (window.mouse.moved && !mouse_free) {
        camera_mouse_cb(&state.renderer.camera, window.mouse.x, window.mouse.y);
        window.mouse.moved = false;
    }

    if (chat_active) {
        input_chat_handle();
        return;
    }

    if (get_mouse_button(GLFW_MOUSE_BUTTON_LEFT) && mouse_free) {
        glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        set_mouse_button(GLFW_MOUSE_BUTTON_LEFT, false);
        mouse_free = false;
    }

    if (get_key(GLFW_KEY_LEFT_SHIFT))
        state.renderer.camera.move_speed = 0.35f;
    else
        state.renderer.camera.move_speed = 0.1f;

    if (get_key(GLFW_KEY_W)) camera_move(&state.renderer.camera, CAMERA_DIRECTION_FORWARD);
    if (get_key(GLFW_KEY_A)) camera_move(&state.renderer.camera, CAMERA_DIRECTION_LEFT);
    if (get_key(GLFW_KEY_S)) camera_move(&state.renderer.camera, CAMERA_DIRECTION_BACK);
    if (get_key(GLFW_KEY_D)) camera_move(&state.renderer.camera, CAMERA_DIRECTION_RIGHT);

    if (window.mouse.scrolled) {
        state.renderer.camera.perspective.fovy -= glm_rad(window.mouse.scroll.y);
        window.mouse.scrolled = false;

        if (state.renderer.camera.perspective.fovy > M_PI - 0.1f) state.renderer.camera.perspective.fovy = M_PI - 0.1f;
        if (state.renderer.camera.perspective.fovy < 0.1f) state.renderer.camera.perspective.fovy = 0.1f;
    }

    if (get_key(GLFW_KEY_ESCAPE)) {
        glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        set_key(GLFW_KEY_ESCAPE, false);
        mouse_free = true;
    }


    if (get_mouse_button(GLFW_MOUSE_BUTTON_LEFT)) {
        set_mouse_button(GLFW_MOUSE_BUTTON_LEFT, false);

        vec3 ray_dir;
        glm_vec3_sub(state.renderer.camera.target,state.renderer.camera.origin,ray_dir);

        struct raycast_info ray_info = raycast(state.renderer.camera.origin, ray_dir);
        struct world_get_at_info info = world_get_at(&state.world, ray_info.x, ray_info.y, ray_info.z);

        struct world_get_at_relative_info relative_info = world_get_at_relative(info);
        state.player.hovered_block = NULL;

        switch (state.player.mode) {
            case MODE_BLOCK_PLACE: {
                if (state.player.selected_block != AIR && info.chunk->blocks[info.x][info.y][info.z].id != AIR) {
                    switch (ray_info.face) {
                        case FACE_TOP: ray_info.y++; break;
                        case FACE_BOTTOM: ray_info.y--; break;
                        case FACE_RIGHT: ray_info.x++; break;
                        case FACE_LEFT: ray_info.x--; break;
                        case FACE_FRONT: ray_info.z++; break;
                        case FACE_BACK: ray_info.z--; break;
                        default: break;
                    }
                }
                info = world_get_at(&state.world, ray_info.x, ray_info.y,ray_info.z);
                info.chunk->blocks[info.x][info.y][info.z].id = state.player.selected_block;
                chunk_bake(info.chunk);
                break;
            }

            case MODE_WIRE_PLACE:
            case MODE_WIRE_DESTROY: {
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
                } else if (state.player.mode == MODE_WIRE_DESTROY) {
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
            }

            case MODE_WIRE_PARALLEL_PLACE:
            case MODE_WIRE_PARALLEL_DESTROY: {
                state.player.points[state.player.point][0] = relative_info.x;
                state.player.points[state.player.point][1] = relative_info.y;
                state.player.points[state.player.point][2] = relative_info.z;

                state.player.point++;

                if (state.player.point >= LAST_POINT) {
                    int x_diff0 = abs(state.player.points[FIRST_POINT][0] - state.player.points[SECOND_POINT][0])+1,
                        y_diff0 = abs(state.player.points[FIRST_POINT][1] - state.player.points[SECOND_POINT][1])+1,
                        z_diff0 = abs(state.player.points[FIRST_POINT][2] - state.player.points[SECOND_POINT][2])+1;
                    int x_diff1 = abs(state.player.points[THIRD_POINT][0] - state.player.points[FOURTH_POINT][0])+1,
                        y_diff1 = abs(state.player.points[THIRD_POINT][1] - state.player.points[FOURTH_POINT][1])+1,
                        z_diff1 = abs(state.player.points[THIRD_POINT][2] - state.player.points[FOURTH_POINT][2])+1;
                        
                    if (x_diff0!=y_diff0 && x_diff0>1 && y_diff0>1) {
                        state.player.point = 0;
                        break;
                    }
                    if (x_diff0!=z_diff0 && x_diff0>1 && z_diff0>1) {
                        state.player.point = 0;
                        break;
                    }
                    if (y_diff0!=z_diff0 && y_diff0>1 && z_diff0>1) {
                        state.player.point = 0;
                        break;
                    }
                    if (x_diff1!=y_diff1 && x_diff1>1 && y_diff1>1) {
                        state.player.point = 0;
                        break;
                    }
                    if (x_diff1!=z_diff1 && x_diff1>1 && z_diff1>1) {
                        state.player.point = 0;
                        break;
                    }
                    if (y_diff1!=z_diff1 && y_diff1>1 && z_diff1>1) {
                        state.player.point = 0;
                        break;
                    }
                    int length = min(max(max(x_diff0,y_diff0),z_diff0),max(max(x_diff1,y_diff1),z_diff1));
                    int dx0 = 0,
                        dy0 = 0,
                        dz0 = 0,
                        dx1 = 0,
                        dy1 = 0,
                        dz1 = 0;
                    if ((state.player.points[SECOND_POINT][0] - state.player.points[FIRST_POINT][0]) < 0) dx0 = -1;
                    if ((state.player.points[SECOND_POINT][0] - state.player.points[FIRST_POINT][0]) > 0) dx0 =  1;
                    if ((state.player.points[SECOND_POINT][1] - state.player.points[FIRST_POINT][1]) < 0) dy0 = -1;
                    if ((state.player.points[SECOND_POINT][1] - state.player.points[FIRST_POINT][1]) > 0) dy0 =  1;
                    if ((state.player.points[SECOND_POINT][2] - state.player.points[FIRST_POINT][2]) < 0) dz0 = -1;
                    if ((state.player.points[SECOND_POINT][2] - state.player.points[FIRST_POINT][2]) > 0) dz0 =  1;
                    if ((state.player.points[FOURTH_POINT][0] - state.player.points[THIRD_POINT][0]) < 0) dx1 = -1;
                    if ((state.player.points[FOURTH_POINT][0] - state.player.points[THIRD_POINT][0]) > 0) dx1 =  1;
                    if ((state.player.points[FOURTH_POINT][1] - state.player.points[THIRD_POINT][1]) < 0) dy1 = -1;
                    if ((state.player.points[FOURTH_POINT][1] - state.player.points[THIRD_POINT][1]) > 0) dy1 =  1;
                    if ((state.player.points[FOURTH_POINT][2] - state.player.points[THIRD_POINT][2]) < 0) dz1 = -1;
                    if ((state.player.points[FOURTH_POINT][2] - state.player.points[THIRD_POINT][2]) > 0) dz1 =  1;
                    int x0 = state.player.points[FIRST_POINT][0],
                        y0 = state.player.points[FIRST_POINT][1],
                        z0 = state.player.points[FIRST_POINT][2],
                        x1 = state.player.points[THIRD_POINT][0],
                        y1 = state.player.points[THIRD_POINT][1],
                        z1 = state.player.points[THIRD_POINT][2];
                    for (size_t i = 0; i < length; i++) {
                        if (state.player.mode == MODE_WIRE_PARALLEL_PLACE) {
                            world_create_wire((wire_t){
                                .ox = x0,
                                .oy = y0,
                                .oz = z0,
                                .dx = x1,
                                .dy = y1,
                                .dz = z1
                            });
                        } else {
                            world_destroy_wire((wire_t){
                                .ox = x0,
                                .oy = y0,
                                .oz = z0,
                                .dx = x1,
                                .dy = y1,
                                .dz = z1
                            });
                        }
                        x0 += dx0;
                        y0 += dy0;
                        z0 += dz0;
                        x1 += dx1;
                        y1 += dy1;
                        z1 += dz1;
                    }
                    state.player.point = 0;
                }

                break;
            }

            case MODE_BLOCK_HOVER: {
                state.player.hovered_block = &info.chunk->blocks[info.x][info.y][info.z];
                break;
            }

            case MODE_BUILDING_PLACE: {
                if (info.chunk->blocks[info.x][info.y][info.z].id != AIR) {
                    switch (ray_info.face) {
                        case FACE_TOP: ray_info.y++; break;
                        case FACE_BOTTOM: ray_info.y--; break;
                        case FACE_RIGHT: ray_info.x++; break;
                        case FACE_LEFT: ray_info.x--; break;
                        case FACE_FRONT: ray_info.z++; break;
                        case FACE_BACK: ray_info.z--; break;
                        default: break;
                    }
                }
                int camera_rotation;
                vec3 camera_direction;
                glm_vec3_sub(state.renderer.camera.target, state.renderer.camera.origin, camera_direction);
                if (fabsf(camera_direction[2]) > fabsf(camera_direction[0])) {
                    if (camera_direction[2] > 0) {
                        camera_rotation = CAMERA_DIRECTION_FORWARD;
                    } else {
                        camera_rotation = CAMERA_DIRECTION_BACK;
                    }
                } else {
                    if (camera_direction[0] > 0) {
                        camera_rotation = CAMERA_DIRECTION_LEFT;
                    } else {
                        camera_rotation = CAMERA_DIRECTION_RIGHT;
                    }
                }
                building_create((building_t){
                    .id = MEMORY,
                    .x = ray_info.x,
                    .y = ray_info.y,
                    .z = ray_info.z,
                    .rotation = camera_rotation,
                    .bitwidth = 16,
                    .addresswidth = 16
                });
                
                break;
            }

            case MODE_BLOCK_POKE: {
                info.chunk->blocks[info.x][info.y][info.z].gate.poked = true;
                info.chunk->blocks[info.x][info.y][info.z].gate.new_state ^= 1;
                break;
            }
        
            default: break;                
        }
    }

    if (get_key(GLFW_KEY_E)) {
        set_key(GLFW_KEY_E, false);
        state.player.mode++;
        if (state.player.mode == MODE_LAST) state.player.mode = 0;
    }

    if (get_key(GLFW_KEY_Q)) {
        set_key(GLFW_KEY_Q, false);
        if (++state.player.selected_block == BLOCKID_LAST) state.player.selected_block = 0;
    }

    if (get_key(GLFW_KEY_O)) {
        set_key(GLFW_KEY_O, false);
        state.renderer.wireframe = !state.renderer.wireframe;
    }

    if (get_key(GLFW_KEY_R)) {
        set_key(GLFW_KEY_R, false);
        memset(&state.renderer.camera.origin, 0, sizeof(state.renderer.camera.origin));
    }

    if (get_key(GLFW_KEY_Z)) {
        set_key(GLFW_KEY_Z, false);
        save_save(config_get("SAVETO"));
        chat_add_message("comm", "Save written");
    }

    if (get_key(GLFW_KEY_T)) {
        chat_active = true;
        set_key(GLFW_KEY_T, false);
    }
    
    if (get_key(GLFW_KEY_F11)) {
        set_key(GLFW_KEY_F11, false);
        fullscreen = !fullscreen;

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (fullscreen) {
            glfwSetWindowMonitor(window.handle, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(window.handle, NULL, 100, 100, 800, 600, 0);
        }
    }

    for (int i = 0; i < BLOCK_KEYBINDS_COUNT; i++) {
        block_keybind_t *block_kb = &block_keybinds[i];
        if (!get_key(block_kb->key)) continue;
        state.player.selected_block = block_kb->blockid;
        set_key(block_kb->key, false);
    }
}
