#include "state.h"

struct state state = {0};

const char *mode_name(enum mode mode) {
    switch (mode) {
        case MODE_BLOCK_PLACE: return "BLOCK_PLACE";
        case MODE_WIRE_PLACE: return "WIRE_PLACE";
        case MODE_WIRE_DESTROY: return "WIRE_DESTROY";
        case MODE_BLOCK_POKE: return "BLOCK_POKE";
        case MODE_BLOCK_HOVER: return "BLOCK_HOVER";
        default: return "UNKNOWN";
    }
}
