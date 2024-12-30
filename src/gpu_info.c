#include "include/gpu_info.h"

gpu_info_t get_gpu_info() {
    gpu_info_t info;

    char* wmi_gpu_names = wmi_get_data(WMI_WIN32_VIDEOCONTROLLER, WMI_GPU_NAMES);
    char** gpu_names = NULL;

    size_t num_devices = splitstr(wmi_gpu_names, &gpu_names, "\n");

    return info;
}