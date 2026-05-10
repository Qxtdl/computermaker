#include <glad/glad.h>
#include <math.h>

#define GLT_IMPLEMENTATION
#include <gltext.h>

#include "renderer.h"
#include "../config.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "vbo.h"
#include "vao.h"
#include "../gfx/raycast.h"

void renderer_init(struct renderer *renderer, float aspect) {
    memset(renderer, 0, sizeof(*renderer));

    camera_init(&renderer->camera, (vec3){0, 3, 0}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, (vec3){0, 0, -1});
    camera_perspective_init(&renderer->camera, glm_rad(128.0f), aspect, 0.1, 1000);
    gltInit();

    renderer->vao = vao_create();
    renderer->vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    renderer->ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    // shaders
    renderer->shaders[RENDERER_SHADER_2D] = shader_load("res/shader/shader.vert", "res/shader/shader.frag");
    renderer->shaders[RENDERER_SHADER_CHUNK] = shader_load("res/shader/chunk.vert", "res/shader/chunk.frag");
    renderer->shaders[RENDERER_SHADER_INSTANCED_3D] = shader_load("res/shader/instanced_3d.vert", "res/shader/instanced_3d.frag");
    // textures
    renderer->textures[RENDERER_TEXTURE_BLOCKATLAS] = texture_load(config_get("BLOCK_ATLAS"));
    renderer->textures[RENDERER_TEXTURE_WIRE] = texture_load("res/texture/wire.png");

    renderer->camera.render_distance = atoi(config_get("RENDER_DISTANCE"));
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
            glClear(GL_DEPTH_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            glPolygonMode(GL_FRONT_AND_BACK, renderer->wireframe ? GL_LINE : GL_FILL);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glFrontFace(GL_CCW);  

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

void renderer_chunk(struct renderer *renderer, vao_t vao, vbo_t vbo, vbo_t ebo, vbo_t bbo, vec3 translation, enum RendererTextureType texture) {
    vec3 distance;

    glm_vec3_sub(renderer->camera.origin, translation, distance);
    glm_vec3_abs(distance, distance);

    for (int i = 0; i < 3; i++)
        if (distance[i] > renderer->camera.render_distance)
            return;

    renderer_use_shader(renderer, RENDERER_SHADER_CHUNK);
    renderer_use_texture(renderer, texture);

    vao_attribute(vao, vbo, shader_attribute(renderer->current_shader, "aPos"), 3, GL_FLOAT, sizeof(float) * 5, (void*)0);
    vao_attribute(vao, vbo, shader_attribute(renderer->current_shader, "aTexCoord"), 2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    vao_attribute(vao, bbo, shader_attribute(renderer->current_shader, "blockinfo"), 3, GL_INT, sizeof(int) * 3, (void*)0);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, translation);

    vec3 raycast_direction;
    glm_vec3_sub(renderer->camera.target,renderer->camera.origin,raycast_direction);

    struct raycast_info raycast_info = raycast(renderer->camera.origin, raycast_direction);

    ivec3 target = {
        floor(raycast_info.x),
        floor(raycast_info.y),
        floor(raycast_info.z)
    };

    sendUniformM4FV(renderer->current_shader, "model", model);
    sendUniformM4FV(renderer->current_shader, "view", renderer->v);
    sendUniformM4FV(renderer->current_shader, "projection", renderer->p);
    sendUniformI3V(renderer->current_shader, "target", target);

    glDrawElements(GL_TRIANGLES, ebo.size, GL_UNSIGNED_INT, 0);
}

void renderer_instanced_mesh(struct renderer *renderer, vao_t vao, vbo_t vbo, vbo_t ebo, vbo_t ibo, enum RendererTextureType texture, int instancecount) {
    renderer_use_shader(renderer, RENDERER_SHADER_INSTANCED_3D);
    renderer_use_texture(renderer, texture);

    vao_attribute(vao, vbo, shader_attribute(renderer->current_shader, "aPos"), 3, GL_FLOAT, sizeof(float) * 5, (void*)0);
    vao_attribute(vao, vbo, shader_attribute(renderer->current_shader, "aTexCoord"), 2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    for (int i = 0; i < 4; i++) {
        vao_attribute_divisor(vao, ibo, shader_attribute(renderer->current_shader, "model")+i, 4, GL_FLOAT, sizeof(mat4), (void*)(sizeof(vec4) * i), 1);
    }

    sendUniformM4FV(renderer->current_shader, "view", renderer->v);
    sendUniformM4FV(renderer->current_shader, "projection", renderer->p);

    glDrawElementsInstanced(GL_TRIANGLES, ebo.size, GL_UNSIGNED_INT, 0, instancecount);
}

void renderer_text(float x, float y, float scale, const char *text, vec3 color) {
    GLboolean cull = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    GLTtext *ptr = gltCreateText();
    gltSetText(ptr, text);

    gltBeginDraw();

    if (!color)
        gltColor(1, 1, 1, 1);
    else
        gltColor(color[0], color[1], color[2], 1);

    gltDrawText2D(ptr, x, y, scale);

    gltEndDraw();

    gltDestroyText(ptr);

    if (cull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}
