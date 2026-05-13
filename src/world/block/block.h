#pragma once

#include <string.h>
#include <cglm/cglm.h>

#include "../blockmesh.h"

#define BLOCK_TICK_DECLARE(name) extern void _##name##_tick(block_t *block);

struct logic_gate;

typedef struct {
    struct logic_gate *gate;
} input_t;

typedef struct logic_gate {
    int num_inputs;
    int inputs_size;
    input_t *inputs;
    enum LogicState {
        STATE_OFF,
        STATE_ON
    } state, new_state;
    bool poked;
} logic_gate_t;

typedef struct {
    enum BlockId {
        AIR,
        STUD,
        BRICK,

        // logic gates
        AND,
        OR,
        XOR,
        NAND,
        NOR,
        XNOR,
        FLIPFLOP,
        NODE,

        BLOCKID_LAST,

        BUILDING_PIN,
    } id;
    logic_gate_t gate;
} block_t;

const char *block_id_name(enum BlockId id);
void block_get_uv(block_t block, enum Face face, vec2 *scale, vec2 *uv);
void block_tick(block_t *block);
bool is_logic_block(block_t block);
void logic_block_add_input(block_t *from, block_t *to);
void logic_block_remove_input(block_t *from, block_t *to);

BLOCK_TICK_DECLARE(and)
BLOCK_TICK_DECLARE(or)
BLOCK_TICK_DECLARE(xor)
BLOCK_TICK_DECLARE(nand)
BLOCK_TICK_DECLARE(nor)
BLOCK_TICK_DECLARE(xnor)
BLOCK_TICK_DECLARE(flipflop)
BLOCK_TICK_DECLARE(node)
BLOCK_TICK_DECLARE(building_pin)
