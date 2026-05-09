#pragma once

#define MAX_CHAT_MESSAGES 16
#define CHAT_INPUT_MAX 4096

#include <stddef.h>
#include <stdbool.h>

#include "../gfx/renderer.h"

typedef struct {
    const char *name;
    const char *message;
    char *formatted;
} chat_message_t;

extern chat_message_t chat_messages[MAX_CHAT_MESSAGES];
extern size_t chat_count;
extern size_t chat_head;

extern char chat_input[CHAT_INPUT_MAX];
extern size_t chat_input_len;
extern bool chat_active;

extern float chat_fontscale;
extern float chat_fontsize;
extern int chat_y_sub;
extern vec3 chat_color;

void render_chat(void);
void chat_init(void);
void chat_handle_command(const char *text);
void chat_add_message(
    const char *name,
    const char *text
);
void chat_cleanup(void);
