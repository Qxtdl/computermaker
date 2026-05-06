/*
 * cm2save.c
 *
 * load cm2 savestrings into the world
*/

#include <stddef.h>
#include <stdlib.h>

#include "../vec.h"
#include "../state.h"
#include "global.h"
#include "util.h"
#include "world/block/block.h"
#include "world/wire.h"
#include "world/world.h"

/*
  TODO: code formatting
*/

typedef struct {const char *start; size_t size;} StringView;

StringView cstr2SV(const char *cstr) {
    StringView sv;
    sv.start = cstr;
    size_t i = 0;
    while (cstr[i]) {
        i++;
    }
    sv.size = i - 1;
    return sv;
}

void SV_chop_left(StringView *sv, size_t n) {
    if (n > sv->size) n = sv->size;
    sv->start += n;
    sv->size -= n;
}

StringView SV_chop_by_delim(StringView *sv, char delim) {
    size_t i = 0;
    while (i < sv->size && sv->start[i] != delim) i++;
    if (i < sv->size) {
        StringView result = {
            .start = sv->start,
            .size = i,
        };
        SV_chop_left(sv, i + 1);
        return result;
    }
    StringView result = *sv;
    SV_chop_left(sv, sv->size);
    return result;
}

struct {
    int block_count;
    int block_capacity;
    ivec3 *blocks;
} cm2_blocks = {
    .block_count = 0,
    .block_capacity = 0,
    .blocks = NULL
};

static void cm2save_add_block(StringView sv_block) {
    if (cm2_blocks.blocks == NULL) {
        cm2_blocks.blocks = smalloc(100 * sizeof(ivec3));
        cm2_blocks.block_capacity = 100;
    }
    if (++cm2_blocks.block_count > cm2_blocks.block_capacity) {
        cm2_blocks.blocks = srealloc(cm2_blocks.blocks, (cm2_blocks.block_capacity << 1) * sizeof(ivec3));
        cm2_blocks.block_capacity <<= 1;
    }
    block_t block;
    StringView sv_block_type = SV_chop_by_delim(&sv_block, ',');
    StringView sv_block_state = SV_chop_by_delim(&sv_block, ',');
    StringView sv_block_x = SV_chop_by_delim(&sv_block, ',');
    StringView sv_block_y = SV_chop_by_delim(&sv_block, ',');
    StringView sv_block_z = SV_chop_by_delim(&sv_block, ',');
    char buf[10];
    size_t i = 0;
    for (; i<9 && i<sv_block_type.size; i++) {
        buf[i] = sv_block_type.start[i];
    }
    buf[++i] = '\0';
    switch (atoi(buf)) {
        case 0://NOR
            block.id = NOR;
            break;
        case 1://AND
            block.id = AND;
            break;
        case 2://OR
            block.id = OR;
            break;
        case 3://XOR
            block.id = XOR;
            break;
        case 4://BUTTON
            app_warn("button not implemented\n");
            break;
        case 5://FLIPFLOP
            block.id = FLIPFLOP;
            break;
        case 6://LED
            app_warn("led not implemented\n");
            break;
        case 7://SOUND
            app_warn("sound not implemented\n");
            break;
        case 8://CONDUCTOR
            app_warn("conductor not implemented\n");
            break;
        case 9://CUSTOM
            app_warn("custom not implemented\n");
            break;
        case 10://NAND
            block.id = NAND;
            break;
        case 11://XNOR
            block.id = XNOR;
            break;
        case 12://RANDOM
            app_warn("random not implemented\n");
            break;
        case 13://TEXT
            app_warn("text not implemented\n");
            break;
        case 14://TILE
            app_warn("tile not implemented\n");
            break;
        case 15://NODE
            block.id = NODE;
            break;
        case 16://DELAY
            app_warn("delay not implemented\n");
            break;
        case 17://ANTENNA
            app_warn("antenna not implemented\n");
            break;
        case 18://CONDUCTORV2
            app_warn("conductorv2 not implemented\n");
            break;
        case 19://LEDMIXER
            app_warn("ledmixer not implemented\n");
            break;
    }
    block.gate.inputs = NULL;
    block.gate.num_inputs = 0;
    if (sv_block_state.size == 0) {
        block.gate.state = STATE_OFF;
    } else if (*sv_block_type.start == '1') {
        block.gate.state = STATE_ON;
    } else {
        block.gate.state = STATE_OFF;
    }
    block.gate.new_state = STATE_OFF;
    i = 0;
    for (; i<9 && i<sv_block_x.size; i++) {
        buf[i] = sv_block_x.start[i];
    }
    buf[++i] = '\0';
    int block_x = atoi(buf);
    for (; i<9 && i<sv_block_y.size; i++) {
        buf[i] = sv_block_y.start[i];
    }
    buf[++i] = '\0';
    int block_y = atoi(buf);
    for (; i<9 && i<sv_block_z.size; i++) {
        buf[i] = sv_block_z.start[i];
    }
    buf[++i] = '\0';
    int block_z = atoi(buf);
    cm2_blocks.blocks[cm2_blocks.block_count - 1][0] = block_x;
    cm2_blocks.blocks[cm2_blocks.block_count - 1][1] = block_y;
    cm2_blocks.blocks[cm2_blocks.block_count - 1][2] = block_z;
    world_place_at(&state.world, block_x, block_y, block_z, block);
}

static void cm2save_add_wire(StringView wire) {
    wire_t new_wire;
    StringView sv_source = SV_chop_by_delim(&wire, ',');
    char buf[10];
    size_t i = 0;
    for (; i<9 && i<sv_source.size; i++) {
        buf[i] = sv_source.start[i];
    }
    buf[++i] = '\0';
    ivec3 source = {
        cm2_blocks.blocks[atoi(buf)][0],
        cm2_blocks.blocks[atoi(buf)][1],
        cm2_blocks.blocks[atoi(buf)][2]
    };

    i = 0;
    for (; i<9 && i<wire.size; i++) {
        buf[i] = wire.start[i];
    }
    buf[++i] = '\0';
    ivec3 dest = {
        cm2_blocks.blocks[atoi(buf)][0],
        cm2_blocks.blocks[atoi(buf)][1],
        cm2_blocks.blocks[atoi(buf)][2]
    };

    new_wire.ox = source[0];
    new_wire.oy = source[1];
    new_wire.oz = source[2];
    new_wire.dx = dest[0];
    new_wire.dy = dest[1];
    new_wire.dz = dest[2];

    world_create_wire(new_wire);
}

void cm2save_process(const char *savestring) {
	if (!savestring) {
		return;
	}
	
    StringView sv_savestring = cstr2SV(savestring);
    StringView sv_blocks = SV_chop_by_delim(&sv_savestring, '?');
    StringView sv_connections = SV_chop_by_delim(&sv_savestring, '?');
    while (sv_blocks.size > 0) {
        cm2save_add_block(SV_chop_by_delim(&sv_blocks, ';'));
    }
    while (sv_connections.size > 0) {
        cm2save_add_wire(SV_chop_by_delim(&sv_connections, ';'));
    }
    free(cm2_blocks.blocks);
}
