#include "building.h"
#include "../../state.h"
#include "../../util.h"
#include "../../global.h"

building_t **buildings = NULL;
size_t buildings_size = 0, buildings_count = 1;

const char *building_id_name(enum BuildingId id) {
	switch (id) {
		case ASCII_KEY_INPUT: return "ASCII_KEY_INPUT";
		case MEMORY: return "MEMORY";
		case DUAL_MEMORY: return "DUAL_MEMORY";
		case TEXT_CONSOLE: return "TEXT_CONSOLE";
		case LARGE_RGB_DISPLAY: return "LARGE_RGB_DISPLAY";
		case MULTIPLIER: return "MULTIPLIER";
		case DIVIDER: return "DIVIDER";
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
		case MEMORY: _memory_tick(building);break;
		case DUAL_MEMORY: _dualmemory_tick(building);break;
		case MULTIPLIER: _multiplier_tick(building);break;
		case DIVIDER: _divider_tick(building);break;
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
	int bytewidth, memory_thickness;
	if (building.bitwidth <= 8) bytewidth = sizeof(uint8_t);
	else if (building.bitwidth <= 16) bytewidth = sizeof(uint16_t);
	else if (building.bitwidth <= 32) bytewidth = sizeof(uint32_t);
	else bytewidth = sizeof(uint64_t);
	switch (building.id) {
		case MEMORY:
			if (building.bitwidth > 64) {
				app_warn("bitwidth too big\n")
				break;
			}
			if (building.addresswidth > 32) {
				app_warn("addresswidth too big\n")
				break;
			}
			building.state.memory.cells = scalloc(1, ((uint64_t)1 << building.addresswidth) * bytewidth);
			
			if (building.addresswidth >= 16) memory_thickness = 3;
			else memory_thickness = 2;

			int center_x = (-building.addresswidth -building.bitwidth -memory_thickness)/2;
			
			// generate a address bus
			for (int x = 0; x < building.addresswidth; x++) {
				BUILDING_PLACE_PIN(-center_x +x -building.addresswidth +1, 0, -memory_thickness)
			}
			// generate a value bus
			int vx;
			for (vx = 0; vx < building.bitwidth; vx++) {
				BUILDING_PLACE_PIN(-center_x +vx -building.addresswidth -building.bitwidth -memory_thickness +2, 0, -memory_thickness)
			}
			// generate a output bus
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(-center_x +x -building.bitwidth +1, 0, memory_thickness)
			}
			// place the write pin
			BUILDING_PLACE_PIN(-center_x -building.addresswidth -building.bitwidth -memory_thickness, 0, -memory_thickness)
			break;
		
		case DUAL_MEMORY:
			if (building.bitwidth > 64) {
				app_warn("bitwidth too big\n")
				break;
			}
			if (building.addresswidth > 32) {
				app_warn("addresswidth too big\n")
				break;
			}
			building.state.memory.cells = scalloc(1, ((uint64_t)1 << building.addresswidth) * bytewidth);

			if (building.addresswidth >= 16) memory_thickness = 3;
			else memory_thickness = 2;

			// generate save address bus
			for (int x = 0; x < building.addresswidth; x++) {
				BUILDING_PLACE_PIN(x - 3, 0, -2)
			}
			// generate load address bus
			for (int x = 0; x < building.addresswidth; x++) {
				BUILDING_PLACE_PIN(x - 2 + building.addresswidth, 0, -2)
			}
			// generate output bus
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x - 2 + building.addresswidth, 0, 2)
			}
			// generate value bus
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x - 4 - building.bitwidth, 0, -2)
			}
			// place the write pin
			BUILDING_PLACE_PIN(-6 - building.bitwidth, 0, -2)
			break;

		case MULTIPLIER:
			if (building.bitwidth > 64) {
				app_warn("bitwidth too big\n")
				break;
			}
			// generate input a
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x + 2 - building.bitwidth, 0, -7)
			}
			// generate input b
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x - 2*building.bitwidth, 0, -7)
			}
			// generate lower half of output
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x - 2*building.bitwidth, 0, 1)
			}
			// generate upper half of output
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x + 2 - building.bitwidth, 0, 1)
			}
			break;
		
		case DIVIDER:
			if (building.bitwidth > 64) {
				app_warn("bitwidth too big\n")
				break;
			}
			// generate input a
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x + 2 - building.bitwidth, 0, -7)
			}
			// generate input b
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x - 2*building.bitwidth, 0, -7)
			}
			// generate remainder output
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x - 2*building.bitwidth, 0, 1)
			}
			// generate quotient output
			for (int x = 0; x < building.bitwidth; x++) {
				BUILDING_PLACE_PIN(x + 2 - building.bitwidth, 0, 1)
			}
			break;

		default: break;			
	}

	building_t *_building = smalloc(sizeof(building_t));
	memcpy(_building, &building, sizeof(building_t));
	push_building(_building);
}
