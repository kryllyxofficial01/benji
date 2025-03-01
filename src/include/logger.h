#ifndef __BENJI_LOGGER_H
#define __BENJI_LOGGER_H

#include "utils.h"

void log_error(result_t* result) {
    fprintf(
        stdout, "\033[31m%s:%d [%s] -> %s (%i)\033[0m\n",

        result->payload.error.location.file_name,
        result->payload.error.location.lineno,
        result->payload.error.location.function_name,

        result->payload.error.message,
        result->payload.error.code
    );
}

#endif