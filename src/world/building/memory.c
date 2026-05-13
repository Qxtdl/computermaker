#include "building.h"
#include <stdint.h>

void _memory_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint64_t address = 0, value = 0, output = 0;
	uint8_t write = 0;

	int i;
	for (i = 0; i < building->addresswidth; i++) {
		address |= (building->pins[i]->gate.state << i);
	}
	for (i = 0; i < building->bitwidth; i++) {
		value |= (building->pins[building->addresswidth+i]->gate.state << i);
	}

	write = building->pins[building->addresswidth+2*building->bitwidth]->gate.state;

	if (building->bitwidth <= 8) {
		if (write) {
			((uint8_t*)(building->state.memory.cells))[address] = value;
		}
		output = ((uint8_t*)(building->state.memory.cells))[address];
	}
	else if (building->bitwidth <= 16) {
		if (write) {
			((uint16_t*)(building->state.memory.cells))[address] = value;
		}
		output = ((uint16_t*)(building->state.memory.cells))[address];
	}
	else if (building->bitwidth <= 32) {
		if (write) {
			((uint32_t*)(building->state.memory.cells))[address] = value;
		}
		output = ((uint32_t*)(building->state.memory.cells))[address];
	}
	else {
		if (write) {
			((uint64_t*)(building->state.memory.cells))[address] = value;
		}
		output = ((uint64_t*)(building->state.memory.cells))[address];
	}

	

	for (i = 0; i < building->bitwidth; i++) {
		building->pins[building->addresswidth+building->bitwidth+i]->gate.new_state = (output >> i) & 1;
	}
}
