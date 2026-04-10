#pragma once

#include <string.h>
#include <cglm/cglm.h>

#define BLOCK_UV_DECLARE(name) extern void _##name##_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv);
#define BLOCK_UV_GET(name, block, face, scale, uv) _##name##_get_uv(block, face, scale, uv);

#define BLOCK_TICK_DECLARE(name) extern void _##name##_tick(block_t *block);

enum Face {
    FACE_TOP,
    FACE_BOTTOM,
    FACE_RIGHT,
    FACE_LEFT,
    FACE_FRONT,
    FACE_BACK,
    FACE_LAST
};

struct logic_gate;

typedef struct {
    struct logic_gate *gate;
} input_t;

typedef struct logic_gate {
    int num_inputs;
    input_t *inputs;
    enum LogicState {
        STATE_OFF,
        STATE_ON
    } state, new_state;
} logic_gate_t;

typedef struct {
    enum BlockId {
        AIR,
        STUD,
        BRICK,

        // logic gates
        AND,
        NAND,

        BLOCKID_LAST
    } id;
    logic_gate_t gate;
} block_t;

void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv);
void block_tick(block_t *block);
bool is_logic_block(block_t block);

BLOCK_UV_DECLARE(air)
BLOCK_UV_DECLARE(stud)
BLOCK_UV_DECLARE(brick)
BLOCK_UV_DECLARE(and)
BLOCK_UV_DECLARE(nand)

BLOCK_TICK_DECLARE(and)
BLOCK_TICK_DECLARE(nand)
