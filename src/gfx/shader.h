#pragma once

#include <stddef.h>
#include <glad/glad.h>

typedef struct {
    GLuint handle;
} shader_t;

shader_t shader_load(const char *vshader_path, const char *fshader_path);
GLint shader_attribute(shader_t shader, const char *name);
void sendUniformM4FV(shader_t shader, const char *uniform, void *data);
void sendUniform4FV(shader_t shader, const char *uniform, void *data);
void shader_use(shader_t shader);
