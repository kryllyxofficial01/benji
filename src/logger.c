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

void log_warning(result_error_payload_t error) {
    fprintf(
        stderr, YELLOW "%s:%i under %s() -> %s (%i)\n" COLOR_RESET,
        error.location.file_name,
        error.location.lineno,
        error.location.function_name,
        error.message,
        error.code
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

void log_error(result_error_payload_t error) {
    fprintf(
        stderr, RED "%s:%i under %s() -> %s (%i)\n" COLOR_RESET,
        error.location.file_name,
        error.location.lineno,
        error.location.function_name,
        error.message,
        error.code
    );
}