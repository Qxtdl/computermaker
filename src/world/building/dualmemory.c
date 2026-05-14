#include "building.h"

void _dualmemory_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint64_t save_address = 0, load_address = 0, value = 0, output = 0;
	uint8_t write = 0;

	int i;
	for (i = 0; i < building->state.memory.address_width; i++) {
		save_address |= (building->pins[i]->gate.state << i);
	}
	for (i = 0; i < building->state.memory.address_width; i++) {
		load_address |= (building->pins[building->state.memory.address_width+i]->gate.state << i);
	}
	for (i = 0; i < building->bit_width; i++) {
		value |= (building->pins[building->state.memory.address_width*2+building->bit_width+i]->gate.state << i);
	}

    write = building->pins[building->state.memory.address_width*2+building->bit_width*2]->gate.state;

	if (building->bit_width <= 8) {
		if (write) {
			((uint8_t*)(building->state.memory.cells))[save_address] = value;
		}
		output = ((uint8_t*)(building->state.memory.cells))[load_address];
	}
	else if (building->bit_width <= 16) {
		if (write) {
			((uint16_t*)(building->state.memory.cells))[save_address] = value;
		}
		output = ((uint16_t*)(building->state.memory.cells))[load_address];
	}
	else if (building->bit_width <= 32) {
		if (write) {
			((uint32_t*)(building->state.memory.cells))[save_address] = value;
		}
		output = ((uint32_t*)(building->state.memory.cells))[load_address];
	}
	else {
		if (write) {
			((uint64_t*)(building->state.memory.cells))[save_address] = value;
		}
		output = ((uint64_t*)(building->state.memory.cells))[load_address];
	}

	for (i = 0; i < building->bit_width; i++) {
		building->pins[building->state.memory.address_width*2+i]->gate.new_state |= (output >> i) & 1;
	}
}
