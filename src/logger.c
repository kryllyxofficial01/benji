#include "include/logger.h"

/**
 * @todo Before writing the log information to a log file, strip the text of
 * any leading or trailing whitespace that was added to pretty-up the stdout pipe
 */

void log_debug(const char* string, ...) {
    va_list arguments;

    va_start(arguments, string);

    char* output = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    if (!output) {
        return;
    }

    output[0] = '\0';

    vsprintf(output, string, arguments);
    strtrim(output);
    // fprintf(stdout, "\n");

    OutputDebugStringA(output);

    free(output);

    va_end(arguments);
}

void log_info(const char* info, ...) {
    va_list arguments;

    va_start(arguments, info);

    // fprintf(stdout, ANSI_CYAN);
    // vfprintf(stdout, info, arguments);
    // fprintf(stdout, ANSI_COLOR_RESET "\n");
    // fprintf(stdout, "\n");

    char* output = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    if (!output) {
        return;
    }

    output[0] = '\0';

    vsprintf(output, info, arguments);
    strtrim(output);

    OutputDebugStringA(output);

    free(output);

    va_end(arguments);
}

void log_warning(result_error_payload_t error) {
    // fprintf(
    //     stderr,
    //     ANSI_YELLOW "%s:%i under %s() -> %s (%i)\n" ANSI_COLOR_RESET,
    //     error.location.file_name,
    //     error.location.lineno,
    //     error.location.function_name,
    //     error.message,
    //     error.code
    // );

    char* output = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    if (!output) {
        return;
    }

    output[0] = '\0';

    sprintf(
        output,
        "%s:%i under %s() -> %s (%i)\n",
        error.location.file_name,
        error.location.lineno,
        error.location.function_name,
        error.message,
        error.code
    );

    OutputDebugStringA(output);

    free(output);
}

void log_warning_info(const char* info, ...) {
    va_list arguments;

    va_start(arguments, info);

    // fprintf(stdout, ANSI_YELLOW);
    // vfprintf(stdout, info, arguments);
    // fprintf(stdout, ANSI_COLOR_RESET "\n");
    // fprintf(stdout, "\n");

    char* output = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    if (!output) {
        return;
    }

    output[0] = '\0';

    vsprintf(output, info, arguments);
    strtrim(output);

    OutputDebugStringA(output);

    free(output);

    va_end(arguments);
}

void log_error(result_error_payload_t error) {
    // fprintf(
    //     stderr,
    //     ANSI_RED "[FATAL] %s:%i under %s() -> %s (%i)\n" ANSI_COLOR_RESET,
    //     error.location.file_name,
    //     error.location.lineno,
    //     error.location.function_name,
    //     error.message,
    //     error.code
    // );

    char* output = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    if (!output) {
        return;
    }

    output[0] = '\0';

    sprintf(
        output,
        "[FATAL] %s:%i under %s() -> %s (%i)\n",
        error.location.file_name,
        error.location.lineno,
        error.location.function_name,
        error.message,
        error.code
    );

    OutputDebugStringA(output);

    free(output);
}