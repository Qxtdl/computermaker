#include "block.h"
#include "../util.h"
#include "../global.h"
#include "uv.h"

const char *block_id_name(enum BlockId id) {
    switch (id) {
        case AIR : return "AIR";
        case STUD: return "STUD";
        case BRICK: return "BRICK";

        // logic gates
        case AND: return "AND";
        case OR: return "OR";
        case XOR: return "XOR";
        case NAND: return "NAND";

        default: return "UNKNOWN";
    }
}

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
        case OR: _or_tick(block); break;
        case XOR: _xor_tick(block); break;
        case NAND: _nand_tick(block); break;
        default: break;
    }
}

bool is_logic_block(block_t block) {
    switch (block.id) {
        case AND: return true;
        case OR: return true;
        case XOR: return true;
        case NAND: return true;
        default: return false;
    }
}

void logic_block_add_input(block_t *from, block_t *to) {
    for (int i = 0; i < to->gate.num_inputs; i++) {
        if (!to->gate.inputs[i].gate) {
            to->gate.inputs[i].gate = &from->gate;
            return;
        }
    }    
    to->gate.inputs = srealloc(to->gate.inputs, ++to->gate.num_inputs * sizeof(input_t));
    to->gate.inputs[to->gate.num_inputs - 1].gate = &from->gate;
}

void logic_block_remove_input(block_t *from, block_t *to) {
    for (int i = 0; i < to->gate.num_inputs; i++) {
        if (!to->gate.inputs[i].gate) continue;
        if (to->gate.inputs[i].gate == &from->gate) {
            to->gate.inputs[i].gate = NULL;
            break;
        }
    }
}
