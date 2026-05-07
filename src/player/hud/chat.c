#include <stddef.h>
#include <stdio.h>

#include "../gfx/renderer.h"
#include "../global.h"
#include "../state.h"
#include "../world/tick.h"
#include "chat.h"

chat_message_t chat_messages[MAX_CHAT_MESSAGES] = {0};
size_t chat_count = 0;
size_t chat_head = 0;

char chat_input[CHAT_INPUT_MAX];
size_t chat_input_len = 0;
bool chat_active = false;

void render_chat() {
    for (size_t i = 0; i < chat_count; i++) {
        size_t start = (chat_head + MAX_CHAT_MESSAGES - chat_count) % MAX_CHAT_MESSAGES;
        size_t index = (start + i) % MAX_CHAT_MESSAGES;

        const chat_message_t *message = &chat_messages[index]; 

        if (message->formatted == NULL) continue;

        int y = state.renderer.height - 
            CHAT_MESSAGE_FONTSIZE * MAX_CHAT_MESSAGES +
            i * CHAT_MESSAGE_FONTSIZE - CHAT_MESSAGE_FONTSIZE;

        renderer_text(16, y, CHAT_MESSAGE_SCALE, message->formatted);
    }
}

void chat_handle_command(const char *text) {
    if (strncmp(text, "!tps", 4) == 0) {
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
    message->name = name;
    message->message = text;
    
    char *buffer = malloc(8096);
    if (buffer == NULL) {
        app_error("Failed to allocate chat buffer");
        return;
    }
    if (message->formatted) free(message->formatted);

    snprintf(buffer, 8096, "[%s]: %s", name, text);
    message->formatted = buffer;

    chat_head = (chat_head + 1) % MAX_CHAT_MESSAGES;
    if (chat_count < MAX_CHAT_MESSAGES) {
        chat_count++;
    }
}