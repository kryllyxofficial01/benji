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
    // char* temp_string = strdup(string);

    // if (temp_string == NULL) {
    //     return 0;
    // }

    // size_t token_count = 0;
    // char* token = strtok(temp_string, character);

    // while (token != NULL) {
    //     char** temp_tokens = realloc(*tokens, BENJI_CAPACITY(token_count + 1, char*));

    //     if (temp_tokens == NULL) {
    //         free(temp_tokens);

    //         break;
    //     }

    //     *tokens = temp_tokens;

    //     (*tokens)[token_count] = strdup(token);

    //     if ((*tokens)[token_count] == NULL) {
    //         break;
    //     }

    //     token_count++;
    //     token = strtok(NULL, character);
    // }

    // free(temp_string);

    // return token_count;

    int len = strlen(string);
    int start = 0, count = 0;

    // Count the number of tokens by counting occurrences of the delimiter
    for (int i = 0; i < len; i++) {
        if (string[i] == character) {
            count++;
        }
    }

    // The number of tokens is the number of delimiters + 1
    count++;

    // Allocate memory for the array of token pointers
    *tokens = (char **)malloc(count * sizeof(char *));
    if (*tokens == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i <= len; i++) {
        if (string[i] == character || string[i] == '\0') {
            int tokenLen = i - start;
            (*tokens)[index] = (char *)malloc((tokenLen + 1) * sizeof(char));
            if ((*tokens)[index] == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            strncpy((*tokens)[index], string + start, tokenLen);
            (*tokens)[index][tokenLen] = '\0'; // Null-terminate the token
            start = i + 1;
            index++;
        }
    }

    return count;  // Return the number of tokens
}

void write_to_file(const char* filepath, const char* data) {
    FILE* file = fopen(filepath, "w");

    if (file != NULL) {
        fprintf(file, "%s\n", data);
    }

    fclose(file);
}