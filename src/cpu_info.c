#include "include/cpu_info.h"

cpu_info_t get_cpu_info() {
    cpu_info_t info;

    info.name = get_cpu_name();
    info.arch = get_cpu_arch();
    info.clock_speed = get_cpu_clock_speed();
    info.core_count = get_cpu_core_count();
    info.logical_processors_count = get_cpu_logical_processors_count();

    return info;
}

const char* get_cpu_name() {
    #if defined(_WIN32)
        int cpuid_info[4];
        char* cpu_name = malloc(0x40 * sizeof(char));

        memset(cpu_name, 0, sizeof(cpu_name));

        __cpuid(cpuid_info, 0x80000002);
        memcpy(cpu_name, cpuid_info, sizeof(cpuid_info));

        __cpuid(cpuid_info, 0x80000003);
        memcpy(cpu_name + 16, cpuid_info, sizeof(cpuid_info));

        __cpuid(cpuid_info, 0x80000004);
        memcpy(cpu_name + 32, cpuid_info, sizeof(cpuid_info));

        return cpu_name;
    #endif
}

const char* get_cpu_arch() {
    #if defined(_WIN32)
        SYSTEM_INFO system_info;

        GetSystemInfo(&system_info);

        switch (system_info.wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_AMD64: return "x64";
            case PROCESSOR_ARCHITECTURE_ARM: return "ARM";
            case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
            case PROCESSOR_ARCHITECTURE_IA64: return "IA-64";
            case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
            case PROCESSOR_ARCHITECTURE_UNKNOWN: return "???";
        }
    #endif
}

double get_cpu_clock_speed() {
    #if defined(_WIN32)
        return atof(wmi_get_data(WMI_WIN32_PROCESSOR, WMI_CPU_CLOCK_SPEED)) / 1000; // the command outputs MHz, we want GHz
    #endif
}

size_t get_cpu_core_count() {
    #if defined(_WIN32)
        return atoi(wmi_get_data(WMI_WIN32_PROCESSOR, WMI_CPU_CORE_COUNT));
    #endif
}

size_t get_cpu_logical_processors_count() {
    #if defined(_WIN32)
        return atoi(wmi_get_data(WMI_WIN32_PROCESSOR, WMI_CPU_LOGICAL_PROCESSORS_COUNT));
    #endif
}

json_t* serialize_cpu_info(cpu_info_t cpu_info) {
    json_t* cpu_info_json = json_init();

    char* buffer = malloc(1024 * sizeof(char));
    buffer[0] = '\0';

    json_insert(cpu_info_json, "cpu_name", cpu_info.name);

    json_insert(cpu_info_json, "cpu_arch", cpu_info.arch);

    sprintf(buffer, "%f", cpu_info.clock_speed);
    json_insert(cpu_info_json, "cpu_clock_speed", buffer);

    sprintf(buffer, "%li", cpu_info.core_count);
    json_insert(cpu_info_json, "cpu_core_count", buffer);

    sprintf(buffer, "%li", cpu_info.logical_processors_count);
    json_insert(cpu_info_json, "cpu_logical_processors_count", buffer);

    free(buffer);

    return cpu_info_json;
}