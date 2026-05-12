#include "building.h"

void _dualmem_tick(building_t *building) {
	// TODO: remove '= 0' initalizers if all are read
	uint8_t save_address = 0, load_address = 0, value = 0, output = 0;
	uint8_t write = 0;

	int i;
	for (i = 0; i < 8; i++) {
		save_address |= (building->pins[DUALMEM_AW0+i]->gate.state << i);
	}
	for (i = 0; i < 8; i++) {
		load_address |= (building->pins[DUALMEM_AR0+i]->gate.state << i);
	}
	for (i = 0; i < 8; i++) {
		value |= (building->pins[DUALMEM_V0+i]->gate.state << i);
	}

    write = building->pins[DUALMEM_WRITE]->gate.state;

	if (write) {
        building->state.hugemem.cells[save_address] = value;
	}
    
    output = building->state.hugemem.cells[load_address];

	for (i = 0; i < 8; i++) {
		building->pins[DUALMEM_O0+i]->gate.new_state = (output >> i) & 1;
	}
}
