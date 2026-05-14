#pragma once

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef void (*WindowCallback)(void);

extern struct window {
    GLFWwindow *handle;
    const char *title;
    int width, height;
    WindowCallback init, destroy, tick, render;
    struct {
        struct {
            bool down;
        } keys[GLFW_KEY_LAST];
    } keyboard;
    struct {
        struct {
            bool down;
        } buttons[GLFW_MOUSE_BUTTON_LAST];
        double x, y;
        bool moved;
        struct {
            double x, y;
        } scroll;
        bool scrolled;
    } mouse;
    double now, deltaTime, target_fps;
} window;

void window_init(void);
void window_create(
    const char *title, int width, int height,
    WindowCallback init,
    WindowCallback destroy,
    WindowCallback tick,
    WindowCallback render
);
void window_loop(void);

