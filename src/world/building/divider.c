#include "building.h"

void _divider_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint64_t input_a = 0, input_b = 0;
    uint64_t output_quotient = 0, output_remainder = 0;

	int i;
	for (i = 0; i < building->bitwidth; i++) {
		input_a |= (building->pins[i]->gate.state << i);
	}
	for (i = 0; i < building->bitwidth; i++) {
		input_b |= (building->pins[building->bitwidth+i]->gate.state << i);
	}
    if (input_b != 0) {
        output_quotient = input_a / input_b;
        output_remainder = input_a % input_b;
    }
	for (i = 0; i < building->bitwidth; i++) {
		building->pins[building->bitwidth*2+i]->gate.new_state = (output_remainder >> i) & 1;
	}
	for (i = 0; i < building->bitwidth; i++) {
		building->pins[building->bitwidth*3+i]->gate.new_state = (output_quotient >> i) & 1;
	}
}
