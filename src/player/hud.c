#include "../global.h"
#include "../state.h"
#include "../gfx/renderer.h"

void hud_draw(void) {
    glDisable(GL_DEPTH_TEST);

    char buf[128];
    snprintf(buf, sizeof(buf), 
        "Selected block: \n"
        "%s \n"
        "(q to change block)\n"
        "Mode: %s\n"
        "(e to cycle)", 
        block_id_name(state.player.selected_block),
        mode_name(state.player.mode)
    );

    renderer_text(0, 0, 2, APP_RELEASE_STRING);
    renderer_text(0, 32, 2, buf);
}
