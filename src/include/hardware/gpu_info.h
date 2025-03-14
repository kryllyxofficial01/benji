#ifndef __BENJI_GPU_INFO
#define __BENJI_GPU_INFO

#ifndef BENJI_USE_SYS_INFO_UTILS
    #define BENJI_USE_SYS_INFO_UTILS
#endif

#pragma comment(lib, "wbemuuid.lib")

#include <wbemidl.h>
#include <comdef.h>

#include "../utils.h"

#ifndef BENJI_GPU_VENDOR_INTEL
    #define BENJI_GPU_VENDOR_INTEL (0x8086)
#endif

#ifndef BENJI_GPU_VENDOR_AMD
    #define BENJI_GPU_VENDOR_AMD (0x1002)
#endif

#ifndef BENJI_GPU_VENDOR_NVIDIA
    #define BENJI_GPU_VENDOR_NVIDIA (0x10de)
#endif

typedef struct _BENJI_GPU_INFO {
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

result_t* gpu_info_to_map(gpu_info_t gpu_info);

#endif