#include <GLFW/glfw3.h>

#include "../world/block/block.h"
#include "keybinds.h"

block_keybind_t block_keybinds[] = {
    { GLFW_KEY_1, AND },
    { GLFW_KEY_2, OR },
    { GLFW_KEY_3, XOR },
    { GLFW_KEY_4, NAND },
    { GLFW_KEY_5, NOR },
    { GLFW_KEY_6, XNOR },
    { GLFW_KEY_7, FLIPFLOP },
    { GLFW_KEY_8, NODE }
};

const int BLOCK_KEYBINDS_COUNT = sizeof(block_keybinds) / sizeof(block_keybinds[0]);
