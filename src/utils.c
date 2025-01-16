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

size_t splitstr(const char* string, char*** tokens, const char character) {
    int string_length = strlen(string);
    size_t count = 0;

    for (int i = 0; i < string_length; i++) {
        if (string[i] == character) {
            count++;
        }
    }
    count++;

    *tokens = (char**) malloc(BENJI_CAPACITY(count, char*));
    if (*tokens == NULL) {
        return 0; // return 0 cause we havent processed any tokens yet
    }

    int index = 0, start = 0;
    for (int i = 0; i < string_length; i++) {
        if (string[i] == character) {
            int token_length = i - start;

            (*tokens)[index] = (char*) malloc(BENJI_CAPACITY(token_length + 1, char));

            if ((*tokens)[index] == NULL) {
                return count; // return how ever many tokens we may have gotten
            }

            strncpy((*tokens)[index], string + start, token_length);

            (*tokens)[index][token_length] = '\0';

            start = i + 1;

            index++;
        }
    }

    return count;  // Return the number of tokens
}

char* wcharp_to_charp(const WCHAR* wchar) {
    if (wchar == NULL) {
        return NULL;
    }

    int size = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, NULL, 0, NULL, NULL);

    if (size == 0) {
        return NULL;
    }

    char* string = malloc(size);

    if (string == NULL) {
        return NULL;
    }

    WideCharToMultiByte(CP_UTF8, 0, wchar, -1, string, size, NULL, NULL);

    return string;
}

void write_to_file(const char* filepath, const char* data) {
    FILE* file = fopen(filepath, "w");

    if (file != NULL) {
        fprintf(file, "%s\n", data);
    }

    fclose(file);
}