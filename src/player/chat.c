#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "chat.h"
#include "../util.h"
#include "../config.h"
#include "../gfx/renderer.h"
#include "../gfx/window.h"
#include "../world/tick.h"

chat_message_t chat_messages[MAX_CHAT_MESSAGES] = {0};
size_t chat_count = 0;
size_t chat_head = 0;

char chat_input[CHAT_INPUT_MAX];
size_t chat_input_len = 0;
bool chat_active = false;

float chat_fontscale = 2;
float chat_fontsize = 32;
int chat_y_sub = 1024;

void render_chat(void) {
    for (size_t i = 0; i < chat_count; i++) {
        size_t start = (chat_head + MAX_CHAT_MESSAGES - chat_count) % MAX_CHAT_MESSAGES;
        size_t index = (start + i) % MAX_CHAT_MESSAGES;

        const chat_message_t *message = &chat_messages[index]; 

        if (message->formatted == NULL) continue;

        int y = window.height - 
            chat_fontsize * MAX_CHAT_MESSAGES +
            i * chat_fontsize - chat_fontsize;

        renderer_text(0, y - chat_y_sub, chat_fontscale, message->formatted, NULL);
    }
}

void chat_init(void) {
    chat_y_sub = atoi(config_get("CHAT_Y_SUB"));
    chat_fontscale = atof(config_get("CHAT_FONTSCALE"));
    chat_fontsize = chat_fontscale * 16;
}

void chat_handle_command(const char *text) {
    if (strncmp(text, "!tps ", 5) == 0) {
        int target = atoi(text + 5);

        if (target < 0) {
            chat_add_message("comm", "invalid tps");
            return;
        }

        tick_interval = 1.0 / target;
        chat_add_message("comm", "tps updated");
        return;
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

void chat_cleanup(void) {
    for (size_t i = 0; i < MAX_CHAT_MESSAGES; i++) {
        free(chat_messages[i].formatted);
        free((void*)chat_messages[i].name);
        free((void*)chat_messages[i].message);
    }
}
