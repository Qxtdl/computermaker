#include "camera.h"

void camera_init(camera_t *camera, vec3 origin, vec3 target, vec3 up, vec3 front) {
    memset(camera, 0, sizeof(*camera));
    memcpy(camera->origin, origin, sizeof(camera->origin));
    memcpy(camera->target, target, sizeof(camera->target));
    memcpy(camera->up, up, sizeof(camera->up));
    memcpy(camera->front, front, sizeof(camera->front));
}

void camera_perspective_init(camera_t *camera, float fov, float aspect, float nearZ, float farZ) {
    camera->perspective.fovy = fov;
    camera->perspective.aspect = aspect;
    camera->perspective.nearZ = nearZ;
    camera->perspective.farZ = farZ;
}

void camera_move(camera_t *camera, enum CameraDirection direction) {
    vec3 offset = {0};
    switch (direction) {
        case CAMERA_DIRECTION_FORWARD:
            offset[2] = -1;
            break;
        case CAMERA_DIRECTION_LEFT:
            offset[0] = -1;
            break;
        case CAMERA_DIRECTION_BACK:
            offset[2] = 1;
            break;
        case CAMERA_DIRECTION_RIGHT:
            offset[0] = 1;
    }

    glm_vec3_add(camera->origin, offset, camera->origin);
    glm_vec3_add(camera->target, camera->front, camera->target);
}

void camera_mouse_cb(camera_t *camera, double mouseX, double mouseY) {
    glm_vec3_add(camera->target, (vec3){x, 0, y}, camera->target);
}
