/*
 * config.c
 *
 * this will help getting the values stored in a configuration file
 * for example, to decide which skybox to use
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "util.h"

static char *consource;

void config_open(const char *filename) {
    consource = readfile(filename);
}

static struct key_value {
    const char *key;
    char *value;
    bool modified;
} *tables = NULL;
int tables_size;

static void config_add_key_value(const char *key, char *value) {
    tables = srealloc(tables, ++tables_size * sizeof(struct key_value));
    tables[tables_size - 1] = (struct key_value){
        key,
        value,
        false
    };
    app_log("[CONFIG]: Key-value pair: [%s : %s]\n", key, value);
}

void config_process(void) {
    char *save, *save2;

    char *line = strtok_r(consource, "\n", &save);
    while (line != NULL) {
        if (*line == ';') goto skip;
        char *key = strtok_r(line, "=", &save2);
        char *value = strtok_r(NULL, "=", &save2);
        if (!strcmp(value, "null")) value = NULL;
        config_add_key_value(key, value);
    skip:
        line = strtok_r(NULL, "\n", &save);
    }
}

const char *config_get(const char *key) {
    for (size_t i = 0; i < tables_size; i++) {
        if (!strcmp(tables[i].key, key)) {
            return tables[i].value;
        }
    }
    
    app_error("The value in the config \"%s\" could not be found\n", key)
}

void config_modify(const char *key, const char *value) {
   	for (int i = 0; i < tables_size; i++) {
   		if (!strcmp(tables[i].key, key)) {
   			if (tables[i].modified) {
   				free(tables[i].value);
   			}
   			tables[i].value = smalloc(strlen(value) + 1);
   			strcpy(tables[i].value, value);
            tables[i].modified = true;
   		}
   	}
}
