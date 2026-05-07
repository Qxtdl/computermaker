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
        case NOR: return "NOR";
        case XNOR: return "XNOR";
        case FLIPFLOP: return "FLIPFLOP";
        case NODE: return "NODE";

        default: return "UNKNOWN";
    }
}

void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv) {
    if (block.id >= BLOCKID_LAST) {
        app_warn("Invalid block id: %d\n", block.id);
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
        case NOR: _nor_tick(block); break;
        case XNOR: _xnor_tick(block); break;
        case FLIPFLOP: _flipflop_tick(block); break;
        case NODE: _node_tick(block); break;
        default: break;
    }
}

bool is_logic_block(block_t block) {
    switch (block.id) {
        case AND:
        case OR:
        case XOR:
        case NAND:
        case NOR:
        case XNOR:
        case FLIPFLOP:
        case NODE:
        return true;
        default: return false;
    }
}

void logic_block_add_input(block_t *from, block_t *to) {
    for (int i = 0; i < to->gate.inputs_size; i++) {
        if (!to->gate.inputs[i].gate) {
            to->gate.inputs[i].gate = &from->gate;
            to->gate.num_inputs++;
            return;
        }
    }    
    to->gate.num_inputs++;
    to->gate.inputs = srealloc(to->gate.inputs, ++to->gate.inputs_size * sizeof(input_t));
    to->gate.inputs[to->gate.inputs_size - 1].gate = &from->gate;
}

void logic_block_remove_input(block_t *from, block_t *to) {
    for (int i = 0; i < to->gate.inputs_size; i++) {
        if (!to->gate.inputs[i].gate) continue;
        if (to->gate.inputs[i].gate == &from->gate) {
            to->gate.inputs[i].gate = NULL;
            to->gate.num_inputs--;
            break;
        }
    }
}
