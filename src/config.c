/*
 * config.c
 *
 * this will help getting the values stored in a configuration file
 * for example, to decide which skybox to use
*/

#include <stdio.h>
#include <string.h>

#include "global.h"
#include "util.h"

static char *consource;

void config_open(const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (!fptr) {
        app_error("Failed to find/open the config file %s\n", filename)
    }

    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    rewind(fptr);
    consource = smalloc(size + 1);
    size_t read_size = fread(consource, 1, size, fptr);
    if (read_size != size) {
        app_error("Error whilst reading the config file %s\n", filename)
    }
    consource[size] = '\0';
}

static struct key_value {
    const char *key;
    const char *value;
} *tables = NULL;
size_t tables_size;

static void config_add_key_value(const char *key, const char *value) {
    tables = srealloc(tables, ++tables_size * sizeof(struct key_value));
    tables[tables_size - 1] = (struct key_value){
        key,
        value
    };
    printf("[CONFIG]: Key-value pair: [%s : %s]\n", key, value);
}

void config_process(void) {
    char *save, *save2;

    char *line = strtok_r(consource, "\n", &save);
    while (line != NULL) {
        if (*line == ';') goto skip;
        char *key = strtok_r(line, "=", &save2);
        char *value = strtok_r(NULL, "=", &save2);
        config_add_key_value(key, value);
    skip:
        line = strtok_r(NULL, "\n", &save);
    }
}

const char *config_get(const char *key) {
    for (size_t i = 0; i < tables_size; i++) {
        if (!strcmp(tables[i].key, key)) {
            printf("[CONFIG]: Got %s!\n", tables[i].value);
            return tables[i].value;
        }
    }
    app_error("The value in the config \"%s\" could not be found\n", key)
}

