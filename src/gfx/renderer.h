#pragma once

#include <glad/glad.h>

#include "cglm/cglm.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "vao.h"
#include "vbo.h"

enum RendererShaderType {
    RENDERER_SHADER_2D,
    RENDERER_SHADER_3D,
    RENDERER_SHADER_LAST
};

enum RendererTextureType {
    RENDERER_TEXTURE_WALL,
    RENDERER_TEXTURE_STUD,
    RENDERER_TEXTURE_LAST,
};

struct renderer {
    shader_t shaders[RENDERER_SHADER_LAST], current_shader;
    texture_t textures[RENDERER_TEXTURE_LAST];
    camera_t camera;
    vao_t vao;
    vbo_t vbo;
    mat4 m, v, p;
    bool wireframe;
};

void renderer_init(struct renderer *renderer);
void renderer_use_shader(struct renderer *renderer, enum RendererShaderType shader);
void renderer_prepare(struct renderer *renderer);
void renderer_box(struct renderer *renderer, vec3 translation, enum RendererTextureType texture);
