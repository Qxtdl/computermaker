#include "building.h"
#include "../../state.h"
#include "../../util.h"

building_t **buildings = NULL;
size_t buildings_size = 0, buildings_count = 1;

const char *building_id_name(enum BuildingId id) {
	switch (id) {
		case ASCII_KEY_INPUT: return "ASCII_KEY_INPUT";
		case HUGE_MEMORY: return "HUGE_MEMORY";
		case DUAL_MEMORY: return "DUAL_MEMORY";
		case TEXT_CONSOLE: return "TEXT_CONSOLE";
		case LARGE_RGB_DISPLAY: return "LARGE_RGB_DISPLAY";
		default: return "UNKNOWN";
	}
}

static void push_building(building_t *building) {
	buildings_size++;
	while (buildings_count <= buildings_size) {
		buildings_count <<= 1;
		buildings = srealloc(buildings, buildings_count * sizeof(building_t *));
	}
	buildings[buildings_size - 1] = building;
}

void building_tick(building_t *building) {
	switch (building->id) {
		case HUGE_MEMORY: _hugemem_tick(building);break;
		case MULTIPLIER: _multiplier_tick(building);break;
		case DUAL_MEMORY: _dualmem_tick(building);break;
		default:break;
	}
}

void buildings_tick(void) {
	for (size_t i = 0; i < buildings_size; i++) {
		building_tick(buildings[i]);
	}
}

void building_create(building_t building) {
	int pin_index = 0;
	switch (building.id) {
		case HUGE_MEMORY:
			building.state.hugemem.cells = scalloc(1, 65536 * sizeof(uint16_t));
			// generate a address bus
			for (int x = 0; x < 16; x++) {
				BUILDING_PLACE_PIN(x + 2, 0, -3)
			}
			// generate a value bus
			int vx;
			for (vx = 0; vx < 16; vx++) {
				BUILDING_PLACE_PIN(vx - 16, 0, -3)
			}
			// generate a output bus
			for (int x = 0; x < 16; x++) {
				BUILDING_PLACE_PIN(x + 2, 0, 3)
			}
			// place the write pin
			BUILDING_PLACE_PIN(-vx - 2, 0, -3)
			break;
		
		case MULTIPLIER:
			// generate input a
			for (int x = 0; x < 16; x++) {
				BUILDING_PLACE_PIN(x -14, 0, -7)
			}
			// generate input b
			for (int x = 0; x < 16; x++) {
				BUILDING_PLACE_PIN(x -32, 0, -7)
			}
			// generate lower half of output
			for (int x = 0; x < 16; x++) {
				BUILDING_PLACE_PIN(x -32, 0, 1)
			}
			// generate upper half of output
			for (int x = 0; x < 16; x++) {
				BUILDING_PLACE_PIN(x -14, 0, 1)
			}
			break;

		case DUAL_MEMORY:
			building.state.dualmem.cells = scalloc(1, 256 * sizeof(uint8_t));
			// generate save address bus
			for (int x = 0; x < 8; x++) {
				BUILDING_PLACE_PIN(x - 3, 0, -2)
			}
			// generate load address bus
			for (int x = 0; x < 8; x++) {
				BUILDING_PLACE_PIN(x + 6, 0, -2)
			}
			// generate output bus
			for (int x = 0; x < 8; x++) {
				BUILDING_PLACE_PIN(x + 6, 0, 2)
			}
			// generate value bus
			for (int x = 0; x < 8; x++) {
				BUILDING_PLACE_PIN(x - 12, 0, -2)
			}
			// place the write pin
			BUILDING_PLACE_PIN(-14, 0, -2)
			break;

		default: break;			
	}

	building_t *_building = smalloc(sizeof(building_t));
	memcpy(_building, &building, sizeof(building_t));
	push_building(_building);
}
