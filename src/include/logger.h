#ifndef __BENJI_LOGGER_H
#define __BENJI_LOGGER_H

#include "utils.h"
#include "colors.h"

void log_debug(const char* string);
void log_info(const char* info);
void log_warning(result_t* result);
void log_error(result_t* result);

#endif