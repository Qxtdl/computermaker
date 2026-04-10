#include "block.h"
#include "../util.h"
#include "../global.h"
#include "uv.h"

void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv) {
    if (block.id >= BLOCKID_LAST) {
        printf("Invalid block id: %d\n", block.id);
    }
    memcpy(scale, uv_get_scale(block), sizeof(vec2));
    memcpy(uv, uv_get_position(block), sizeof(vec2));
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

