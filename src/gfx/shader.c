#include <glad/glad.h>

#include "shader.h"
#include "../global.h"
#include "../util.h"

static GLuint compile(const char *filename, GLenum type) {
    char *source = readfile(filename);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar *const *)&source, NULL);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        app_error("Failed to compile shader: %s\n", log)
    }
    free(source);
    return shader;
}

shader_t shader_load(const char *vshader_path, const char *fshader_path) {
    GLuint vshader = compile(vshader_path, GL_VERTEX_SHADER);
    GLuint fshader = compile(fshader_path, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    glAttachShader(program, vshader);
    glAttachShader(program, fshader);

    glLinkProgram(program);
    shader_t shader;
    shader.handle = program;
    return shader;
}

GLint shader_attribute(shader_t shader, const char *name) {
    return glGetAttribLocation(shader.handle, name);
}

void sendUniformM4FV(shader_t shader, const char *uniform, void *data) {
    glUniformMatrix4fv(glGetUniformLocation(shader.handle, uniform), 1, GL_FALSE, data);
}

void sendUniform4FV(shader_t shader, const char *uniform, void *data) {
    glUniform4fv(glGetUniformLocation(shader.handle, uniform), 1, data);
}

void shader_use(shader_t shader) {
    glUseProgram(shader.handle);
}
