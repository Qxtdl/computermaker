#include "skybox.h"

static void texture_setup(void) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void skybox_init(skybox_t *skybox) {
    skybox->shader = shader_load("res/shader/skybox.vert", "res/shader/skybox.frag");
    skybox->faces[CUBEMAP_FACE_TOP] = texture_load("res/texture/sky512_up.png", texture_setup);
    skybox->faces[CUBEMAP_FACE_BOTTOM] = texture_load("res/texture/sky512_dn.png", texture_setup);
    skybox->faces[CUBEMAP_FACE_RIGHT] = texture_load("res/texture/sky512_rt.png", texture_setup);
    skybox->faces[CUBEMAP_FACE_LEFT] = texture_load("res/texture/sky512_lf.png", texture_setup);
    skybox->faces[CUBEMAP_FACE_FRONT] = texture_load("res/texture/sky512_ft.png", texture_setup);
    skybox->faces[CUBEMAP_FACE_BACK] = texture_load("res/texture/sky512_bk.png", texture_setup);

}