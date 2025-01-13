#include "include/gpu_info.h"

gpu_info_t get_gpu_info() {
    gpu_info_t info;

    info.name = get_gpu_name();
    strtrim(info.name);

    // info.vendor = get_gpu_vendor();
    // strtrim(info.vendor);

    // info.dedicated_video_memory = get_gpu_dedicated_video_memory();
    // info.dedicated_system_memory = get_gpu_dedicated_system_memory();
    // info.shared_system_memory = get_gpu_shared_system_memory();

    return info;
}

char* get_gpu_name() {
    get_gpu_data();
}

char* get_gpu_vendor();
double get_gpu_dedicated_video_memory();
double get_gpu_dedicated_system_memory();
double get_gpu_shared_system_memory();

DXGI_ADAPTER_DESC get_gpu_data() {
    IDXGIFactory* factory = NULL;

    HRESULT result = CreateDXGIFactory(&IID_IDXGIFactory, (void**) &factory);

    if (FAILED(result)) {
        return;
    }

    IDXGIAdapter* adapter = NULL;
    size_t i = 0;

    while (factory->lpVtbl->EnumAdapters(factory, i, &adapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC description;

        adapter->lpVtbl->GetDesc(adapter, &description);

        printf("Adapter %u: %s\n", i, description.Description);
        printf("\tDedicated Video Memory: %llu MB\n", description.DedicatedVideoMemory / (1024 * 1024));
        printf("\tDedicated System Memory: %llu MB\n", description.DedicatedSystemMemory / (1024 * 1024));
        printf("\tShared System Memory: %llu MB\n", description.SharedSystemMemory / (1024 * 1024));

        adapter->lpVtbl->Release(adapter);

        i++;
    }

    factory->lpVtbl->Release(factory);
}