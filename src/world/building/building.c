#include "building.h"
#include "../../util.h"

building_t *buildings = NULL;
size_t buildings_size, buildings_count;

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

static void push_building(building_t building) {
	buildings_size++;
	while (buildings_count <= buildings_size) {
		buildings_count <<= 1;
		buildings = srealloc(buildings, buildings_count * sizeof(building_t));
	}
	buildings[buildings_size - 1] = building;
}

void building_create(building_t building) {

}
