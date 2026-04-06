#pragma once

#include <string.h>
#include <cglm/cglm.h>

#define BLOCK_UV_DECLARE(name) extern void _##name##_get_uv(enum Face face, vec2 *scale, vec2 *uv);
#define BLOCK_UV_GET(name, face, scale, uv) _##name##_get_uv(face, scale, uv);

enum Face {
    FACE_TOP,
    FACE_BOTTOM,
    FACE_RIGHT,
    FACE_LEFT,
    FACE_FRONT,
    FACE_BACK
};

typedef struct {
    enum BlockId {
        AIR,
        STUD,
        BRICK,
        BLOCKID_LAST
    } id;
} block_t;

void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv);

BLOCK_UV_DECLARE(air)
BLOCK_UV_DECLARE(stud)
BLOCK_UV_DECLARE(brick)
