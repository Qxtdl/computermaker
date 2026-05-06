#pragma once

#include <string.h>
#include <cglm/cglm.h>

typedef struct {
    float fovy;
    float aspect;
    float nearZ;
    float farZ;
} perspective_t;

typedef struct {
    vec3 origin;
    vec3 target;
    vec3 rotation;
    vec3 up;
    vec3 front;
    perspective_t perspective;
    int render_distance; // in chunks
} camera_t;

enum CameraDirection {
    CAMERA_DIRECTION_FORWARD,
    CAMERA_DIRECTION_LEFT,
    CAMERA_DIRECTION_BACK,
    CAMERA_DIRECTION_RIGHT
};

void camera_init(camera_t *camera, vec3 origin, vec3 target, vec3 up, vec3 front);
void camera_perspective_init(camera_t *camera, float fov, float aspect, float nearZ, float farZ);
void camera_move(camera_t *camera, enum CameraDirection direction);
void camera_mouse_cb(camera_t *camera, double mouseX, double mouseY);

