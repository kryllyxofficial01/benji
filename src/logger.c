#include "include/logger.h"

void log_debug(const char* string) {
    fprintf(stdout, "%s\n", string);
}

void log_info(const char* info) {
    fprintf(stdout, CYAN "%s\n" COLOR_RESET, info);
}

void log_warning(result_t* result) {
    fprintf(
        stderr, YELLOW "%s:%d under %s() -> %s (%i)\n" COLOR_RESET,

        result->payload.error.location.file_name,
        result->payload.error.location.lineno,
        result->payload.error.location.function_name,

        result->payload.error.message,
        result->payload.error.code
    );
}

void log_error(result_t* result) {
    fprintf(
        stderr, RED "%s:%d under %s() -> %s (%i)\n" COLOR_RESET,

        result->payload.error.location.file_name,
        result->payload.error.location.lineno,
        result->payload.error.location.function_name,

        result->payload.error.message,
        result->payload.error.code
    );
}