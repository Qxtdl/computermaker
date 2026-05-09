#pragma once

#include "../vec.h"
#include "gfx/renderer.h"
#include "world/world.h"

extern struct state {
    struct renderer renderer;
    struct world world;
    struct player {
        enum mode {
            MODE_BLOCK_PLACE,
            MODE_WIRE_PLACE,
            MODE_WIRE_DESTROY,
            MODE_WIRE_PARALLEL_PLACE,
            MODE_WIRE_PARALLEL_DESTROY,
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

        // parallel wire placing
        enum {
            FIRST_POINT,
            SECOND_POINT,
            THIRD_POINT,
            FOURTH_POINT,
            LAST_POINT = 4
        } point_stage;
        int point;
        ivec3 points[4];

        // hovered block
        block_t *hovered_block;
    } player;
    float deltaTime;
} state;

const char *mode_name(enum mode mode);
