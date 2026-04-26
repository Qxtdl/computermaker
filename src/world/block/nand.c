#include "block.h"

void _nand_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
        block->gate.new_state = STATE_ON;
        return;
    }
    for (int i = 0; i < block->gate.num_inputs; i++) {
        if (!block->gate.inputs[i].gate) continue;
        if (block->gate.inputs[i].gate->state == STATE_ON) {
            block->gate.new_state = STATE_OFF;
            return;
        }
    }
    block->gate.new_state = STATE_ON;
}

