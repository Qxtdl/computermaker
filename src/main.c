#include "state.h"
#include "config.h"
#include "gfx/renderer.h"
#include "world/block/block.h"
#include "world/world.h"
#include "world/tick.h"
#include "player/input.h"
#include "player/hud.h"
#include "cm2save.h"

//testing
#include "world/wire.h"

void init(void) {
    config_open("res/config.comm");
    config_process();
    renderer_init(&state.renderer);
    state.renderer.wireframe = false;
    world_worldgen(&state.world);
    skybox_init(&state.world.skybox);
    world_wire_init();

    printf("Savestring: %s\n", config_get("SAVE"));
    cm2save_process(config_get("SAVE"));

    /* Test */
    // for (int x = 0; x < 4; x++) {
    //     for (int z = 15; z < 17; z++) {
    //         world_place_at(&state.world, x, 4, z, (block_t){.id = STUD});
    //     }
    // }

    world_create_wire((wire_t){
        .ox = 0,
        .oy = 2,
        .oz = 0,

        .dx = 3,
        .dy = 2,
        .dz = 0
    });
}

void destroy(void) {
    exit(0);
}

void tick(void) {
    world_tick(&state.world);
    input_handle();
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
