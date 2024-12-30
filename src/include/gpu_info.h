#ifndef __BENJI_GPU_INFO_H
#define __BENJI_GPU_INFO_H

#define BENJI_NO_SERVER_INCLUDES // we dont need all the server stuff in utils.h

#include "utils.h"

typedef struct GPURES {
    size_t horizontal;
    size_t vertical;
} gpu_res_t;

typedef struct GPUDEVICE {
    const char** name;
    gpu_res_t resolution;
    double current_refresh_rate;
    double vram_amount; // (Get-ItemProperty -Path "HKLM:\SYSTEM\ControlSet001\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318}\0*" -Name HardwareInformation.qwMemorySize -ErrorAction SilentlyContinue)."HardwareInformation.qwMemorySize"
} gpu_device_t;

typedef struct GPUINFO {
    gpu_device_t* devices; // apparently there can be multiple listed, so this is my hacky solution
    size_t count;
} gpu_info_t;

gpu_info_t get_gpu_info();

#endif