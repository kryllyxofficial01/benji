#ifndef __BENJI_GPU_INFO
#define __BENJI_GPU_INFO

#define BENJI_USE_SYS_INFO_UTILS

#include "utils.h"

#include "map.h"

typedef struct BENJIGPUINFO {
    char* name;
    char* vendor;
    double dedicated_video_memory; // in GB
    double dedicated_system_memory; // in GB
    double shared_system_memory; // in GB
} gpu_info_t;

result_t* get_gpu_info();

result_t* get_gpu_name();
result_t* get_gpu_vendor();
result_t* get_gpu_dedicated_video_memory();
result_t* get_gpu_dedicated_system_memory();
result_t* get_gpu_shared_system_memory();

#ifdef _WIN32
    result_t* get_gpu_description();
#endif

map_t* gpu_info_to_map(gpu_info_t gpu_info);

#endif