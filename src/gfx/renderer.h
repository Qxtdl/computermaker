#pragma once

#include <glad/glad.h>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "vao.h"
#include "vbo.h"

enum RendererPass {
    RENDERER_PASS_2D,
    RENDERER_PASS_3D
};

enum RendererShaderType {
    RENDERER_SHADER_2D,
    RENDERER_SHADER_3D,
    RENDERER_SHADER_LAST
};

enum RendererTextureType {
    RENDERER_TEXTURE_BLOCKATLAS = 1,
    RENDERER_TEXTURE_WIRE,
    RENDERER_TEXTURE_LAST,
};

struct renderer {
    shader_t shaders[RENDERER_SHADER_LAST], current_shader;
    texture_t textures[RENDERER_TEXTURE_LAST];
    camera_t camera;

    vao_t vao;
    vbo_t vbo, ebo;

    mat4 m, v, p;

    bool wireframe;
};

void renderer_init(struct renderer *renderer);
void renderer_use_shader(struct renderer *renderer, enum RendererShaderType shader);
void renderer_prepare(struct renderer *renderer, enum RendererPass pass);
void renderer_mesh(struct renderer *renderer, vao_t vao, vbo_t vbo, vbo_t ebo, vec3 translation, enum RendererTextureType texture);
void renderer_wire(struct renderer *renderer, vao_t vao, vbo_t vbo, enum RendererTextureType texture);
void renderer_text(float x, float y, float scale, const char *text);

