#pragma once

#include <stddef.h>
#include <glad/glad.h>

typedef struct {
    GLuint handle;
    GLuint type;
    GLuint attribute;
    size_t size;
} vbo_t;

vbo_t vbo_create(GLuint type, GLuint attribute);
void vbo_destroy(vbo_t vbo);
void vbo_bind(vbo_t vbo);
void vbo_buffer(vbo_t *vbo, void *data, size_t offset, size_t count);
