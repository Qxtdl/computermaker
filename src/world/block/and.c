#include "block.h"

void _and_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv) {
    switch (block.gate.state) {
        case STATE_OFF:
            memcpy(scale, (vec2){0.0625, 0.0625}, sizeof(vec2));
            memcpy(uv, (vec2){0.5, 0.0625}, sizeof(vec2));
            break;
        case STATE_ON:
            memcpy(scale, (vec2){0.0625, 0.0625}, sizeof(vec2));
            memcpy(uv, (vec2){0.5625, 0.0625}, sizeof(vec2));            
    }
}

void _and_tick(block_t *block) {
    if (block->gate.num_inputs == 0) {
        block->gate.new_state = STATE_OFF;
        return;
    }
    for (int i = 0; i < block->gate.num_inputs; i++) {
        if (block->gate.inputs[i].gate->state == STATE_OFF) {
            block->gate.new_state = STATE_OFF;
            return;
        }
    }
    block->gate.new_state = STATE_ON;
}
