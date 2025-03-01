#ifndef __BENJI_LOGGER_H
#define __BENJI_LOGGER_H

#include <stdarg.h>

#include "utils.h"
#include "colors.h"

void log_debug(const char* string, ...);
void log_info(const char* info, ...);
void log_warning(result_error_payload_t error);
void log_warning_info(const char* info, ...);
void log_error(result_error_payload_t error);

#endif