#include "block.h"

void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv) {
    switch (block.id) {
        case AIR: BLOCK_UV_GET(air, face, scale, uv) break;
        case STUD: BLOCK_UV_GET(stud, face, scale, uv) break;
        case BRICK: BLOCK_UV_GET(brick, face, scale, uv) break;
    }
}
