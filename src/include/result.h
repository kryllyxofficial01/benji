#ifndef __BENJI_RESULT_H
#define __BENJI_RESULT_h

#include <stdlib.h>
#include <stdbool.h>

typedef struct _BENJI_RESULT {
    bool is_error;

    union _RESULT_PAYLOAD {
        void* value;

        struct _RESULT_PAYLOAD_ERROR {
            int code;
            const char* error_message;
        } error;
    } payload;
} result_t;

#define return_if_error(result) \
    if (result->is_error) { \
        return result_error( \
            result->payload.error.code, \
            result->payload.error.error_message \
        ); \
    }

result_t* result_init();

result_t* result_success(void* value);
result_t* result_error(int error_code, const char* error_message);

void* result_unwrap(result_t* result);

void result_free(result_t* result);

#endif