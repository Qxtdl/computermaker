#pragma once

#include <cglm/cglm.h>
#include "block.h"

#define UV_NULL (vec2){1, 1}

vec2 *uv_get_scale(block_t block);
vec2 *uv_get_position(block_t block);

