#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include "window.h"
#include "../global.h"
#include "../state.h"
#include "../player/chat.h"
#include "../config.h"

struct window window;

void window_init(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void framebuffer_size_callback(GLFWwindow* gwindow, int width, int height) {
    state.renderer.camera.perspective.aspect=(float)width/(float)height;
    window.width = width;
    window.height = height;
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* gwindow, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS: window.keyboard.keys[key].down = true; break;
        case GLFW_RELEASE: window.keyboard.keys[key].down = false;
    }
}

static void char_callback(GLFWwindow* gwindow, unsigned int codepoint) {
    chat_char_callback(codepoint);
}

static void mouse_button_callback(GLFWwindow *gwindow, int button, int action, int mods) {
    if (button < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS: window.mouse.buttons[button].down = true; break;
        case GLFW_RELEASE: window.mouse.buttons[button].down = false;
    }
}

static void cursor_pos_callback(GLFWwindow *gwindow, double x, double y) {
    window.mouse.x = x;
    window.mouse.y = y;
    window.mouse.moved = true;
}

static void scroll_callback(GLFWwindow *gwindow, double x, double y) {
    window.mouse.scroll.x = x;
    window.mouse.scroll.y = y;
    window.mouse.scrolled = true;
}

void window_create(
    const char *title, int width, int height, int target_fps,
    WindowCallback init,
    WindowCallback destroy,
    WindowCallback tick,
    WindowCallback render     
) {
    GLFWwindow *gwindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!gwindow) {
        app_error("Failed to create window\n");
    }
    glfwMakeContextCurrent(gwindow);
    window.handle = gwindow;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        app_error("Failed to load OpenGL\n");
    }

    glfwSetFramebufferSizeCallback(gwindow, framebuffer_size_callback);
    glfwSetKeyCallback(gwindow, key_callback);
    glfwSetCharCallback(gwindow, char_callback);
    glfwSetMouseButtonCallback(gwindow, mouse_button_callback);
    glfwSetCursorPosCallback(gwindow, cursor_pos_callback);
    glfwSetScrollCallback(gwindow, scroll_callback);

    window.init = init;
    window.destroy = destroy;
    window.tick = tick;
    window.render = render;

    window.width = width;
    window.height = height;
    window.target_fps = target_fps;
}

void window_loop(void) {
    window.init();
    state.restart = false;
    while (!glfwWindowShouldClose(window.handle)) {
        window.now = glfwGetTime();
        window.tick();
        window.render();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
        window.deltaTime = glfwGetTime() - window.now;
    }
    window.destroy();
}
