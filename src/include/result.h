#ifndef __BENJI_RESULT_H
#define __BENJI_RESULT_h

#include <stdlib.h>
#include <stdbool.h>

typedef struct _BENJI_ERROR_PACKET {
    const char* file_name;
    const char* function_name;
    unsigned long lineno;
} error_packet_t;

typedef struct _BENJI_RESULT {
    bool is_error;

    union _RESULT_PAYLOAD {
        void* value;

        struct _RESULT_PAYLOAD_ERROR {
            int code;
            const char* message;

            error_packet_t location;
        } error;
    } payload;
} result_t;

#define BENJI_ERROR_PACKET (struct _BENJI_ERROR_PACKET) { .file_name = __FILE__, .function_name = __func__, .lineno = __LINE__ }

#define return_if_error(result) if (result->is_error) { \
    return result_error( \
        result->payload.error.code, \
        result->payload.error.message, \
        BENJI_ERROR_PACKET \
    ); \
}

result_t* result_init();

result_t* result_success(void* value);
result_t* result_error(int error_code, const char* message, error_packet_t location);

void* result_unwrap(result_t* result);

void result_free(result_t* result);

#endif