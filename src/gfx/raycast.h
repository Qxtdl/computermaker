#pragma once

#include "../world/blockmesh.h"

struct raycast_info {
    float x,y,z;
    enum Face face;
};

void raycast_set_distance(float raycast_distance);
struct raycast_info raycast(vec3 origin, vec3 direction);
