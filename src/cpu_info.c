#include "include/cpu_info.h"

result_t* get_cpu_info() {
    cpu_info_t* info = malloc(sizeof(cpu_info_t));

    info->name = strdup(get_cpu_name());
    strtrim(info->name);

    info->vendor = strdup(get_cpu_vendor());
    strtrim(info->vendor);

    info->arch = strdup(get_cpu_arch());
    strtrim(info->name);

    info->clock_speed = get_cpu_clock_speed();
    info->core_count = get_cpu_core_count();
    info->logical_processors_count = get_cpu_logical_processors_count();

    return result_success(info);
}

char* get_cpu_name() {
    #if defined(_WIN32)
        int cpuid_info[BENJI_CPUID_BUFFER_LENGTH];

        char* cpu_name = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
        cpu_name[0] = '\0';

        for (int i = 0; i < BENJI_CPUID_CPU_NAME_SECTIONS_COUNT; ++i) {
            __cpuid(cpuid_info, BENJI_CPUID_CPU_NAME_START + i);
            memcpy(cpu_name + (i * 16), cpuid_info, sizeof(cpuid_info));
        }

        return cpu_name;
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

char* get_cpu_vendor() {
    #if defined(_WIN32)
        int cpu_info[BENJI_CPUID_BUFFER_LENGTH];

        char* cpu_vendor = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
        cpu_vendor[0] = '\0';

        __cpuid(cpu_info, 0);

        *((int*) cpu_vendor) = cpu_info[1];
        *((int*) (cpu_vendor + 4)) = cpu_info[3];
        *((int*) (cpu_vendor + 8)) = cpu_info[2];

        cpu_vendor[strlen(cpu_vendor) - 2] = '\0';

        return cpu_vendor;
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

char* get_cpu_arch() {
    #if defined(_WIN32)
        SYSTEM_INFO system_info;

        GetSystemInfo(&system_info);

        switch (system_info.wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
            case PROCESSOR_ARCHITECTURE_AMD64: return "x64";
            case PROCESSOR_ARCHITECTURE_ARM: return "ARM";
            case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
            case PROCESSOR_ARCHITECTURE_IA64: return "IA-64";
            case PROCESSOR_ARCHITECTURE_UNKNOWN: return "??";
        }
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

double get_cpu_clock_speed() {
    #if defined(_WIN32)
        HKEY hkey;

        LONG result = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0, KEY_READ, &hkey
        );

        if (result == BENJI_NO_ERROR) {
            DWORD speed = 0;
            DWORD data_type, data_size = sizeof(speed);

            result = RegQueryValueEx(
                hkey, "~MHz", NULL, &data_type, (LPBYTE) &speed, &data_size
            );

            RegCloseKey(hkey);

            return (result == BENJI_NO_ERROR && data_type == REG_DWORD) ? ((double) speed) / 1000 : -1.0;
        }
        else {
            return -1.0;
        }
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

int get_cpu_core_count() {
    #if defined(_WIN32)
       return get_cpu_processor_info(count_cpu_cores);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

int get_cpu_logical_processors_count() {
    #if defined(_WIN32)
        return get_cpu_processor_info(count_cpu_logical_processors);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

#ifdef _WIN32
    int get_cpu_processor_info(processor_info_callback_t callback) {
        DWORD length = 0;

        GetLogicalProcessorInformation(NULL, &length);

        SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) malloc(length);

        if (!buffer) {
            return -1;
        }

        if (!GetLogicalProcessorInformation(buffer, &length)) {
            free(buffer);

            return -1;
        }

        DWORD result = 0;

        size_t count = length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        for (size_t i = 0; i < count; i++) {
            if (buffer[i].Relationship == RelationProcessorCore) {
                result += callback(&buffer[i]);
            }
        }

        free(buffer);

        return result;
    }

    DWORD count_cpu_cores(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info) {
        return 1;
    }

    DWORD count_cpu_logical_processors(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info) {
        return __popcnt(info->ProcessorMask);
    }
#endif

map_t* cpu_info_to_map(cpu_info_t cpu_info) {
    map_t* cpu_info_map = map_init();

    char* buffer = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
    buffer[0] = '\0';

    map_insert(cpu_info_map, "cpu_name", cpu_info.name);
    map_insert(cpu_info_map, "cpu_vendor", cpu_info.vendor);
    map_insert(cpu_info_map, "cpu_arch", cpu_info.arch);

    sprintf(buffer, "%0.3f", cpu_info.clock_speed);
    map_insert(cpu_info_map, "cpu_clock_speed", buffer);

    sprintf(buffer, "%lli", cpu_info.core_count);
    map_insert(cpu_info_map, "cpu_core_count", buffer);

    sprintf(buffer, "%lli", cpu_info.logical_processors_count);
    map_insert(cpu_info_map, "cpu_logical_processors_count", buffer);

    free(buffer);

    return cpu_info_map;
}