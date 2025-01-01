#ifndef __BENJI_JSON_H
#define __BENJI_JSON_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"

#define BENJI_JSON_INITIAL_CAPACITY 1 /* number of key-value pairs to allocate memory for at initiation */

#define BENJI_JSON_KEY_NOT_FOUND -1

typedef struct BENJIJSON {
    char** keys;
    char** values;

    size_t size;
    size_t capacity;
} json_t;

json_t* json_init();

void json_insert(json_t* json, const char* key, const char* value);
const char* json_get(json_t* json, const char* key);

int json_get_index_of(json_t* json, const char* key);

void json_resize(json_t* json, size_t new_capacity);
void json_free(json_t* json);

#endif