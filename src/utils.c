#include "include/utils.h"

void strtrim(char* string) {
    int i = 0;
    while (isspace(string[i])) {
        i++;
    }
    if (i > 0) {
        memmove(string, string + i, strlen(string) - i + 1);
    }

    int length = strlen(string);
    while (length > 0 && isspace(string[length - 1])) {
        length--;
    }
    string[length] = '\0';
}

size_t splitstr(const char* string, char*** tokens, const char* character) {
    char* temp_string = strdup(string);

    if (temp_string == NULL) {
        return 0;
    }

    size_t token_count = 0;
    char* token = strtok(temp_string, character);

    while (token != NULL) {
        char** temp_tokens = realloc(*tokens, BENJI_CAPACITY(token_count + 1, char*));

        if (temp_tokens == NULL) {
            free(temp_tokens);

            break;
        }

        *tokens = temp_tokens;

        (*tokens)[token_count] = strdup(token);

        if ((*tokens)[token_count] == NULL) {
            break;
        }

        token_count++;
        token = strtok(NULL, character);
    }

    free(temp_string);

    return token_count;
}

void write_to_file(const char* filepath, const char* data) {
    FILE* file = fopen(filepath, "w");

    if (file != NULL) {
        fprintf(file, "%s\n", data);
    }

    fclose(file);
}