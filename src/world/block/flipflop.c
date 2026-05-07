#include "block.h"

void _flipflop_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
        return;
    }
    for (int i = 0; i < block->gate.inputs_size; i++) {
        if (!block->gate.inputs[i].gate) continue;
        if (block->gate.inputs[i].gate->new_state != block->gate.inputs[i].gate->state &&
            block->gate.inputs[i].gate->new_state == STATE_ON
        ) {
            block->gate.new_state = !block->gate.state;
            return;
        }
    }
}
