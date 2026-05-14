#pragma once

#include <stdint.h>

#include "../block/block.h"

#define BUILDING_MAX_PINS 256

#define BUILDING_TICK_DECLARE(name) extern void _##name##_tick(building_t *building);
#define BUILDING_PLACE_PIN(_x, _y, _z) \
	{ \
		int rx, ry, rz; \
		switch (building.rotation) { \
			case ROTATION_FRONT: rx = building.x + (_x); ry = building.y + (_y); rz = building.z + (_z); break; \
			case ROTATION_LEFT: rx = building.x + (_z); ry = building.y + (_y); rz = building.z - (_x); break; \
			case ROTATION_BACK: rx = building.x - (_x); ry = building.y + (_y); rz = building.z - (_z); break; \
			case ROTATION_RIGHT: rx = building.x - (_z); ry = building.y + (_y); rz = building.z + (_x); break; \
			default: rx = building.x + (_x), ry = building.y + (_y), rz = building.z + (_z); \
		} \
		struct world_get_at_info info = world_place_at(&state.world, (rx), (ry), (rz), (block_t){ \
			.id = BUILDING_PIN, \
			.gate = {0} \
		}); \
		building.pins[pin_index++] = &info.chunk->blocks[info.x][info.y][info.z]; \
	}

typedef struct {
	enum BuildingId {
		ASCII_KEY_INPUT,
		MEMORY,
		DUAL_MEMORY,
		TEXT_CONSOLE,
		LARGE_RGB_DISPLAY,
		MULTIPLIER,
		DIVIDER,
	} id;
	int x, y, z;
	enum {
		ROTATION_FRONT,
		ROTATION_LEFT,
		ROTATION_BACK,
		ROTATION_RIGHT
	} rotation;
	block_t *pins[BUILDING_MAX_PINS];
	union {
		struct {
			void *cells;
			int address_width;
		} memory;
	} state;
	int bit_width;
} building_t;

const char *building_id_name(enum BuildingId id);
void buildings_tick(void);
void building_tick(building_t *building);
void building_create(building_t building);

BUILDING_TICK_DECLARE(memory)
BUILDING_TICK_DECLARE(dualmemory)
BUILDING_TICK_DECLARE(multiplier)
BUILDING_TICK_DECLARE(divider)
