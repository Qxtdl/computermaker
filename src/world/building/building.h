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
		HUGE_MEMORY,
		DUAL_MEMORY,
		TEXT_CONSOLE,
		LARGE_RGB_DISPLAY,
		MULTIPLIER
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
		struct {
			uint8_t *cells;
		} dualmem;
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

BUILDING_TICK_DECLARE(dualmem)
enum {
	DUALMEM_AW0,
	DUALMEM_AW1,
	DUALMEM_AW2,
	DUALMEM_AW3,
	DUALMEM_AW4,
	DUALMEM_AW5,
	DUALMEM_AW6,
	DUALMEM_AW7,
	DUALMEM_AR0,
	DUALMEM_AR1,
	DUALMEM_AR2,
	DUALMEM_AR3,
	DUALMEM_AR4,
	DUALMEM_AR5,
	DUALMEM_AR6,
	DUALMEM_AR7,
	DUALMEM_O0,
	DUALMEM_O1,
	DUALMEM_O2,
	DUALMEM_O3,
	DUALMEM_O4,
	DUALMEM_O5,
	DUALMEM_O6,
	DUALMEM_O7,
	DUALMEM_V0,
	DUALMEM_V1,
	DUALMEM_V2,
	DUALMEM_V3,
	DUALMEM_V4,
	DUALMEM_V5,
	DUALMEM_V6,
	DUALMEM_V7,
	DUALMEM_WRITE
};

BUILDING_TICK_DECLARE(multiplier)
enum {
	MULTIPLIER_A0,
	MULTIPLIER_A1,
	MULTIPLIER_A2,
	MULTIPLIER_A3,
	MULTIPLIER_A4,
	MULTIPLIER_A5,
	MULTIPLIER_A6,
	MULTIPLIER_A7,
	MULTIPLIER_A8,
	MULTIPLIER_A9,
	MULTIPLIER_A10,
	MULTIPLIER_A11,
	MULTIPLIER_A12,
	MULTIPLIER_A13,
	MULTIPLIER_A14,
	MULTIPLIER_A15,
	MULTIPLIER_B0,
	MULTIPLIER_B1,
	MULTIPLIER_B2,
	MULTIPLIER_B3,
	MULTIPLIER_B4,
	MULTIPLIER_B5,
	MULTIPLIER_B6,
	MULTIPLIER_B7,
	MULTIPLIER_B8,
	MULTIPLIER_B9,
	MULTIPLIER_B10,
	MULTIPLIER_B11,
	MULTIPLIER_B12,
	MULTIPLIER_B13,
	MULTIPLIER_B14,
	MULTIPLIER_B15,
	MULTIPLIER_O0,
	MULTIPLIER_O1,
	MULTIPLIER_O2,
	MULTIPLIER_O3,
	MULTIPLIER_O4,
	MULTIPLIER_O5,
	MULTIPLIER_O6,
	MULTIPLIER_O7,
	MULTIPLIER_O8,
	MULTIPLIER_O9,
	MULTIPLIER_O10,
	MULTIPLIER_O11,
	MULTIPLIER_O12,
	MULTIPLIER_O13,
	MULTIPLIER_O14,
	MULTIPLIER_O15,
	MULTIPLIER_O16,
	MULTIPLIER_O17,
	MULTIPLIER_O18,
	MULTIPLIER_O19,
	MULTIPLIER_O20,
	MULTIPLIER_O21,
	MULTIPLIER_O22,
	MULTIPLIER_O23,
	MULTIPLIER_O24,
	MULTIPLIER_O25,
	MULTIPLIER_O26,
	MULTIPLIER_O27,
	MULTIPLIER_O28,
	MULTIPLIER_O29,
	MULTIPLIER_O30,
	MULTIPLIER_O31,
};
