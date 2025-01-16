#include "include/gpu_info.h"

gpu_info_t get_gpu_info() {
    gpu_info_t info;

    info.name = get_gpu_name();
    strtrim(info.name);

    info.vendor = get_gpu_vendor();
    strtrim(info.vendor);

    info.dedicated_video_memory = get_gpu_dedicated_video_memory();
    info.dedicated_system_memory = get_gpu_dedicated_system_memory();
    info.shared_system_memory = get_gpu_shared_system_memory();

    return info;
}

char* get_gpu_name() {
    #ifdef _WIN32
        DISPLAY_DEVICEW device;
        device.cb = sizeof(DISPLAY_DEVICEW);

        for (int i = 0; EnumDisplayDevicesW(NULL, i, &device, 0); i++) {
            // get the primary GPU
            if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                return wcharp_to_charp(device.DeviceString);
            }
        }

        return NULL;
    #endif
}

char* get_gpu_vendor() {
    #ifdef _WIN32
        switch (get_gpu_data().description.VendorId) {
            case BENJI_GPU_VENDOR_INTEL: return "Intel";
            case BENJI_GPU_VENDOR_AMD: return "AMD";
            case BENJI_GPU_VENDOR_NVIDIA: return "NVIDIA";

            default: return "???";
        }
    #endif
}

double get_gpu_dedicated_video_memory() {
    #ifdef _WIN32
        return (get_gpu_data().description.DedicatedVideoMemory / (1024 * 1024)) / 1000;
    #endif
}

double get_gpu_dedicated_system_memory() {
    #ifdef _WIN32
        return (get_gpu_data().description.DedicatedSystemMemory/ (1024 * 1024)) / 1000;
    #endif
}

double get_gpu_shared_system_memory() {
    #ifdef _WIN32
        return (get_gpu_data().description.SharedSystemMemory / (1024 * 1024)) / 1000;
    #endif
}

struct BENJIGPUDATA get_gpu_data() {
    struct BENJIGPUDATA gpu_data;

    IDXGIFactory* factory = NULL;

    HRESULT result = CreateDXGIFactory(&IID_IDXGIFactory, (void**) &factory);

    if (FAILED(result)) {
        // todo: error handling
    }

    IDXGIAdapter* adapter = NULL;

    DISPLAY_DEVICEW device;
    device.cb = sizeof(DISPLAY_DEVICEW);

    DXGI_ADAPTER_DESC description;

    adapter->lpVtbl->GetDesc(adapter, &description);
    gpu_data.description = description;

    for (int i = 0; EnumDisplayDevicesW(NULL, i, &device, 0); i++) {
        // check if the primary GPU
        if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
            gpu_data.device = device;
            break;
        }
    }

    adapter->lpVtbl->Release(adapter);
    factory->lpVtbl->Release(factory);

    return gpu_data;
}

map_t* gpu_info_to_map(gpu_info_t gpu_info) {
    map_t* gpu_info_map = map_init();

    char* buffer = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
    buffer[0] = '\0';

    map_insert(gpu_info_map, "gpu_name", gpu_info.name);
    map_insert(gpu_info_map, "gpu_vendor", gpu_info.vendor);

    sprintf(buffer, "%0.3f", gpu_info.dedicated_video_memory);
    map_insert(gpu_info_map, "gpu_dedicated_video_memory", buffer);

    sprintf(buffer, "%0.3f", gpu_info.dedicated_system_memory);
    map_insert(gpu_info_map, "gpu_dedicated_system_memory", buffer);

    sprintf(buffer, "%0.3f", gpu_info.shared_system_memory);
    map_insert(gpu_info_map, "gpu_shared_system_memory", buffer);

    free(buffer);

    return gpu_info_map;
}