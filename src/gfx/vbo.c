#include "vbo.h"

vbo_t vbo_create(GLuint type, GLuint attribute) {
    vbo_t vbo;
    vbo.attribute = attribute;
    vbo.type = type;
    glGenBuffers(1, &vbo.handle);
    return vbo;
}

void vbo_destroy(vbo_t vbo) {
    glDeleteBuffers(1, &vbo.handle);
}

void vbo_bind(vbo_t vbo) {
    glBindBuffer(vbo.type, vbo.handle);
}

void vbo_buffer(vbo_t vbo, void *data, size_t offset, size_t count) {
    vbo_bind(vbo);
    glBufferData(vbo.type, count - offset, data, vbo.attribute);
}
