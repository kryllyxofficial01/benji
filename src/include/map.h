#ifndef __BENJI_MAP_H
#define __BENJI_MAP_H

#include "utils.h"

#define BENJI_MAP_INITIAL_CAPACITY 2 /* number of elements (keys + values) to allocate memory for at initiation */

#define BENJI_MAP_KEY_NOT_FOUND -1

typedef struct BENJIMAP {
    char** keys;
    char** values;

    size_t size;
    size_t capacity;
} map_t;

map_t* map_init();

void map_insert(map_t* map, const char* key, const char* value);
const char* map_get(map_t* map, const char* key);
int map_get_index_of(map_t* map, const char* key);

char* map_serialize(map_t* map, const char* header);

void map_resize(map_t* map, size_t new_capacity);
void map_free(map_t* map);

#endif