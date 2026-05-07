#pragma once

typedef struct {
    int key;
    int blockid;
} block_keybind_t;

extern block_keybind_t block_keybinds[];
extern const int BLOCK_KEYBINDS_COUNT; // born to be macro, forced to be variable