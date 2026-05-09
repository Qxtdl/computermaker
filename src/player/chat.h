#pragma once

#define MAX_CHAT_MESSAGES 48
#define CHAT_MESSAGE_SCALE 2
#define CHAT_MESSAGE_FONTSIZE (CHAT_MESSAGE_SCALE * 16)
#define CHAT_INPUT_MAX 4096

#include <stddef.h>
#include <stdbool.h>

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

void chat_render(void);
void chat_handle_command(const char *text);
void chat_add_message(
    const char *name,
    const char *text
);
void chat_char_callback(unsigned int codepoint);
void chat_cleanup(void);
