#include "block.h"

void _nand_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
        block->gate.new_state = STATE_ON;
        return;
    }
    for (int i = 0; i < block->gate.inputs_size; i++) {
        if (!block->gate.inputs[i].gate) continue;
        if (block->gate.inputs[i].gate->state == STATE_OFF) {
            block->gate.new_state = STATE_ON;
            return;
        }
    }
    block->gate.new_state = STATE_OFF;
}
