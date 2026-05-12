/*
 * cm2save.c
 *
 * load cm2 savestrings into the world
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../state.h"
#include "gfx/camera.h"
#include "global.h"
#include "util.h"
#include "world/block/block.h"
#include "world/wire.h"
#include "world/world.h"
#include "world/building/building.h"

/*
  TODO: code formatting
*/

typedef struct { const char *start; size_t size; } stringview_t;

stringview_t cstr2SV(const char *cstr) {
    stringview_t sv;
    sv.start = cstr;
    size_t i = 0;
    while (cstr[i]) {
        i++;
    }
    sv.size = i;
    return sv;
}

stringview_t SV_chop_left(stringview_t *sv, size_t n) {
    if (n > sv->size) n = sv->size;
    stringview_t sv_ret = {.start = sv->start, .size = n};
    sv->start += n;
    sv->size -= n;
    return sv_ret;
}

stringview_t SV_chop_by_delim(stringview_t *sv, char delim) {
    size_t i = 0;
    while (i < sv->size && sv->start[i] != delim) i++;
    if (i < sv->size) {
        stringview_t result = {
            .start = sv->start,
            .size = i,
        };
        SV_chop_left(sv, i + 1);
        return result;
    }
    stringview_t result = *sv;
    SV_chop_left(sv, sv->size);
    return result;
}

bool SV_cmp_cstr(stringview_t sv, const char *cstr) {
    for (size_t i = 0; i < sv.size; i++) {
        if (sv.start[i]^cstr[i]) {return false;}
        if (!cstr[i]) {return false;}
    }
    return true;
}

int SV_to_int(stringview_t sv) {
    char buf[12];
    size_t i = 0;
    for (; i<9 && i<sv.size; i++) {
        buf[i] = sv.start[i];
    }
    buf[i] = '\0';
    return atoi(buf);
}

void SV_print(stringview_t sv) {
    printf("%.*s",(int)sv.size,sv.start);
}

struct ivec3 {int x,y,z;};

struct {
    int block_count;
    int block_capacity;
    struct ivec3 *blocks;
} cm2_blocks = {
    .block_count = 0,
    .block_capacity = 0,
    .blocks = NULL
};

static void cm2save_add_block(stringview_t sv_block) {
    if (cm2_blocks.blocks == NULL) {
        cm2_blocks.blocks = smalloc(100 * sizeof(struct ivec3));
        cm2_blocks.block_capacity = 100;
    }
    if (++cm2_blocks.block_count > cm2_blocks.block_capacity) {
        cm2_blocks.blocks = srealloc(cm2_blocks.blocks, (cm2_blocks.block_capacity << 1) * sizeof(struct ivec3));
        cm2_blocks.block_capacity <<= 1;
    }
    block_t block;
    stringview_t sv_block_type = SV_chop_by_delim(&sv_block, ',');
    stringview_t sv_block_state = SV_chop_by_delim(&sv_block, ',');
    stringview_t sv_block_x = SV_chop_by_delim(&sv_block, ',');
    stringview_t sv_block_y = SV_chop_by_delim(&sv_block, ',');
    stringview_t sv_block_z = SV_chop_by_delim(&sv_block, ',');
    switch (SV_to_int(sv_block_type)) {
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
            block.id = BRICK;        
            app_warn("button not implemented\n");
            break;
        case 5://FLIPFLOP
            block.id = FLIPFLOP;
            break;
        case 6://LED
            block.id = BRICK;        
            app_warn("led not implemented\n");
            break;
        case 7://SOUND
            block.id = BRICK;        
            app_warn("sound not implemented\n");
            break;
        case 8://CONDUCTOR
            block.id = BRICK;        
            app_warn("conductor not implemented\n");
            break;
        case 9://CUSTOM
            block.id = BRICK;        
            app_warn("custom not implemented\n");
            break;
        case 10://NAND
            block.id = NAND;
            break;
        case 11://XNOR
            block.id = XNOR;
            break;
        case 12://RANDOM
            block.id = BRICK;
            app_warn("random not implemented\n");
            break;
        case 13://TEXT
            block.id = BRICK;
            app_warn("text not implemented\n");
            break;
        case 14://TILE
            block.id = BRICK;        
            app_warn("tile not implemented\n");
            break;
        case 15://NODE
            block.id = OR;
            break;
        case 16://DELAY
            block.id = BRICK;        
            app_warn("delay not implemented\n");
            break;
        case 17://ANTENNA
            block.id = BRICK;        
            app_warn("antenna not implemented\n");
            break;
        case 18://CONDUCTORV2
            block.id = BRICK;        
            app_warn("conductorv2 not implemented\n");
            break;
        case 19://LEDMIXER
            block.id = BRICK;        
            app_warn("ledmixer not implemented\n");
            break;
    }
    block.gate.inputs = NULL;
    block.gate.num_inputs = 0;
    block.gate.inputs_size = 0;
    if (sv_block_state.size == 0) {
        block.gate.state = STATE_OFF;
    } else if (*sv_block_type.start == '1') {
        block.gate.state = STATE_ON;
    } else {
        block.gate.state = STATE_OFF;
    }
    block.gate.new_state = STATE_OFF;
    int block_x = SV_to_int(sv_block_x);
    int block_y = SV_to_int(sv_block_y);
    int block_z = SV_to_int(sv_block_z);
    cm2_blocks.blocks[cm2_blocks.block_count - 1].x = block_x;
    cm2_blocks.blocks[cm2_blocks.block_count - 1].y = block_y;
    cm2_blocks.blocks[cm2_blocks.block_count - 1].z = block_z;
    world_place_at(&state.world, block_x, block_y, block_z, block);
}

static void cm2save_add_wire(stringview_t sv_wire) {
    stringview_t sv_source = SV_chop_by_delim(&sv_wire, ',');
    int source_id = SV_to_int(sv_source)-1;
    struct ivec3 source = {
        cm2_blocks.blocks[source_id].x,
        cm2_blocks.blocks[source_id].y,
        cm2_blocks.blocks[source_id].z
    };
    int dest_id = SV_to_int(sv_wire)-1;
    struct ivec3 dest = {
        cm2_blocks.blocks[dest_id].x,
        cm2_blocks.blocks[dest_id].y,
        cm2_blocks.blocks[dest_id].z
    };

    world_create_wire((wire_t){
        .ox = source.x,
        .oy = source.y,
        .oz = source.z,
        .dx = dest.x,
        .dy = dest.y,
        .dz = dest.z
    });
}


static struct ivec3 cm2save_get_building_pin_pos(building_t building, int pin_id) {
    int x=0,y=0,z=0;
    switch (building.id) {
        case HUGE_MEMORY:
            if (pin_id < 16) {
                switch (pin_id) {
                    case  0:x = 17; y = 0; z = -3;break;
                    case  1:x = 8; y = 0; z = -3;break;
                    case  2:x = 7; y = 0; z = -3;break;
                    case  3:x = 6; y = 0; z = -3;break;
                    case  4:x = 5; y = 0; z = -3;break;
                    case  5:x = 4; y = 0; z = -3;break;
                    case  6:x = 3; y = 0; z = -3;break;
                    case  7:x = 2; y = 0; z = -3;break;
                    case  8:x = 16; y = 0; z = -3;break;
                    case  9:x = 15; y = 0; z = -3;break;
                    case 10:x = 14; y = 0; z = -3;break;
                    case 11:x = 13; y = 0; z = -3;break;
                    case 12:x = 12; y = 0; z = -3;break;
                    case 13:x = 11; y = 0; z = -3;break;
                    case 14:x = 10; y = 0; z = -3;break;
                    case 15:x = 9; y = 0; z = -3;break;
                }
            } else if (pin_id < 32) {
                switch (pin_id - 16) {
                    case  0:x = 17; y = 0; z = 3;break;
                    case  1:x = 8; y = 0; z = 3;break;
                    case  2:x = 7; y = 0; z = 3;break;
                    case  3:x = 6; y = 0; z = 3;break;
                    case  4:x = 5; y = 0; z = 3;break;
                    case  5:x = 4; y = 0; z = 3;break;
                    case  6:x = 3; y = 0; z = 3;break;
                    case  7:x = 2; y = 0; z = 3;break;
                    case  8:x = 16; y = 0; z = 3;break;
                    case  9:x = 15; y = 0; z = 3;break;
                    case 10:x = 14; y = 0; z = 3;break;
                    case 11:x = 13; y = 0; z = 3;break;
                    case 12:x = 12; y = 0; z = 3;break;
                    case 13:x = 11; y = 0; z = 3;break;
                    case 14:x = 10; y = 0; z = 3;break;
                    case 15:x = 9; y = 0; z = 3;break;
                }
            } else if (pin_id < 48) {
                switch (pin_id - 32) {
                    case  0:x = -1; y = 0; z = -3;break;
                    case  1:x = -10; y = 0; z = -3;break;
                    case  2:x = -11; y = 0; z = -3;break;
                    case  3:x = -12; y = 0; z = -3;break;
                    case  4:x = -13; y = 0; z = -3;break;
                    case  5:x = -14; y = 0; z = -3;break;
                    case  6:x = -15; y = 0; z = -3;break;
                    case  7:x = -16; y = 0; z = -3;break;
                    case  8:x = -2; y = 0; z = -3;break;
                    case  9:x = -3; y = 0; z = -3;break;
                    case 10:x = -4; y = 0; z = -3;break;
                    case 11:x = -5; y = 0; z = -3;break;
                    case 12:x = -6; y = 0; z = -3;break;
                    case 13:x = -7; y = 0; z = -3;break;
                    case 14:x = -8; y = 0; z = -3;break;
                    case 15:x = -9; y = 0; z = -3;break;
                }
            } else {x = -18; y = 0; z = -3;}
            break;
        case MULTIPLIER:
            if (pin_id < 16) {
                switch (pin_id) {
                    case  0:x =   1; y = 0; z = -7;break;
                    case  1:x =  -8; y = 0; z = -7;break;
                    case  2:x =  -9; y = 0; z = -7;break;
                    case  3:x = -10; y = 0; z = -7;break;
                    case  4:x = -11; y = 0; z = -7;break;
                    case  5:x = -12; y = 0; z = -7;break;
                    case  6:x = -13; y = 0; z = -7;break;
                    case  7:x = -14; y = 0; z = -7;break;
                    case  8:x =   0; y = 0; z = -7;break;
                    case  9:x =  -1; y = 0; z = -7;break;
                    case 10:x =  -2; y = 0; z = -7;break;
                    case 11:x =  -3; y = 0; z = -7;break;
                    case 12:x =  -4; y = 0; z = -7;break;
                    case 13:x =  -5; y = 0; z = -7;break;
                    case 14:x =  -6; y = 0; z = -7;break;
                    case 15:x =  -7; y = 0; z = -7;break;
                }
            } else if (pin_id < 32) {
                switch (pin_id - 16) {
                    case  0:x = -17; y = 0; z = -7;break;
                    case  1:x = -26; y = 0; z = -7;break;
                    case  2:x = -27; y = 0; z = -7;break;
                    case  3:x = -28; y = 0; z = -7;break;
                    case  4:x = -29; y = 0; z = -7;break;
                    case  5:x = -30; y = 0; z = -7;break;
                    case  6:x = -31; y = 0; z = -7;break;
                    case  7:x = -32; y = 0; z = -7;break;
                    case  8:x = -18; y = 0; z = -7;break;
                    case  9:x = -19; y = 0; z = -7;break;
                    case 10:x = -20; y = 0; z = -7;break;
                    case 11:x = -21; y = 0; z = -7;break;
                    case 12:x = -22; y = 0; z = -7;break;
                    case 13:x = -23; y = 0; z = -7;break;
                    case 14:x = -24; y = 0; z = -7;break;
                    case 15:x = -25; y = 0; z = -7;break;
                }
            } else if (pin_id < 48) {
                switch (pin_id - 32) {
                    case  0:x =   1; y = 0; z = 1;break;
                    case  1:x =  -8; y = 0; z = 1;break;
                    case  2:x =  -9; y = 0; z = 1;break;
                    case  3:x = -10; y = 0; z = 1;break;
                    case  4:x = -11; y = 0; z = 1;break;
                    case  5:x = -12; y = 0; z = 1;break;
                    case  6:x = -13; y = 0; z = 1;break;
                    case  7:x = -14; y = 0; z = 1;break;
                    case  8:x =   0; y = 0; z = 1;break;
                    case  9:x =  -1; y = 0; z = 1;break;
                    case 10:x =  -2; y = 0; z = 1;break;
                    case 11:x =  -3; y = 0; z = 1;break;
                    case 12:x =  -4; y = 0; z = 1;break;
                    case 13:x =  -5; y = 0; z = 1;break;
                    case 14:x =  -6; y = 0; z = 1;break;
                    case 15:x =  -7; y = 0; z = 1;break;
                }
            } else {
                switch (pin_id - 48) {
                    case  0:x = -17; y = 0; z = 1;break;
                    case  1:x = -26; y = 0; z = 1;break;
                    case  2:x = -27; y = 0; z = 1;break;
                    case  3:x = -28; y = 0; z = 1;break;
                    case  4:x = -29; y = 0; z = 1;break;
                    case  5:x = -30; y = 0; z = 1;break;
                    case  6:x = -31; y = 0; z = 1;break;
                    case  7:x = -32; y = 0; z = 1;break;
                    case  8:x = -18; y = 0; z = 1;break;
                    case  9:x = -19; y = 0; z = 1;break;
                    case 10:x = -20; y = 0; z = 1;break;
                    case 11:x = -21; y = 0; z = 1;break;
                    case 12:x = -22; y = 0; z = 1;break;
                    case 13:x = -23; y = 0; z = 1;break;
                    case 14:x = -24; y = 0; z = 1;break;
                    case 15:x = -25; y = 0; z = 1;break;
                }
            }
            break;
        case DUAL_MEMORY:
            if (pin_id < 8) {
                x = 4 - pin_id;
                y = 0;
                z = -2;
            } else if (pin_id < 16) {
                x = 13 - (pin_id - 8);
                y = 0;
                z = -2;
            } else if (pin_id < 24) {
                x = 13 - (pin_id - 16);
                y = 0;
                z = 2;
            } else if (pin_id < 32) {
                x = -5 - (pin_id - 24);
                y = 0;
                z = -2;
            } else {
                x = -14;
                y = 0;
                z = -2;
            }
            break;
        default:break;
    }
    int rx, ry, rz;
    switch (building.rotation) {
        case ROTATION_FRONT: rx = building.x + x; ry = building.y + y; rz = building.z + z; break;
        case ROTATION_LEFT: rx = building.x + z; ry = building.y + y; rz = building.z - x; break;
        case ROTATION_BACK: rx = building.x - x; ry = building.y + y; rz = building.z - z; break;
        case ROTATION_RIGHT: rx = building.x - z; ry = building.y + y; rz = building.z + x; break;
        default: rx = building.x + x, ry = building.y + y, rz = building.z + z;
    }
    return (struct ivec3){rx, ry, rz};
}

static void cm2save_add_building(stringview_t sv_building) {
    stringview_t sv_building_type = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_building_x = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_building_y = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_building_z = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_00 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_10 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_20 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_01 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_11 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_21 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_02 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_12 = SV_chop_by_delim(&sv_building, ',');
    stringview_t sv_cframe_22 = SV_chop_by_delim(&sv_building, ',');
    int building_x = SV_to_int(sv_building_x);
    int building_y = SV_to_int(sv_building_y);
    int building_z = SV_to_int(sv_building_z);
    
    int building_rotation;
    if (
        SV_cmp_cstr(sv_cframe_00, "0")&&
        SV_cmp_cstr(sv_cframe_10, "0")&&
        SV_cmp_cstr(sv_cframe_20, "1")&&
        SV_cmp_cstr(sv_cframe_01, "0")&&
        SV_cmp_cstr(sv_cframe_11, "1")&&
        SV_cmp_cstr(sv_cframe_21, "0")&&
        SV_cmp_cstr(sv_cframe_02, "-1")&&
        SV_cmp_cstr(sv_cframe_12, "0")&&
        SV_cmp_cstr(sv_cframe_22, "0")
    ) {
        building_rotation = CAMERA_DIRECTION_FORWARD;
    } else if (
        SV_cmp_cstr(sv_cframe_00, "0")&&
        SV_cmp_cstr(sv_cframe_10, "0")&&
        SV_cmp_cstr(sv_cframe_20, "-1")&&
        SV_cmp_cstr(sv_cframe_01, "0")&&
        SV_cmp_cstr(sv_cframe_11, "1")&&
        SV_cmp_cstr(sv_cframe_21, "0")&&
        SV_cmp_cstr(sv_cframe_02, "1")&&
        SV_cmp_cstr(sv_cframe_12, "0")&&
        SV_cmp_cstr(sv_cframe_22, "0")
    ) {
        building_rotation = CAMERA_DIRECTION_BACK;
    } else if (
        SV_cmp_cstr(sv_cframe_00, "-1")&&
        SV_cmp_cstr(sv_cframe_10, "0")&&
        SV_cmp_cstr(sv_cframe_20, "0")&&
        SV_cmp_cstr(sv_cframe_01, "0")&&
        SV_cmp_cstr(sv_cframe_11, "1")&&
        SV_cmp_cstr(sv_cframe_21, "0")&&
        SV_cmp_cstr(sv_cframe_02, "0")&&
        SV_cmp_cstr(sv_cframe_12, "0")&&
        SV_cmp_cstr(sv_cframe_22, "-1")
    ) {
        building_rotation = CAMERA_DIRECTION_LEFT;
    } else if (
        SV_cmp_cstr(sv_cframe_00, "1")&&
        SV_cmp_cstr(sv_cframe_10, "0")&&
        SV_cmp_cstr(sv_cframe_20, "0")&&
        SV_cmp_cstr(sv_cframe_01, "0")&&
        SV_cmp_cstr(sv_cframe_11, "1")&&
        SV_cmp_cstr(sv_cframe_21, "0")&&
        SV_cmp_cstr(sv_cframe_02, "0")&&
        SV_cmp_cstr(sv_cframe_12, "0")&&
        SV_cmp_cstr(sv_cframe_22, "1")
    ) {
        building_rotation = CAMERA_DIRECTION_RIGHT;
    } else {
        app_warn("invalid building rotation\n")
        building_rotation = CAMERA_DIRECTION_FORWARD;
    }

    int pins_count = 0;
    building_t building;
    if (SV_cmp_cstr(sv_building_type, "HugeMemory")) {
        pins_count = 49;
        building = (building_t){
            .id = HUGE_MEMORY,
            .x = building_x,
            .y = building_y,
            .z = building_z,
            .rotation = building_rotation
        };
    } else if (SV_cmp_cstr(sv_building_type, "DualMemory")) {
        pins_count = 33;
        building = (building_t){
            .id = DUAL_MEMORY,
            .x = building_x,
            .y = building_y,
            .z = building_z,
            .rotation = building_rotation
        };
    } else if (SV_cmp_cstr(sv_building_type, "Multiplier")) {
        pins_count = 64;
        building = (building_t){
            .id = MULTIPLIER,
            .x = building_x,
            .y = building_y,
            .z = building_z,
            .rotation = building_rotation
        };
    } else {
        app_warn("building: ")
        SV_print(sv_building_type);
        printf(" not implemented\n");
        return;
    }
    building_create(building);
    for (size_t i = 0; i < pins_count; i++) {
        stringview_t sv_connections = SV_chop_by_delim(&sv_building, ',');
        struct ivec3 pin_ptr = cm2save_get_building_pin_pos(building, i);
        while (sv_connections.size > 0) {
            stringview_t sv_connection = SV_chop_by_delim(&sv_connections, '+');
            char start_ch = *SV_chop_left(&sv_connection, 1).start;
            struct ivec3 block_ptr = cm2_blocks.blocks[SV_to_int(sv_connection)-1];
            if (start_ch == '0') {//output
                world_create_wire((wire_t){
                    .ox = pin_ptr.x,
                    .oy = pin_ptr.y,
                    .oz = pin_ptr.z,
                    .dx = block_ptr.x,
                    .dy = block_ptr.y,
                    .dz = block_ptr.z
                });
            } else if (start_ch == '1') {//input
                world_create_wire((wire_t){
                    .ox = block_ptr.x,
                    .oy = block_ptr.y,
                    .oz = block_ptr.z,
                    .dx = pin_ptr.x,
                    .dy = pin_ptr.y,
                    .dz = pin_ptr.z
                });
            }
        }
    }
}

void cm2save_process(const char *savestring) {
	if (!savestring) {
		return;
	}
	
    stringview_t sv_savestring = cstr2SV(savestring);
    stringview_t sv_blocks = SV_chop_by_delim(&sv_savestring, '?');
    stringview_t sv_connections = SV_chop_by_delim(&sv_savestring, '?');
    stringview_t sv_buildings = SV_chop_by_delim(&sv_savestring, '?');
    while (sv_blocks.size > 0) {
        cm2save_add_block(SV_chop_by_delim(&sv_blocks, ';'));
    }
    while (sv_connections.size > 0) {
        cm2save_add_wire(SV_chop_by_delim(&sv_connections, ';'));
    }
    while (sv_buildings.size > 0) {
        cm2save_add_building(SV_chop_by_delim(&sv_buildings, ';'));
    }
    free(cm2_blocks.blocks);
    cm2_blocks.blocks = NULL;
    cm2_blocks.block_count = 0;
    cm2_blocks.block_capacity = 0;
}
