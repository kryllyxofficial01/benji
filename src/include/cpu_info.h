#ifndef __BENJI_CPU_INFO_H
#define __BENJI_CPU_INFO_H

#define BENJI_USE_SYS_INFO_UTILS

#include "utils.h"

#include "map.h"

#if defined(_WIN32)
    #include <windows.h>
    #include <intrin.h>
    #include <unistd.h>
#endif

typedef struct BENJICPUINFO {
    char* name;
    char* vendor;
    char* arch;
    double clock_speed; // in GHz
    size_t core_count;
    size_t logical_processors_count;
    // double current_temp; // TBD until i figure out how
} cpu_info_t;

cpu_info_t get_cpu_info();

char* get_cpu_name();
char* get_cpu_vendor();
char* get_cpu_arch();
double get_cpu_clock_speed();
int get_cpu_core_count();
int get_cpu_logical_processors_count();

#ifdef _WIN32
    static int get_cpu_processor_info(processor_info_callback_t callback);
    static DWORD count_cpu_cores(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info);
    static DWORD count_cpu_logical_processors(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info);
#endif

map_t* cpu_info_to_map(cpu_info_t cpu_info);

#endif