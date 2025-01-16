#include "include/gpu_info.h"

gpu_info_t get_gpu_info() {
    gpu_info_t info;

    info.name = get_gpu_name();
    strtrim(info.name);

    info.vendor = get_gpu_vendor();

    info.dedicated_video_memory = get_gpu_dedicated_video_memory();
    info.dedicated_system_memory = get_gpu_dedicated_system_memory();
    info.shared_system_memory = get_gpu_shared_system_memory();

    return info;
}

char* get_gpu_name() {
    #if defined(_WIN32)
        DISPLAY_DEVICEW device;
        device.cb = sizeof(DISPLAY_DEVICEW);

        for (int i = 0; EnumDisplayDevicesW(NULL, i, &device, 0); i++) {
            // get the primary GPU
            if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                return wcharp_to_charp(device.DeviceString);
            }
        }

        return NULL;
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

char* get_gpu_vendor() {
    #if defined(_WIN32)
        switch (get_gpu_description().VendorId) {
            case BENJI_GPU_VENDOR_INTEL: return "Intel";
            case BENJI_GPU_VENDOR_AMD: return "AMD";
            case BENJI_GPU_VENDOR_NVIDIA: return "NVIDIA";

            default: return "???";
        }
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

double get_gpu_dedicated_video_memory() {
    #if defined(_WIN32)
        return get_gpu_description().DedicatedVideoMemory / 1e9;
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

double get_gpu_dedicated_system_memory() {
    #if defined(_WIN32)
        return get_gpu_description().DedicatedSystemMemory / 1e9;
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

double get_gpu_shared_system_memory() {
    #if defined(_WIN32)
        return get_gpu_description().SharedSystemMemory / 1e9;
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

#ifdef _WIN32
    DXGI_ADAPTER_DESC get_gpu_description() {
        HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

        if (FAILED(result)) {}

        IDXGIFactory* factory = NULL;
        result = CreateDXGIFactory(&IID_IDXGIFactory, (void**) &factory);

        if (FAILED(result)) {}

        IDXGIAdapter* primary_adapter = NULL;
        DXGI_ADAPTER_DESC primary_adapter_description;

        IDXGIAdapter* adapter = NULL;
        UINT index = 0;
        while (factory->lpVtbl->EnumAdapters(factory, index, &adapter) != DXGI_ERROR_NOT_FOUND) {
            IDXGIOutput* output = NULL;

            if (adapter->lpVtbl->EnumOutputs(adapter, 0, &output) == S_OK) {
                DXGI_OUTPUT_DESC output_description;
                result = output->lpVtbl->GetDesc(output, &output_description);

                if (SUCCEEDED(result) && output_description.AttachedToDesktop) {
                    primary_adapter = adapter;
                    primary_adapter->lpVtbl->AddRef(primary_adapter);

                    output->lpVtbl->Release(output);

                    break;
                }

                output->lpVtbl->Release(output);
            }

            adapter->lpVtbl->Release(adapter);

            index++;
        }

        result = primary_adapter->lpVtbl->GetDesc(primary_adapter, &primary_adapter_description);

        if (primary_adapter == NULL) {}

        primary_adapter->lpVtbl->Release(primary_adapter);
        factory->lpVtbl->Release(factory);

        CoUninitialize();

        return primary_adapter_description;
    }
#endif

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