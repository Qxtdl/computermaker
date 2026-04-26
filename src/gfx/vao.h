#pragma once

#include <glad/glad.h>

#include "vbo.h"

typedef struct {
    GLuint handle;
} vao_t;

vao_t vao_create(void);
void vao_destroy(vao_t vao);
void vao_bind(vao_t vao);
void vao_attribute(vao_t vao, vbo_t vbo, GLuint index, GLint size, GLenum type, GLsizei stride, void *offset);
void vao_attribute_divisor(vao_t vao, vbo_t vbo, GLuint index, GLint size, GLenum type, GLsizei stride, void *offset, unsigned int divisor);
