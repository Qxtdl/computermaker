#pragma once

#include <stdint.h>

#include "../world.h"
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
		HUGE_MEMORY,
		DUAL_MEMORY,
		TEXT_CONSOLE,
		LARGE_RGB_DISPLAY
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
			uint16_t *cells;
		} hugemem;
	} state;
} building_t;

const char *building_id_name(enum BuildingId id);
void buildings_tick(void);
void building_tick(building_t *building);
void building_create(building_t building);

BUILDING_TICK_DECLARE(hugemem)
enum {
	HUGEMEM_A0,
	HUGEMEM_A1,
	HUGEMEM_A2,
	HUGEMEM_A3,
	HUGEMEM_A4,
	HUGEMEM_A5,
	HUGEMEM_A6,
	HUGEMEM_A7,
	HUGEMEM_A8,
	HUGEMEM_A9,
	HUGEMEM_A10,
	HUGEMEM_A11,
	HUGEMEM_A12,
	HUGEMEM_A13,
	HUGEMEM_A14,
	HUGEMEM_A15,
	HUGEMEM_V0,
	HUGEMEM_V1,
	HUGEMEM_V2,
	HUGEMEM_V3,
	HUGEMEM_V4,
	HUGEMEM_V5,
	HUGEMEM_V6,
	HUGEMEM_V7,
	HUGEMEM_V8,
	HUGEMEM_V9,
	HUGEMEM_V10,
	HUGEMEM_V11,
	HUGEMEM_V12,
	HUGEMEM_V13,
	HUGEMEM_V14,
	HUGEMEM_V15,
	HUGEMEM_O0,
	HUGEMEM_O1,
	HUGEMEM_O2,
	HUGEMEM_O3,
	HUGEMEM_O4,
	HUGEMEM_O5,
	HUGEMEM_O6,
	HUGEMEM_O7,
	HUGEMEM_O8,
	HUGEMEM_O9,
	HUGEMEM_O10,
	HUGEMEM_O11,
	HUGEMEM_O12,
	HUGEMEM_O13,
	HUGEMEM_O14,
	HUGEMEM_O15,	
	HUGEMEM_WRITE
};
