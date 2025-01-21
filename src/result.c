#include "include/result.h"

result_t* result_init() {
    result_t* result = malloc(sizeof(result_t));

    result->is_error = false;

    return result;
}

result_t* result_success(void* value) {
    result_t* result = result_init();

    result->is_error = false;
    result->payload.value = value;

    return result;
}

result_t* result_error(int error_code, const char* error_message) {
    result_t* result = result_init();

    result->is_error = true;

    result->payload.error.code = error_code;
    result->payload.error.error_message = error_message;

    return result;
}

void* result_unwrap(result_t* result) {
    void* value = result->payload.value;

    result_free(result);

    return value;
}

void result_free(result_t* result) {
    free(result);
}