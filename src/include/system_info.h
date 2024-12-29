#ifndef __BENJI_SYSTEM_INFO_H
#define __BENJI_SYSTEM_INFO_H

#include <stdio.h>

#define BENJI_NO_SERVER_INCLUDES // we dont need all the server stuff in utils.h

#include "utils.h"

#if defined(_WIN32)
    #include <windows.h>
    #include <intrin.h>
    #include <unistd.h>

    enum WMI_ENTRY {
        WMI_CLOCK_SPEED,
        WMI_CORE_COUNT,
        WMI_LOGICAL_PROCESSORS_COUNT,
    };

    const char* wmi_get_data(enum WMI_ENTRY entry);
    const char* wmi_entry_to_string(enum WMI_ENTRY entry);
#endif

typedef struct CPUINFO {
    const char* name;
    const char* arch;
    double clock_speed; // in GHz
    unsigned int core_count;
    unsigned int logical_processors_count;
    // double current_temp; // TBD until i figure out how
} cpu_info_t;

cpu_info_t get_cpu_info();
const char* get_cpu_name();
const char* get_cpu_arch();
double get_cpu_clock_speed();
unsigned int get_cpu_core_count();
unsigned int get_cpu_logical_processors_count();

#endif