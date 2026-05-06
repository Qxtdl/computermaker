#pragma once

typedef struct {
	enum BuildingId {
		ASCII_KEY_INPUT,
		HUGE_MEMORY,
		DUAL_MEMORY,
		TEXT_CONSOLE,
		LARGE_RGB_DISPLAY
	} id;
	int x, y, z;
} building_t;

const char *building_id_name(enum BuildingId id);
static void push_building(building_t building);
void building_create(building_t building);
