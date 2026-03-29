#include "camera.h"
#include <math.h>
#include <string.h>

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
    glm_vec3_add(camera->target, offset, camera->target);
}

void rotate(vec3 vec, vec3 rot) {
    double sa = sin(rot[2]),
           sb = sin(rot[0]),
           sg = sin(rot[1]),
           ca = cos(rot[2]),
           cb = cos(rot[0]),
           cg = cos(rot[1]);
    double x = vec[0],
           y = vec[1],
           z = vec[2];
    vec[0] = (float)(x * (ca * cb) + y * (ca * sb * sg - sa * cg) + z * (ca * sb * cg + sa * sg));
    vec[1] = (float)(x * (sa * cb) + y * (sa * sb * sg + ca * cg) + z * (sa * sb * cg - ca * sg));
    vec[2] = (float)(x * (-sb) + y * (cb * sg) + z * (cb * cg));
}

#define PI 3.141565f
vec3 scratch_buf;
vec3 pmouse = (vec3){0, 0, 0};
void camera_mouse_cb(camera_t *camera, double mouseX, double mouseY) {
    camera->rotation[0] += (mouseX - pmouse[0]) * PI / 180;
    camera->rotation[1] -= (mouseY - pmouse[1]) * PI / 180;
    // TODO: idk man, clamp camera Y rotation [89; -89] deg
    if (camera->rotation[1] < PI / 2) camera->rotation[1] = PI / 2;
    if (camera->rotation[1] > PI) camera->rotation[1] = PI;

    // scratch = rotate(front, rotation)
    memcpy(scratch_buf, camera->front, sizeof(vec3));
    rotate(scratch_buf, camera->rotation);
    // scratch += origin;
    glm_vec3_add(scratch_buf, camera->origin, scratch_buf);
    // target = scratch;
    memcpy(camera->target, scratch_buf, sizeof(vec3));
    pmouse[0] = mouseX;
    pmouse[1] = mouseY;
}
#undef PI
