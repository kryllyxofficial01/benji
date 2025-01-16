#ifndef __BENJI_RESULT_H
#define __BENJI_RESULT_h

#include <stdlib.h>
#include <stdbool.h>

typedef struct BENJIRESULT {
    bool is_error;

    union _PAYLOAD {
        void* value;

        struct _ERROR {
            int code;
            const char* error_message;
        } error;
    } payload;
} result_t;

result_t* result_init();

result_t* result_success(void* value);
result_t* result_error(int error_code, const char* error_message);

void* result_unwrap(result_t* result);

void result_free(result_t* result);

#endif