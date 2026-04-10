#pragma once

#include "gfx/renderer.h"
#include "gfx/window.h"
#include "world/world.h"

extern struct state {
    struct renderer renderer;
    struct world world;
    struct player {
        enum BlockId selected_block;
    } player;
    float deltaTime;
} state;

