#pragma once

#include "../gfx/renderer.h"

typedef struct {
    enum BlockType {
        BLOCK_TYPE_INVISIBLE,
        BLOCK_TYPE_VISIBLE
    } type;
    enum RendererTextureType texture;
} block_t;
