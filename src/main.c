#include <stdlib.h>

#include "state.h"
#include "gfx/window.h"
#include "config.h"
#include "global.h"
#include "gfx/renderer.h"
#include "world/save.h"
#include "world/world.h"
#include "world/tick.h"
#include "world/wire.h"
#include "player/input.h"
#include "player/hud.h"
#include "player/chat.h"
#include "cm2save.h"

void init(void) {
    config_open("res/config.comm");
    config_process();
    renderer_init(&state.renderer, (float)window.width/(float)window.height);
    skybox_init(&state.world.skybox);
    world_wire_init();
    if (!save_load(config_get("SAVELOAD")))
        world_worldgen(&state.world);
    cm2save_process(config_get("SAVE"));
}

void destroy(void) {
    chat_cleanup();
    exit(0);
}

void tick(void) {
    if (state.restart) {
        for (size_t i = 0; i < state.world.chunks_size; i++) {
            free(state.world.chunks[i]);
        }
        state.world.chunks_size = 0;
        config_clear();
        window.init();
        state.restart = false;
    }
    input_handle();
    world_tick(&state.world);
}

void render(void) {
    skybox_draw(&state.world.skybox, &state.renderer);
    world_draw(&state.world);
    hud_draw();
}

int main() {    
    window_init();
    window_create("comm", 800, 600, init, destroy, tick, render);
    chat_add_message("comm", APP_RELEASE_STRING);
    window_loop();
}
