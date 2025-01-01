#ifndef __BENJI_JSON_H
#define __BENJI_JSON_H

#include "utils.h"

#define BENJI_JSON_INITIAL_CAPACITY 2 /* number of elements (keys + values) to allocate memory for at initiation */

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

char* json_create_block(json_t* json, const char* header);

void json_resize(json_t* json, size_t new_capacity);
void json_free(json_t* json);

#endif