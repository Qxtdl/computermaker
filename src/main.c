#include <stdlib.h>

#include "state.h"
#include "config.h"
#include "gfx/renderer.h"
#include "gfx/raycast.h"
#include "world/save.h"
#include "world/world.h"
#include "world/tick.h"
#include "world/wire.h"
#include "player/input.h"
#include "player/hud.h"
#include "cm2save.h"

void init(void) {
    config_open("res/config.comm");
    config_process();
    renderer_init(&state.renderer, 800.0f/600.0f);
    skybox_init(&state.world.skybox);
    world_wire_init();
    set_wire_thickness(atof(config_get("WIRE_THICKNESS")));
    if (!save_load(config_get("SAVELOAD")))
        world_worldgen(&state.world);
    cm2save_process(config_get("SAVE"));
    raycast_set_distance(atof(config_get("RAY_DISTANCE")));
}

void destroy(void) {
    exit(0);
}

void tick(void) {
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
    window_loop();
}
