#include "building.h"

void _multiplier_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint16_t input_a = 0, input_b = 0;
    uint32_t output;

	int i;
	for (i = 0; i < 16; i++) {
		input_a |= (building->pins[MULTIPLIER_A0+i]->gate.state << i);
	}
	for (i = 0; i < 16; i++) {
		input_b |= (building->pins[MULTIPLIER_B0+i]->gate.state << i);
	}
    output = input_a*input_b;

	for (i = 0; i < 32; i++) {
		building->pins[MULTIPLIER_O0+i]->gate.new_state = (output >> i) & 1;
	}
}
