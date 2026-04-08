#include "block.h"
#include "../util.h"

void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv) {
    switch (block.id) {
        case AIR: BLOCK_UV_GET(air, block, face, scale, uv) break;
        case STUD: BLOCK_UV_GET(stud, block, face, scale, uv) break;
        case BRICK: BLOCK_UV_GET(brick, block, face, scale, uv) break;
        case AND: BLOCK_UV_GET(and, block, face, scale, uv) break;
        case NAND: BLOCK_UV_GET(nand, block, face, scale, uv) break;
        default: break;
    }
}

void block_tick(block_t *block) {
    switch (block->id) {
        case AND: _and_tick(block); break;
        case NAND: _nand_tick(block); break;
        default: break;
    }
}

bool is_logic_block(block_t block) {
    switch (block.id) {
        case AND: return true;
        case NAND: return true;
        default: return false;
    }
}

void logic_block_add_input(block_t *from, block_t *to) {
    to->gate.inputs = srealloc(to->gate.inputs, ++to->gate.num_inputs * sizeof(input_t));
    to->gate.inputs[to->gate.num_inputs - 1].gate = &from->gate;
}
