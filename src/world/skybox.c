#include <stb_image.h>
#include <cglm/cglm.h>

#include "skybox.h"
#include "../global.h"
#include "../config.h"

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void skybox_init(skybox_t *skybox) {
    skybox->shader = shader_load("res/shader/skybox.vert", "res/shader/skybox.frag");

    const char *faces[] = {
        config_get("SKY_LF"),
        config_get("SKY_RT"),
        config_get("SKY_DN"),
        config_get("SKY_UP"),
        config_get("SKY_FT"),
        config_get("SKY_BK")
    };    

    /* 
     * we won't use texture_load because it complicates the codebase
     * just make skybox.c directly interface with opengl 
    */
    glGenTextures(1, &skybox->handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->handle);

    int width, height, channels;
    for (int i = 0; i < 6; i++) {
        unsigned char *image = stbi_load(faces[i], &width, &height, &channels, 0);
        if (!image) {
            app_error("Could not load a face required for the skybox %s\n", faces[i]);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);    

    skybox->vao = vao_create();
    skybox->vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vbo_buffer(&skybox->vbo, &skyboxVertices, 0, sizeof(skyboxVertices));
}

void skybox_draw(skybox_t *skybox, struct renderer *renderer) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    glm_perspective(
        renderer->camera.perspective.fovy,
        renderer->camera.perspective.aspect, 
        0,
        renderer->camera.perspective.farZ, 
        renderer->p
    );

    shader_use(skybox->shader);
    
    vao_attribute(skybox->vao, skybox->vbo, shader_attribute(skybox->shader, "aPos"), 3, GL_FLOAT, sizeof(float) * 3, (void*)0);

    mat4 view;
    glm_mat4_copy(renderer->v, view);
    view[3][0] = view[3][1] = view[3][2] = 0;

    sendUniformM4FV(skybox->shader, "view", view);
    sendUniformM4FV(skybox->shader, "projection", renderer->p);    

    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->handle);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
