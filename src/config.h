#pragma once

void config_open(const char *filename);
void config_process(void);
const char *config_get(const char *key);
void config_modify(const char *key, const char *value);
