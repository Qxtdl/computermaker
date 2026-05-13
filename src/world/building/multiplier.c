#include "building.h"

void _multiplier_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint64_t input_a = 0, input_b = 0;
    __uint128_t output;

	int i;
	for (i = 0; i < building->bitwidth; i++) {
		input_a |= (building->pins[i]->gate.state << i);
	}
	for (i = 0; i < building->bitwidth; i++) {
		input_b |= (building->pins[building->bitwidth+i]->gate.state << i);
	}
    output = (__uint128_t)input_a*input_b;

	for (i = 0; i < 2*building->bitwidth; i++) {
		building->pins[building->bitwidth*2+i]->gate.new_state = (output >> i) & 1;
	}
}
