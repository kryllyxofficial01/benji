#include "include/cpu_info.h"

result_t* get_cpu_info() {
    cpu_info_t* info = malloc(sizeof(cpu_info_t));

    info->name = strdup((char*) result_unwrap(get_cpu_name()));
    strtrim(info->name);

    info->vendor = strdup((char*) result_unwrap(get_cpu_vendor()));
    strtrim(info->vendor);

    info->arch = strdup((char*) result_unwrap(get_cpu_arch()));
    strtrim(info->name);

    info->clock_speed = *(double*) result_unwrap(get_cpu_clock_speed());
    info->core_count = (size_t) (uintptr_t) result_unwrap(get_cpu_core_count());
    info->logical_processors_count = (size_t) (uintptr_t) result_unwrap(get_cpu_logical_processors_count());

    return result_success(info);
}

result_t* get_cpu_name() {
    #if defined(_WIN32)
        int cpuid_info[BENJI_CPUID_BUFFER_LENGTH];

        char* cpu_name = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
        cpu_name[0] = '\0';

        for (int i = 0; i < BENJI_CPUID_CPU_NAME_SECTIONS_COUNT; ++i) {
            __cpuid(cpuid_info, BENJI_CPUID_CPU_NAME_START + i);
            memcpy(cpu_name + (i * 16), cpuid_info, sizeof(cpuid_info));
        }

        return result_success(cpu_name);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_cpu_vendor() {
    #if defined(_WIN32)
        int cpu_info[BENJI_CPUID_BUFFER_LENGTH];

        char* cpu_vendor = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
        cpu_vendor[0] = '\0';

        __cpuid(cpu_info, 0);

        *((int*) cpu_vendor) = cpu_info[1];
        *((int*) (cpu_vendor + 4)) = cpu_info[3];
        *((int*) (cpu_vendor + 8)) = cpu_info[2];

        cpu_vendor[strlen(cpu_vendor) - 2] = '\0'; // this is kinda cursed, but it works sooooo

        return result_success(cpu_vendor);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_cpu_arch() {
    #if defined(_WIN32)
        SYSTEM_INFO system_info;

        GetSystemInfo(&system_info);

        char* arch;

        switch (system_info.wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_INTEL: arch = "x86"; break;
            case PROCESSOR_ARCHITECTURE_AMD64: arch = "x64"; break;
            case PROCESSOR_ARCHITECTURE_ARM: arch = "ARM"; break;
            case PROCESSOR_ARCHITECTURE_ARM64: arch = "ARM64"; break;
            case PROCESSOR_ARCHITECTURE_IA64: arch = "IA-64"; break;

            case PROCESSOR_ARCHITECTURE_UNKNOWN: // make this one fall through because i dont wanna deal with edge cases
            default: arch = "??"; break;
        }

        return result_success(arch);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_cpu_clock_speed() {
    #if defined(_WIN32)
        HKEY hkey;

        LONG result = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0, KEY_READ, &hkey
        );

        if (result == BENJI_NO_ERROR) {
            uint32_t speed = 0;
            unsigned long int data_type, data_size = sizeof(speed);

            result = RegQueryValueEx(
                hkey, "~MHz", NULL, &data_type, (LPBYTE) &speed, &data_size
            );

            RegCloseKey(hkey);

            if (result == BENJI_NO_ERROR && data_type == REG_DWORD) {
                void* speed_ghz = malloc(sizeof(double));

                *(double*) speed_ghz = speed / 1000.0;

                return result_success(speed_ghz);
            }
            else {
                return result_error(result, "RegQueryValueEx() failed");
            }
        }
        else {
            return result_error(result, "RegOpenKeyEx() failed");
        }
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_cpu_core_count() {
    #if defined(_WIN32)
        return get_cpu_processor_info(count_cpu_cores);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_cpu_logical_processors_count() {
    #if defined(_WIN32)
        return get_cpu_processor_info(count_cpu_logical_processors);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

#ifdef _WIN32
    result_t* get_cpu_processor_info(processor_info_callback_t callback) {
        unsigned long int length = 0;

        GetLogicalProcessorInformation(NULL, &length);

        SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) malloc(length);

        if (!buffer) {
            return result_error(-1, "buffer is NULL");
        }

        if (!GetLogicalProcessorInformation(buffer, &length)) {
            free(buffer);

            return result_error(-1, "GetLogicalProcessorInformation() failed");
        }

        uint32_t result = 0;

        size_t count = length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        for (size_t i = 0; i < count; i++) {
            if (buffer[i].Relationship == RelationProcessorCore) {
                result += callback(&buffer[i]);
            }
        }

        free(buffer);

        return result_success((void*) (uintptr_t) result);
    }

    uint32_t count_cpu_cores(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info) {
        return 1;
    }

    uint32_t count_cpu_logical_processors(SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info) {
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