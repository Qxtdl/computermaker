#include <stddef.h>
#include <stdio.h>

#include "chat.h"
#include "../util.h"
#include "../config.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "../gfx/window.h"
#include "../world/tick.h"
#include "../world/save.h"
#include "../state.h"

chat_message_t chat_messages[MAX_CHAT_MESSAGES] = {0};
size_t chat_count = 0;
size_t chat_head = 0;

char chat_input[CHAT_INPUT_MAX];
size_t chat_input_len = 0;
bool chat_active = false;

block_command_t block_commands[] = {
    {"!air\0", AIR, 5},
    {"!stud\0", STUD, 6},
    {"!bk\0", BRICK, 4},
    {"!and\0", AND, 5},
    {"!or\0", OR, 4},
    {"!xor\0", XOR, 5},
    {"!nand\0", NAND, 6},
    {"!nor\0", NOR, 5},
    {"!xnor\0", XNOR, 6},
    {"!ff\0", FLIPFLOP, 4},
    {"!node\0", NODE, 6}
};
#define BLOCK_COMMANDS_COUNT (sizeof(block_commands) \
    / sizeof(block_commands[0]))

void chat_render(void) {
    for (size_t i = 0; i < chat_count; i++) {
        size_t start = (chat_head + MAX_CHAT_MESSAGES - chat_count) % MAX_CHAT_MESSAGES;
        size_t index = (start + i) % MAX_CHAT_MESSAGES;

        const chat_message_t *message = &chat_messages[index]; 

        if (message->formatted == NULL) continue;

        int y = window.height - 
            CHAT_MESSAGE_FONTSIZE * MAX_CHAT_MESSAGES +
            i * CHAT_MESSAGE_FONTSIZE - CHAT_MESSAGE_FONTSIZE;

        renderer_text(0, y, CHAT_MESSAGE_SCALE, message->formatted, (vec3){1, 0.5, 1});
    }
}

void chat_handle_command(char *text) {
    char buf[256];

    if (!strncmp(text, "!tps ", 5)) {
        int target = atoi(text + 5);

        if (target < 0) {
            chat_add_message("comm", "invalid tps");
            return;
        }

        tick_interval = 1.0 / target;

        snprintf(buf, sizeof(buf), "tps updated to interval %f", tick_interval);

        chat_add_message("comm", buf);
    }
    else if (!strncmp(text, "!save ", 6)) {
        const char *save_name = text + 6;

        save_save(save_name);

        snprintf(buf, sizeof(buf), "saved to %s", save_name);
        chat_add_message("comm", buf);
    }
    else if (!strncmp(text, "!setting ", 9)) {
        strtok(text, " ");
        char *key = strtok(NULL, " "),
             *value = strtok(NULL, " ");
        config_modify(key, value);
        snprintf(buf, sizeof(buf), "key %s changed to %s", key, value);
        chat_add_message("comm", buf);
    }    
    else if (!strncmp(text, "!system ", 8)) {
        FILE *fp = popen(text + 8, "r");
        if (!fp)
            return;

        while (fgets(buf, sizeof(buf), fp) != NULL) {
            chat_add_message("comm", buf); // print each line
        }

        pclose(fp);
    }
    else if (!strcmp(text, "!restart")) {
        state.restart = true;
    }  
    else if (!strcmp(text, "!wp")) {
        state.player.mode = MODE_WIRE_PLACE;
    }
    else if (!strcmp(text, "!wd")) {
        state.player.mode = MODE_WIRE_DESTROY;
    }
    for (int i = 0; i < BLOCK_COMMANDS_COUNT; i++) {
        block_command_t *bk_command = &block_commands[i];
        if (strncmp(text, bk_command->command, bk_command->len)) continue;
        state.player.selected_block = bk_command->gate;
        state.player.mode = MODE_BLOCK_PLACE;
    }

    return;
}

void chat_add_message(const char *name, const char *text) {
    chat_message_t *message = &chat_messages[chat_head];
    
    if (message->formatted) free(message->formatted);
    if (message->name) free((void*)message->name);
    if (message->message) free((void*)message->message);

    message->name = strdup(name);
    message->message = strdup(text);

    size_t buffer_size = snprintf(NULL, 0, "[%s]: %s", name, text) + 1;
    char *buffer = smalloc(buffer_size);
    snprintf(buffer, buffer_size, "[%s]: %s", name, text);

    message->formatted = buffer;

    chat_head = (chat_head + 1) % MAX_CHAT_MESSAGES;
    if (chat_count < MAX_CHAT_MESSAGES) {
        chat_count++;
    }
}

void chat_char_callback(unsigned int codepoint) {
    if (!chat_active) return;

    if (chat_input_len < CHAT_INPUT_MAX - 1) {
        chat_input[chat_input_len++] = (char)codepoint;
        chat_input[chat_input_len] = '\0';
    }    
}

void chat_cleanup(void) {
    for (size_t i = 0; i < MAX_CHAT_MESSAGES; i++) {
        free(chat_messages[i].formatted);
        free((void*)chat_messages[i].name);
        free((void*)chat_messages[i].message);
    }
}
