#include "building.h"

void _divider_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint16_t input_a = 0, input_b = 0;
    uint16_t output_quotient = 0, output_remainder = 0;

	int i;
	for (i = 0; i < 16; i++) {
		input_a |= (building->pins[DIVIDER_A0+i]->gate.state << i);
	}
	for (i = 0; i < 16; i++) {
		input_b |= (building->pins[DIVIDER_B0+i]->gate.state << i);
	}
    if (input_b != 0) {
        output_quotient = input_a / input_b;
        output_remainder = input_a % input_b;
    }
	for (i = 0; i < 16; i++) {
		building->pins[DIVIDER_OR0+i]->gate.new_state = (output_remainder >> i) & 1;
	}
	for (i = 0; i < 16; i++) {
		building->pins[DIVIDER_OQ0+i]->gate.new_state = (output_quotient >> i) & 1;
	}
}
