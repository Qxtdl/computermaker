#include "../state.h"
#include "../config.h"
#include "../world/wire.h"
#include "../world/save.h"
#include "../global.h"

struct raycast_info {
    float x,y,z;
    enum Face face;
};

struct raycast_info raycast(vec3 origin, vec3 direction, float distance) {
    float x = floor(origin[0]);
    float y = floor(origin[1]);
    float z = floor(origin[2]);
    float fractx=origin[0]-x;
    float fracty=origin[1]-y;
    float fractz=origin[2]-z;
    if (direction[0]==0&&direction[1]==0&&direction[2]==0) app_error("raycast direction is (0,0,0)\n");
    if (distance<=0) app_error("raycast distance is <= 0\n");
    glm_vec3_normalize(direction);
    float sx = direction[0]>0?1:(direction[0]<0?-1:0);
    float sy = direction[1]>0?1:(direction[1]<0?-1:0);
    float sz = direction[2]>0?1:(direction[2]<0?-1:0);
    float distX=direction[0]==0?INFINITY:1/direction[0]<0?-1/direction[0]:1/direction[0];
    float distY=direction[1]==0?INFINITY:1/direction[1]<0?-1/direction[1]:1/direction[1];
    float distZ=direction[2]==0?INFINITY:1/direction[2]<0?-1/direction[2]:1/direction[2];
    float dist2x=direction[0]<0?fractx*distX:(1-fractx)*distX;
    float dist2y=direction[1]<0?fracty*distY:(1-fracty)*distY;
    float dist2z=direction[2]<0?fractz*distZ:(1-fractz)*distZ;
    enum Face face;
    struct world_get_at_info info;
    while (dist2x<distance||dist2y<distance||dist2z<distance) {
        //check for collision
        info = world_get_at(&state.world, x, y, z);
        if (!(info.x < 0 || info.y < 0 || info.z < 0))
            if (info.chunk->blocks[info.x][info.y][info.z].id!=AIR) break;
        //go to next block
        if (dist2x<=dist2y&&dist2x<=dist2z) {x+=sx;dist2x+=distX;face=sx<0?FACE_RIGHT:FACE_LEFT;}
        else if (dist2y<=dist2z) {y+=sy;dist2y+=distY;face=sy<0?FACE_TOP:FACE_BOTTOM;}
        else {z+=sz;dist2z+=distZ;face=sz<0?FACE_FRONT:FACE_BACK;}
    }
    return (struct raycast_info){x,y,z,face};
}

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
                    vec3 direction;
                    glm_vec3_sub(state.renderer.camera.target,state.renderer.camera.origin,direction);
                    struct raycast_info raycast_info = raycast(state.renderer.camera.origin, direction, 10.0f);
                    if (state.player.selected_block!=AIR)
                    switch (raycast_info.face) {
                        case FACE_TOP:raycast_info.y++;break;
                        case FACE_BOTTOM:raycast_info.y--;break;
                        case FACE_RIGHT:raycast_info.x++;break;
                        case FACE_LEFT:raycast_info.x--;break;
                        case FACE_FRONT:raycast_info.z++;break;
                        case FACE_BACK:raycast_info.z--;break;
                    }
                    struct world_get_at_info info = world_get_at(&state.world, raycast_info.x,raycast_info.y,raycast_info.z);
                    if ((info.x < 0 || info.y < 0 || info.z < 0)) break;
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
    if (window.keyboard.keys[GLFW_KEY_Z].down) {
        save_save(config_get("SAVETO"));

        window.keyboard.keys[GLFW_KEY_Z].down = false;
    }
}
