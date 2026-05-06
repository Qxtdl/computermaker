#include "raycast.h"
#include "../state.h"
#include "../global.h"
#include <math.h>

static float distance;

void raycast_set_distance(float raycast_distance) {
    distance = raycast_distance;
}

struct raycast_info raycast(vec3 origin, vec3 direction) {
    float x = floor(origin[0]);
    float y = floor(origin[1]);
    float z = floor(origin[2]);
    float fractx = origin[0] - x;
    float fracty = origin[1] - y;
    float fractz = origin[2] - z;

    if (direction[0] == 0 && direction[1] == 0 && direction[2] == 0) 
        app_error("Raycast direction is zero\n");
    if (distance<=0) 
        app_error("Raycast distance is <= 0\n");

    glm_vec3_normalize(direction);

    float sx = direction[0] > 0 ? 1 : (direction[0] < 0 ? - 1 : 0);
    float sy = direction[1] > 0 ? 1 : (direction[1] < 0 ? - 1 : 0);
    float sz = direction[2] > 0 ? 1 : (direction[2] < 0 ? - 1 : 0);

    float distX = direction[0] == 0 ? INFINITY : (1 / direction[0] < 0 ? -1 / direction[0]: 1 / direction[0]);
    float distY = direction[1] == 0 ? INFINITY : (1 / direction[1] < 0 ? -1 / direction[1]: 1 / direction[1]);
    float distZ = direction[2] == 0 ? INFINITY : (1 / direction[2] < 0 ? -1 / direction[2]: 1 / direction[2]);

    float dist2x = direction[0] < 0 ? fractx * distX: (1 - fractx) * distX;
    float dist2y = direction[1] < 0 ? fracty * distY: (1 - fracty) * distY;
    float dist2z = direction[2] < 0 ? fractz * distZ: (1 - fractz) * distZ;

    enum Face face;
    struct world_get_at_info info;
    while (dist2x < distance || dist2y < distance || dist2z < distance) {
        //check for collision
        info = world_get_at(&state.world, x, y, z);
        if (!(info.x < 0 || info.y < 0 || info.z < 0))
            if (info.chunk->blocks[info.x][info.y][info.z].id != AIR) break;
        //go to next block
        if (dist2x <= dist2y && dist2x <= dist2z) {
            x += sx;
            dist2x += distX;
            face = sx < 0 ? FACE_RIGHT: FACE_LEFT;
        }
        else if (dist2y <= dist2z) {
            y += sy;
            dist2y += distY;
            face = sy <0 ? FACE_TOP : FACE_BOTTOM;
        }
        else {
            z += sz;
            dist2z += distZ;
            face = sz < 0 ? FACE_FRONT : FACE_BACK;
        }
    }

    return (struct raycast_info){x,y,z,face};
}
