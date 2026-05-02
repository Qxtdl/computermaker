#include "block.h"

void _xnor_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
        block->gate.new_state = STATE_ON;
        return;
    }
    int count = 0;
    for (int i = 0; i < block->gate.num_inputs; i++) {
        if (!block->gate.inputs[i].gate) continue;
        if (block->gate.inputs[i].gate->state == STATE_ON) {
            count++;
        }
    }

    if (count % 2)
        block->gate.new_state = STATE_OFF;
    else
        block->gate.new_state = STATE_ON;
}
