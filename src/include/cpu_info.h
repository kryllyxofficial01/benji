#ifndef __BENJI_CPU_INFO_H
#define __BENJI_CPU_INFO_H

#define BENJI_NO_SERVER_INCLUDES // we dont need all the server stuff in utils.h

#include "utils.h"

#if defined(_WIN32)
    #include <windows.h>
    #include <intrin.h>
    #include <unistd.h>
#endif

typedef struct CPUINFO {
    const char* name;
    const char* arch;
    double clock_speed; // in GHz
    size_t core_count;
    size_t logical_processors_count;
    // double current_temp; // TBD until i figure out how
} cpu_info_t;

cpu_info_t get_cpu_info();

const char* get_cpu_name();
const char* get_cpu_arch();
double get_cpu_clock_speed();
size_t get_cpu_core_count();
size_t get_cpu_logical_processors_count();

#endif