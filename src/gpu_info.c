#include "include/gpu_info.h"

result_t* get_gpu_info() {
    gpu_info_t* info = malloc(sizeof(gpu_info_t));

    info->name = strdup((char*) result_unwrap(get_gpu_name()));
    strtrim(info->name);

    info->vendor = strdup((char*) result_unwrap(get_gpu_vendor()));

    info->dedicated_video_memory = *(double*) result_unwrap(get_gpu_dedicated_video_memory());
    info->dedicated_system_memory = *(double*) result_unwrap(get_gpu_dedicated_system_memory());
    info->shared_system_memory = *(double*) result_unwrap(get_gpu_shared_system_memory());

    return result_success(info);
}

result_t* get_gpu_name() {
    #if defined(_WIN32)
        DISPLAY_DEVICEW device;
        device.cb = sizeof(DISPLAY_DEVICEW);

        for (int i = 0; EnumDisplayDevicesW(NULL, i, &device, 0); i++) {
            // get the primary GPU
            if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                return result_success(wcharp_to_charp(device.DeviceString));
            }
        }

        return result_error(-1, "no primary GPU found");
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_vendor() {
    #if defined(_WIN32)
        char* vendor;

        DXGI_ADAPTER_DESC* description = (DXGI_ADAPTER_DESC*) result_unwrap(get_gpu_description());

        switch (description->VendorId) {
            case BENJI_GPU_VENDOR_INTEL: vendor = "Intel"; break;
            case BENJI_GPU_VENDOR_AMD: vendor = "AMD"; break;
            case BENJI_GPU_VENDOR_NVIDIA: vendor = "NVIDIA"; break;

            default: vendor = "???"; break;
        }

        return result_success(vendor);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_dedicated_video_memory() {
    #if defined(_WIN32)
        DXGI_ADAPTER_DESC* description = (DXGI_ADAPTER_DESC*) result_unwrap(get_gpu_description());

        void* memory = malloc(sizeof(double));

        *(double*) memory = description->DedicatedVideoMemory / 1e9;

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_dedicated_system_memory() {
    #if defined(_WIN32)
        DXGI_ADAPTER_DESC* description = (DXGI_ADAPTER_DESC*) result_unwrap(get_gpu_description());

        void* memory = malloc(sizeof(double));

        *(double*) memory = description->DedicatedSystemMemory / 1e9;

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_shared_system_memory() {
    #if defined(_WIN32)
        DXGI_ADAPTER_DESC* description = (DXGI_ADAPTER_DESC*) result_unwrap(get_gpu_description());

        void* memory = malloc(sizeof(double));

        *(double*) memory = description->SharedSystemMemory / 1e9;

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

#ifdef _WIN32
    result_t* get_gpu_description() {
        HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

        if (FAILED(result)) {}

        IDXGIFactory* factory = NULL;
        result = CreateDXGIFactory(&IID_IDXGIFactory, (void**) &factory);

        if (FAILED(result)) {
            return result_error(result, "CreateDXGIFactory failed");
        }

        IDXGIAdapter* primary_adapter = NULL;
        DXGI_ADAPTER_DESC* primary_adapter_description = malloc(sizeof(DXGI_ADAPTER_DESC));

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

        result = primary_adapter->lpVtbl->GetDesc(primary_adapter, primary_adapter_description);

        if (primary_adapter == NULL) {
            return result_error(result, "GetDesc failed");
        }

        primary_adapter->lpVtbl->Release(primary_adapter);
        factory->lpVtbl->Release(factory);

        CoUninitialize();

        return result_success((void*) primary_adapter_description);
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