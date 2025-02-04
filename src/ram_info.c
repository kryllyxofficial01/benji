#include "include/ram_info.h"

result_t* get_ram_info() {
    ram_info_t* info = malloc(sizeof(ram_info_t));

    info->total_memory = *(double*) result_unwrap(get_ram_total_memory());
    info->memory_load = *(double*) result_unwrap(get_ram_memory_load());
    info->free_memory = *(double*) result_unwrap(get_ram_free_memory());
    info->speed = (uint16_t) (uintptr_t) result_unwrap(get_ram_speed());

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

result_t* get_ram_speed() {
    #if defined(_WIN32)
        uint32_t size = GetSystemFirmwareTable('RSMB', 0, NULL, 0);

        if (size == 0) {
            return result_error(-1, "GetSystemFirmwareTable() failed");
        }

        RAW_SMBIOS_DATA* buffer = (RAW_SMBIOS_DATA*) malloc(size);

        if (!GetSystemFirmwareTable('RSMB', 0, buffer, size)) {
            free(buffer);

            return result_error(-1, "GetSystemFirmwareTable() failed");
        }

        uint8_t* data = buffer->data;
        uint8_t* end = data + buffer->length;

        while (data < end) {
            SMBIOS_MEMORY_DEVICE* memory = (SMBIOS_MEMORY_DEVICE*) data;

            if (memory->type == BENJI_SMBIOS_MEMORY_DEVICE_TYPE) {
                free(buffer);

                uint16_t speed = *(uint16_t*) (data + BENJI_SMBIOS_SPEED_OFFSET);

                return result_success((void*) (uintptr_t) speed);
            }

            data += memory->length;

            while (*data != 0 || *(data + 1) != 0) {
                data++;
            }

            data += 2;
        }

        free(buffer);

        return result_error(-1, "could not collect memory speed");
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

    sprintf(buffer, "%i", ram_info.speed);
    map_insert(ram_info_map, "ram_speed", buffer);

    free(buffer);

    return ram_info_map;
}