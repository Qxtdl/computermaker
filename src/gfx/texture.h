#pragma once

#include <glad/glad.h>

typedef struct {
    GLuint handle;
} texture_t;

typedef void (*TextureSetup)(void);

texture_t texture_load(const char *filename, TextureSetup setup);
void texture_destroy(texture_t texture);
void texture_bind(texture_t texture);
