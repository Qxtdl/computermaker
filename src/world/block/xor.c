#include "block.h"

void _xor_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
        block->gate.new_state = STATE_OFF;
        return;
    }

    int state = 0;
    for (int i = 0; i < block->gate.inputs_size; i++) {
        if (!block->gate.inputs[i].gate) continue;
        state ^= block->gate.inputs[i].gate->state;
    }
    block->gate.new_state = state;
}
