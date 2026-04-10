#include <stdbool.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"
#include "../global.h"

texture_t texture_load(const char *filename) {
    texture_t texture;

    GLuint handle;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);
    if (!image) {
        app_error("Failed to load a texture: %s\n", filename);
    }

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    printf("Made a handle %d for %s\n", handle, filename);
    texture.handle = handle;
    return texture;
}

void texture_destroy(texture_t texture) {
    glDeleteTextures(1, &texture.handle);
}

void texture_bind(texture_t texture) {
    static int i = 0;
    if (!i) {
        i++;
        printf("Using handle %d\n", texture.handle);
    }
    glBindTexture(GL_TEXTURE_2D, texture.handle);
}
