#include "vao.h"

vao_t vao_create(void) {
    vao_t vao;
    glGenVertexArrays(1, &vao.handle);
    return vao;
}

void vao_destroy(vao_t vao) {
    glDeleteVertexArrays(1, &vao.handle);
}

void vao_bind(vao_t vao) {
    glBindVertexArray(vao.handle);
}

void vao_attribute(vao_t vao, vbo_t vbo, GLuint index, GLint size, GLenum type, GLsizei stride, void *offset) {
    vao_bind(vao);
    vbo_bind(vbo);
    switch (type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            glVertexAttribIPointer(index, size, type, stride, offset);
            break;
        default:
            glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
    }
    glEnableVertexAttribArray(index);
}

