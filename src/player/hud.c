#include "../state.h"
#include "../gfx/renderer.h"

void hud_draw(void) {
    glDisable(GL_DEPTH_TEST);

    char buf[64], buf2[64];
    snprintf(buf, sizeof(buf), "selected block: %d\n(q to change block)\nmode:%d (e to cycle)", state.player.selected_block, state.player.mode);
    snprintf(buf2, sizeof(buf2), "chunks in memory: %zu", state.world.chunks_size);

    renderer_text(0, 0, 2, "alpha 2.2.0");
    renderer_text(0, 32, 2, buf);
    renderer_text(0, 128, 2, buf2);
}
