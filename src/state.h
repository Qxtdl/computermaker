#pragma once

#include "gfx/renderer.h"
#include "gfx/window.h"
#include "world/world.h"

extern struct state {
    struct renderer renderer;
    struct world world;
    struct player {
        enum mode {
            MODE_BLOCK_PLACE,
            MODE_WIRE_PLACE,
            MODE_WIRE_DESTROY,
            MODE_BLOCK_POKE,
            MODE_BLOCK_HOVER,
            MODE_BUILDING_PLACE,
            MODE_LAST
        } mode;
        // block placing
        enum BlockId selected_block;

        // wire placing
        bool planout;
        int wire_ox, wire_oy, wire_oz;

        // hovered block
        block_t *hovered_block;
    } player;
    float deltaTime;
} state;

const char *mode_name(enum mode mode);
