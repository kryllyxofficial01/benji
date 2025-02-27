#define BENJI_USE_SYS_INFO_UTILS

#include "include/hardware.h"
#include "include/utils.h"

result_t* get_hardware_info(const char* hardware_group, char** header) {
    map_t* map_data;

    if (strcmp(hardware_group, "cpu_all") == 0) {
        result_t* cpu_info_result = get_cpu_info();

        return_if_error(cpu_info_result);

        cpu_info_t cpu_info = *(cpu_info_t*) result_unwrap(cpu_info_result);

        map_data = cpu_info_to_map(cpu_info);

        *header = "cpu_info";
    }
    else if (strcmp(hardware_group, "gpu_all") == 0) {
        result_t* gpu_info_result = get_gpu_info();

        return_if_error(gpu_info_result);

        gpu_info_t gpu_info = *(gpu_info_t*) result_unwrap(gpu_info_result);

        map_data = gpu_info_to_map(gpu_info);

        *header = "gpu_info";
    }
    else if (strcmp(hardware_group, "ram_all") == 0) {
        result_t* ram_info_result = get_ram_info();

        return_if_error(ram_info_result);

        ram_info_t ram_info = *(ram_info_t*) result_unwrap(ram_info_result);

        map_data = ram_info_to_map(ram_info);

        *header = "ram_info";
    }

    return result_success(map_data);
}