#pragma once

#include "../gfx/shader.h"
#include "../gfx/texture.h"
#include "../gfx/vao.h"
#include "../gfx/renderer.h"

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
    GLuint handle;
    vao_t vao;
    vbo_t vbo;
} skybox_t;

void skybox_init(skybox_t *skybox);
void skybox_draw(skybox_t *skybox, struct renderer *renderer);

