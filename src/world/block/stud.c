#include "block.h"

void _stud_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv) {
    memcpy(scale, (vec2){0.0625, 0.0625}, sizeof(vec2));
    memcpy(uv, (vec2){0.5, 0.9375}, sizeof(vec2));
}
