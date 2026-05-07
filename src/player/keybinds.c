#include <GLFW/glfw3.h>

#include "../world/block/block.h"
#include "keybinds.h"

block_keybind_t block_keybinds[] = {
    { GLFW_KEY_1, NOR },
    { GLFW_KEY_Y, AND },
    { GLFW_KEY_C, OR },
    { GLFW_KEY_4, XOR },
    // { GLFW_KEY_R, BUTTON },
    { GLFW_KEY_G, FLIPFLOP },
    // { GLFW_KEY_H, LED },
    // { GLFW_KEY_8, SOUND },
    // { GLFW_KEY_9, CONDUCTOR },
    { GLFW_KEY_MINUS, NAND },
    { GLFW_KEY_EQUAL, XNOR },
    // { GLFW_KEY_M, TEXT },
    { GLFW_KEY_SEMICOLON, STUD }
};

const int BLOCK_KEYBINDS_COUNT = sizeof(block_keybinds) / sizeof(block_keybinds[0]);