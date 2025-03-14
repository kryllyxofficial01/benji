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

size_t strsplit(const char* string, char*** tokens, const char character) {
    if (!string) {
        return 0;
    }

    int string_length = strlen(string);
    size_t count = 0;

    for (int i = 0; i < string_length; i++) {
        if (string[i] == character) {
            count++;
        }
    }
    count++;

    if (string_length > 0 && string[string_length - 1] == character) {
        count--;
    }

    if (count == 0) {
        *tokens = NULL;
        return 0; // none of the specified character were found
    }

    *tokens = malloc(BENJI_CAPACITY(count, char*));
    if (*tokens == NULL) {
        return 0; // return 0 because we havent processed any tokens yet
    }

    int index = 0, start = 0;
    for (int i = 0; i < string_length; i++) {
        if (string[i] == character || string[i] == '\0') {
            if (start < i) {
                int token_length = i - start;

                (*tokens)[index] = malloc(BENJI_CAPACITY(token_length + 1, char));

                if ((*tokens)[index] == NULL) {
                    free((*tokens)[index]); // look at me, being responsible

                    return count; // return how ever many tokens we managed to get
                }

                strncpy((*tokens)[index], string + start, token_length);

                (*tokens)[index][token_length] = '\0';

                index++;
            }

            start = i + 1;
        }
    }

    return count;  // Return the number of tokens
}

#ifdef _WIN32
    char* wcharp_to_charp(const wchar_t* wchar) {
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
#endif