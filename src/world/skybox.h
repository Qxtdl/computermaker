#pragma once

#include "../gfx/shader.h"
#include "../gfx/texture.h"

enum CubeMapFace {
    CUBEMAP_FACE_TOP,
    CUBEMAP_FACE_BOTTOM,
    CUBEMAP_FACE_RIGHT,
    CUBEMAP_FACE_LEFT,
    CUBEMAP_FACE_FRONT,
    CUBEMAP_FACE_BACK,
};

typedef struct {
    shader_t shader;
    texture_t faces[6];
} skybox_t;

void skybox_init(skybox_t *skybox);
