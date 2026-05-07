#pragma once

typedef struct {
    int key;
    int blockid;
} block_keybind_t;

#define BLOCK_KEYBINDS_COUNT 8

extern block_keybind_t block_keybinds[];