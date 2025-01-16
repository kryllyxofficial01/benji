#ifndef __BENJI_GPU_INFO
#define __BENJI_GPU_INFO

#define BENJI_USE_SYS_INFO_UTILS

#include "utils.h"

#include "map.h"

typedef struct BENJIGPUINFO {
    char* name;
    char* vendor;
    double dedicated_video_memory;
    double dedicated_system_memory;
    double shared_system_memory;
} gpu_info_t;

struct BENJIGPUDATA {
    DISPLAY_DEVICEW device;
    DXGI_ADAPTER_DESC description;
};

gpu_info_t get_gpu_info();

char* get_gpu_name();
char* get_gpu_vendor();
double get_gpu_dedicated_video_memory();
double get_gpu_dedicated_system_memory();
double get_gpu_shared_system_memory();

struct BENJIGPUDATA get_gpu_data();

map_t* gpu_info_to_map(gpu_info_t gpu_info);

#endif