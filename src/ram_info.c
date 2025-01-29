#include "include/ram_info.h"

result_t* get_ram_info() {
    ram_info_t* info = malloc(sizeof(ram_info_t));

    info->total_memory = *(double*) result_unwrap(get_ram_total_memory());
    info->memory_load = *(double*) result_unwrap(get_ram_memory_load());
    info->free_memory = *(double*) result_unwrap(get_ram_free_memory());

    return result_success(info);
}

result_t* get_ram_total_memory() {
    #if defined(_WIN32)
        MEMORYSTATUSEX* status = (MEMORYSTATUSEX*) result_unwrap(get_memory_status());

        void* memory = malloc(sizeof(double));

        *(double*) memory = status->ullTotalPhys / (1024.0 * 1024.0 * 1024.0);

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_ram_memory_load() {
    #if defined(_WIN32)
        MEMORYSTATUSEX* status = (MEMORYSTATUSEX*) result_unwrap(get_memory_status());

        double total = *(double*) result_unwrap(get_ram_total_memory());
        double percent = status->dwMemoryLoad / 100.0; // dwMemoryLoad returns a percent

        void* memory = malloc(sizeof(double));

        *(double*) memory = total * percent; // total is already in GB, so no need to convert

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_ram_free_memory() {
    #if defined(_WIN32)
        MEMORYSTATUSEX* status = (MEMORYSTATUSEX*) result_unwrap(get_memory_status());

        void* memory = malloc(sizeof(double));

        *(double*) memory = status->ullAvailPhys / (1024.0 * 1024.0 * 1024.0);

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

#ifdef _WIN32
    result_t* get_memory_status() {
        MEMORYSTATUSEX* status = malloc(sizeof(MEMORYSTATUSEX));

        status->dwLength = sizeof(MEMORYSTATUSEX);

        // for whatever stupid reason, it returns zero upon erroring
        if (!GlobalMemoryStatusEx(status)) {
            free(status);

            return result_error(GetLastError(), "GlobalMemoryStatusEx() failed");
        }

        return result_success((void*) status);
    }
#endif

map_t* ram_info_to_map(ram_info_t ram_info) {
    map_t* ram_info_map = map_init();

    char* buffer = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
    buffer[0] = '\0';

    sprintf(buffer, "%0.3f", ram_info.total_memory);
    map_insert(ram_info_map, "ram_total_memory", buffer);

    sprintf(buffer, "%0.3f", ram_info.memory_load);
    map_insert(ram_info_map, "ram_memory_load", buffer);

    sprintf(buffer, "%0.3f", ram_info.free_memory);
    map_insert(ram_info_map, "ram_free_memory", buffer);

    free(buffer);

    return ram_info_map;
}