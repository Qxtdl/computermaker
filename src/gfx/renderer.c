#include <stdio.h>
#include <string.h>
#include <cglm/cglm.h>

#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "vbo.h"
#include "vao.h"

void renderer_init(struct renderer *renderer) {
    memset(renderer, 0, sizeof(*renderer));

    camera_init(&renderer->camera, (vec3){0, 3, 0}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, (vec3){0, 0, -1});
    camera_perspective_init(&renderer->camera, glm_rad(128.0f), 1, 0.5, 1000);

    renderer->vao = vao_create();
    renderer->vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    renderer->ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    // shaders
    renderer->shaders[RENDERER_SHADER_2D] = shader_load("res/shader/shader.vert", "res/shader/shader.frag");
    renderer->shaders[RENDERER_SHADER_3D] = shader_load("res/shader/3d.vert", "res/shader/3d.frag");
    // textures
    renderer->textures[RENDERER_TEXTURE_WALL] = texture_load("res/texture/wall.jpg");
    renderer->textures[RENDERER_TEXTURE_STUD] = texture_load("res/texture/stud.png");

}

void renderer_use_shader(struct renderer *renderer, enum RendererShaderType shader) {
    shader_use(renderer->shaders[shader]);
    renderer->current_shader = renderer->shaders[shader];
}

void renderer_use_texture(struct renderer *renderer, enum RendererTextureType texture) {
    texture_bind(renderer->textures[texture]);
}

void renderer_prepare(struct renderer *renderer, enum RendererPass pass) {
    switch (pass) {
        case RENDERER_PASS_2D:
        break;
        case RENDERER_PASS_3D:
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, renderer->wireframe ? GL_LINE : GL_FILL);
            // glEnable(GL_CULL_FACE);
            
            glm_perspective(
                renderer->camera.perspective.fovy,
                renderer->camera.perspective.aspect, 
                renderer->camera.perspective.nearZ,
                renderer->camera.perspective.farZ, 
                renderer->p
            );

            glm_lookat(
                renderer->camera.origin,
                renderer->camera.target,
                renderer->camera.up,
                renderer->v
            );
    }
}

void renderer_box(struct renderer *renderer, vao_t vao, vbo_t vbo, vbo_t ebo, vec3 translation, enum RendererTextureType texture) {
    renderer_use_shader(renderer, RENDERER_SHADER_3D);
    renderer_use_texture(renderer, RENDERER_TEXTURE_LAST);

    vao_attribute(vao, vbo, shader_attribute(renderer->current_shader, "aPos"), 3, GL_FLOAT, sizeof(float) * 5, (void*)0);
    vao_attribute(vao, vbo, shader_attribute(renderer->current_shader, "aTexCoord"), 2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3));

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, translation);

    sendUniformM4FV(renderer->current_shader, "model", model);
    sendUniformM4FV(renderer->current_shader, "view", renderer->v);
    sendUniformM4FV(renderer->current_shader, "projection", renderer->p);

    glDrawElements(GL_TRIANGLES, ebo.size, GL_UNSIGNED_INT, 0);
}
