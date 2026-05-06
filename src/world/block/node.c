#include "block.h"

void _node_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
    	block->gate.state = STATE_OFF;
        return;
    }
	block->gate.state = STATE_OFF;
    for (int i = 0; i < block->gate.num_inputs; i++) {
        if (!block->gate.inputs[i].gate) continue;
        if (block->gate.inputs[i].gate->state == STATE_ON) {
            block->gate.state = STATE_ON;
            return;
        }
    }
}
