#include "building.h"

void _hugemem_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint16_t address = 0, value = 0, output = 0;
	uint8_t write = 0;

	int i;
	for (i = 0; i < 16; i++) {
		address |= (building->pins[HUGEMEM_A0+i]->gate.state << i);
	}
	for (i = 0; i < 16; i++) {
		value |= (building->pins[HUGEMEM_V0+i]->gate.state << i);
	}
	write = building->pins[HUGEMEM_WRITE]->gate.state;

	output = building->state.hugemem.cells[address];
	
	if (write) {
		building->state.hugemem.cells[address] = value;
	}

	for (i = 0; i < 16; i++) {
		building->pins[HUGEMEM_O0+i]->gate.new_state = (output >> i) & 1;
	}
}
