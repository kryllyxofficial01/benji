#ifndef __BENJI_CPU_INFO_H
#define __BENJI_CPU_INFO_H

#define BENJI_USE_SYS_INFO_UTILS

#include "utils.h"

#if defined(_WIN32)
    #include <intrin.h>
    #include <unistd.h>
#endif

typedef struct _BENJI_CPU_INFO {
    char* name;
    char* vendor;
    char* arch;
    double clock_speed; // in GHz
    size_t core_count;
    size_t logical_processors_count;
    // double current_temp; // TBD until i figure out how
} cpu_info_t;

result_t* get_cpu_info();

result_t* get_cpu_name();
result_t* get_cpu_vendor();
result_t* get_cpu_arch();
result_t* get_cpu_clock_speed();
result_t* get_cpu_core_count();
result_t* get_cpu_logical_processors_count();

#ifdef _WIN32
    result_t* get_cpu_processor_info(processor_info_callback_t callback);
    uint32_t count_cpu_cores(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info);
    uint32_t count_cpu_logical_processors(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info);
#endif

map_t* cpu_info_to_map(cpu_info_t cpu_info);

#endif