#include "building.h"

void _multiplier_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint64_t input_a = 0, input_b = 0;
    __uint128_t output;

	int i;
	for (i = 0; i < building->bit_width; i++) {
		input_a |= (building->pins[i]->gate.state << i);
	}
	for (i = 0; i < building->bit_width; i++) {
		input_b |= (building->pins[building->bit_width+i]->gate.state << i);
	}
    output = (__uint128_t)input_a*input_b;

	for (i = 0; i < 2*building->bit_width; i++) {
		building->pins[building->bit_width*2+i]->gate.new_state |= (output >> i) & 1;
	}
}
