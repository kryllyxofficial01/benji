#include "include/logger.h"

void log_debug(const char* string, ...) {
    va_list arguments;

    va_start(arguments, string);

    vfprintf(stdout, string, arguments);
    fprintf(stdout, "\n");

    va_end(arguments);
}

void log_info(const char* info, ...) {
    va_list arguments;

    va_start(arguments, info);

    fprintf(stdout, CYAN);
    vfprintf(stdout, info, arguments);
    fprintf(stdout, COLOR_RESET "\n");

    va_end(arguments);
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

void log_warning_info(const char* info, ...) {
    va_list arguments;

    va_start(arguments, info);

    fprintf(stdout, YELLOW);
    vfprintf(stdout, info, arguments);
    fprintf(stdout, COLOR_RESET "\n");

    va_end(arguments);
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