#pragma once

#include <cglm/cglm.h>
#include "../world/blockmesh.h"

struct raycast_info {
    float x, y, z;
    enum Face face;
};

struct raycast_info raycast(vec3 origin, vec3 direction, float distance);
