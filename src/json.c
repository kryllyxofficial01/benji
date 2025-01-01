#include "include/json.h"

json_t* json_init() {
    json_t* json = malloc(sizeof(json_t));

    json->keys = malloc(BENJI_CAPACITY(BENJI_JSON_INITIAL_CAPACITY, char*));
    json->values = malloc(BENJI_CAPACITY(BENJI_JSON_INITIAL_CAPACITY, char*));

    json->size = 0;
    json->capacity = BENJI_JSON_INITIAL_CAPACITY;

    return json;
}

void json_insert(json_t* json, const char* key, const char* value) {
    int index = json_get_index_of(json, key);

    if (index != BENJI_JSON_KEY_NOT_FOUND) {
        free(json->values[index]);

        json->values[index] = strdup(value);
    }
    else {
        if(json->size == json->capacity) {
            json_resize(json, json->capacity * 2); // key + value = 2 new elements to be allocated for
        }

        json->keys[json->size] = strdup(key);
        json->values[json->size] = strdup(value);

        json->size++;
    }
}

const char* json_get(json_t* json, const char* key) {
    int index = json_get_index_of(json, key);

    if (index == BENJI_JSON_KEY_NOT_FOUND) {
        return NULL;
    }

    return json->values[index];
}

int json_get_index_of(json_t* json, const char* key) {
    for (size_t i = 0; i < json->size; i++) {
        if (strcmp(json->keys[i], key) == 0) {
            return i;
        }
    }

    return BENJI_JSON_KEY_NOT_FOUND;
}

char* json_create_block(json_t* json, const char* header) {
    char* block = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH * json->capacity, char));
    char* buffer = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    block[0] = '\0';
    buffer[0] = '\0';

    sprintf(buffer, "\"%s\": {", header);
    strcat(block, buffer);

    size_t i = 0;
    for (i; i < json->size - 1; i++) {
        sprintf(buffer, "\"%s\": \"%s\",", json->keys[i], json->values[i]);
        strcat(block, buffer);
    }

    sprintf(buffer, "\"%s\": \"%s\"", json->keys[i], json->values[i]);
    strcat(block, buffer);

    strcat(block, "}");

    return block;
}

void json_resize(json_t* json, size_t new_capacity) {
    json->keys = realloc(json->keys, BENJI_CAPACITY(new_capacity, char*));
    json->values = realloc(json->values, BENJI_CAPACITY(new_capacity, char*));

    json->capacity = new_capacity;
}

void json_free(json_t* json) {
    for (size_t i = 0; i < json->size; i++) {
        free(json->keys[i]);
        free(json->values[i]);
    }

    free(json->keys);
    free(json->values);

    free(json);
}