#include "include/hardware/gpu_info.h"

result_t* get_gpu_info() {
    gpu_info_t* info = malloc(sizeof(gpu_info_t));

    // result_t* gpu_name_result = get_gpu_name();
    // return_if_error(gpu_name_result);
    // info->name = strdup((char*) result_unwrap_value(gpu_name_result));
    // strtrim(info->name);

    // result_t* gpu_vendor_result = get_gpu_vendor();
    // return_if_error(gpu_vendor_result);
    // info->vendor = strdup((char*) result_unwrap_value(gpu_vendor_result));
    // strtrim(info->vendor);

    // result_t* gpu_dedicated_video_memory_result = get_gpu_dedicated_video_memory();
    // return_if_error(gpu_dedicated_video_memory_result);
    // info->dedicated_video_memory = *(double*) result_unwrap_value(gpu_dedicated_video_memory_result);

    // result_t* gpu_dedicated_system_memory_result = get_gpu_dedicated_system_memory();
    // return_if_error(gpu_dedicated_system_memory_result);
    // info->dedicated_system_memory = *(double*) result_unwrap_value(gpu_dedicated_system_memory_result);

    // result_t* gpu_shared_system_memory_result = get_gpu_shared_system_memory();
    // return_if_error(gpu_shared_system_memory_result);
    // info->shared_system_memory = *(double*) result_unwrap_value(gpu_shared_system_memory_result);

    HRESULT hresult;

    hresult = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hresult)) {
        return result_error(hresult, "CoInitializeEx() failed", BENJI_ERROR_PACKET);
    }

    hresult = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL
    );
    if (FAILED(hresult)) {
        CoUninitialize();

        return result_error(hresult, "CoInitializeSecurity() failed", BENJI_ERROR_PACKET);
    }

    IWbemLocator* locater = NULL;

    hresult = CoCreateInstance(
        &CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        &IID_IWbemLocator, (void*) &locater
    );
    if (FAILED(hresult)) {
        CoUninitialize();

        return result_error(hresult, "CoCreateInstance() failed", BENJI_ERROR_PACKET);
    }

    IWbemServices* services = NULL;

    hresult = locater->lpVtbl->ConnectServer(
        locater, SysAllocString(TEXT("ROOT\\CIMV2")),
        NULL, NULL, NULL, 0, NULL, NULL, &services
    );
    if (FAILED(hresult)) {
        locater->lpVtbl->Release(locater);

        CoUninitialize();

        return result_error(hresult, "ConnectServer() failed", BENJI_ERROR_PACKET);
    }

    hresult = CoSetProxyBlanket(
        (IUnknown*) services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE
    );
    if (FAILED(hresult)) {
        services->lpVtbl->Release(services);
        locater->lpVtbl->Release(locater);

        CoUninitialize();

        return result_error(hresult, "CoSetProxyBlanket() failed", BENJI_ERROR_PACKET);
    }

    IEnumWbemClassObject* enumerator = NULL;

    // gpu name, vendor, and dedicated video memory
    hresult = services->lpVtbl->ExecQuery(
        services, SysAllocString(TEXT("WQL")),
        SysAllocString(TEXT("SELECT Name, Manufacturer, AdapterRAM, DedicatedSystemMemory, SharedSystemMemory FROM Win32_VideoController")),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator
    );
    if (FAILED(hresult) || !enumerator) {
        services->lpVtbl->Release(services);
        locater->lpVtbl->Release(locater);

        CoUninitialize();

        return result_error(hresult, "ExecQuery() failed", BENJI_ERROR_PACKET);
    }

    IWbemClassObject* class_object = NULL;
    unsigned long return_value = 0;

    if (enumerator->lpVtbl->Next(enumerator, WBEM_INFINITE, 1, &class_object, &return_value) == (HRESULT) BENJI_NO_ERROR) {
        VARIANT variant_prop;

        // gpu name
        hresult = class_object->lpVtbl->Get(class_object, TEXT("Name"), 0, &variant_prop, 0, 0);
        if (SUCCEEDED(hresult)) {
            info->name = wcharp_to_charp(variant_prop.bstrVal);
        }
        else {
            info->name = "???";
        }

        VariantClear(&variant_prop);

        // vendor
        hresult = class_object->lpVtbl->Get(class_object, TEXT("Manufacturer"), 0, &variant_prop, 0, 0);
        if (SUCCEEDED(hresult)) {
            info->vendor = wcharp_to_charp(variant_prop.bstrVal);
        }
        else {
            info->vendor = "???";
        }

        VariantClear(&variant_prop);

        // dedicated video memory
        hresult = class_object->lpVtbl->Get(class_object, TEXT("AdapterRAM"), 0, &variant_prop, 0, 0);
        if (SUCCEEDED(hresult) && variant_prop.vt != VT_NULL) {
            info->dedicated_video_memory = (double) variant_prop.ulVal / (1024.0 * 1024.0 * 1024.0);
        }
        else {
            info->dedicated_video_memory = -1.0;
        }

        VariantClear(&variant_prop);

        // dedicated system memory
        hresult = class_object->lpVtbl->Get(class_object, TEXT("DedicatedSystemMemory"), 0, &variant_prop, 0, 0);
        if (SUCCEEDED(hresult) && variant_prop.vt != VT_NULL) {
            info->dedicated_system_memory = (double) variant_prop.ulVal / (1024.0 * 1024.0 * 1024.0);
        }
        else {
            info->dedicated_system_memory = -1.0;
        }

        // shared system memory
        hresult = class_object->lpVtbl->Get(class_object, TEXT("SharedSystemMemory"), 0, &variant_prop, 0, 0);
        if (SUCCEEDED(hresult) && variant_prop.vt != VT_NULL) {
            info->shared_system_memory = (double) variant_prop.ulVal / (1024.0 * 1024.0 * 1024.0);
        }
        else {
            info->shared_system_memory = -1.0;
        }

        VariantClear(&variant_prop);

        class_object->lpVtbl->Release(class_object);
    }
    else {
        enumerator->lpVtbl->Release(enumerator);

        result_error(-1, "Unable to collect GPU information", BENJI_ERROR_PACKET);
    }

    enumerator->lpVtbl->Release(enumerator);

    return result_success(info);
}

result_t* get_gpu_name() {
    #if defined(_WIN32)
        DISPLAY_DEVICEW device;
        device.cb = sizeof(DISPLAY_DEVICEW);

        int i = 0;
        while (true) {
            if (!EnumDisplayDevicesW(NULL, i, &device, 0)) {
                return result_error(GetLastError(), "EnumDisplayDevicesW() failed", BENJI_ERROR_PACKET);
            }

            // get the primary GPU
            if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                return result_success(wcharp_to_charp(device.DeviceString));
            }

            i++;
        }

        return result_error(-1, "no primary GPU found", BENJI_ERROR_PACKET);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_vendor() {
    #if defined(_WIN32)
        char* vendor;

        result_t* description_result = get_gpu_description();
        return_if_error(description_result);

        DXGI_ADAPTER_DESC description = *(DXGI_ADAPTER_DESC*) result_unwrap_value(description_result);

        switch (description.VendorId) {
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
        result_t* description_result = get_gpu_description();
        return_if_error(description_result);

        DXGI_ADAPTER_DESC description = *(DXGI_ADAPTER_DESC*) result_unwrap_value(description_result);

        void* memory = malloc(sizeof(double));

        if (memory) {
            *(double*) memory = description.DedicatedVideoMemory / (1024.0 * 1024.0 * 1024.0);
        }
        else {
            return result_error(-1, "malloc() failed", BENJI_ERROR_PACKET);
        }

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_dedicated_system_memory() {
    #if defined(_WIN32)
        result_t* description_result = get_gpu_description();
        return_if_error(description_result);

        DXGI_ADAPTER_DESC description = *(DXGI_ADAPTER_DESC*) result_unwrap_value(description_result);

        void* memory = malloc(sizeof(double));

        if (memory) {
            *(double*) memory = description.DedicatedSystemMemory / (1024.0 * 1024.0 * 1024.0);
        }
        else {
            return result_error(-1, "malloc() failed", BENJI_ERROR_PACKET);
        }

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

result_t* get_gpu_shared_system_memory() {
    #if defined(_WIN32)
        result_t* description_result = get_gpu_description();
        return_if_error(description_result);

        DXGI_ADAPTER_DESC description = *(DXGI_ADAPTER_DESC*) result_unwrap_value(description_result);

        void* memory = malloc(sizeof(double));

        if (memory) {
            *(double*) memory = description.SharedSystemMemory / (1024.0 * 1024.0 * 1024.0);
        }
        else {
            return result_error(-1, "malloc() failed", BENJI_ERROR_PACKET);
        }

        return result_success(memory);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif
}

#ifdef _WIN32
    result_t* get_gpu_description() {
        HRESULT hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED);

        if (FAILED(hresult)) {
            return result_error(hresult, "CoInitializeEx() failed", BENJI_ERROR_PACKET);
        }

        IDXGIFactory* factory = NULL;
        hresult = CreateDXGIFactory(&IID_IDXGIFactory, (void**) &factory);

        if (FAILED(hresult)) {
            return result_error(hresult, "CreateDXGIFactory() failed", BENJI_ERROR_PACKET);
        }

        IDXGIAdapter* primary_adapter = NULL;
        DXGI_ADAPTER_DESC* primary_adapter_description = malloc(sizeof(DXGI_ADAPTER_DESC));

        if (!primary_adapter_description) {
            return result_error(-1, "malloc() failed", BENJI_ERROR_PACKET);
        }

        IDXGIAdapter* adapter = NULL;
        UINT index = 0;
        while (factory->lpVtbl->EnumAdapters(factory, index, &adapter) != DXGI_ERROR_NOT_FOUND) {
            IDXGIOutput* output = NULL;

            if ((hresult = adapter->lpVtbl->EnumOutputs(adapter, 0, &output)) == S_OK) {
                DXGI_OUTPUT_DESC output_description;
                hresult = output->lpVtbl->GetDesc(output, &output_description);

                if (SUCCEEDED(hresult) && output_description.AttachedToDesktop) {
                    primary_adapter = adapter;
                    primary_adapter->lpVtbl->AddRef(primary_adapter);

                    output->lpVtbl->Release(output);

                    break;
                }
                else {
                    return result_error(hresult, "GetDesc() failed", BENJI_ERROR_PACKET);
                }

                output->lpVtbl->Release(output);
            }
            else {
                return result_error(hresult, "EnumOutputs() failed", BENJI_ERROR_PACKET);
            }

            adapter->lpVtbl->Release(adapter);

            index++;
        }

        hresult = primary_adapter->lpVtbl->GetDesc(primary_adapter, primary_adapter_description);

        if (primary_adapter == NULL) {
            return result_error(hresult, "GetDesc() failed", BENJI_ERROR_PACKET);
        }

        primary_adapter->lpVtbl->Release(primary_adapter);
        factory->lpVtbl->Release(factory);

        CoUninitialize();

        return result_success((void*) primary_adapter_description);
    }
#endif

result_t* gpu_info_to_map(gpu_info_t gpu_info) {
    map_t* gpu_info_map = map_init();

    char* buffer = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    if (!buffer) {
        return result_error(-1, "malloc() failed", BENJI_ERROR_PACKET);
    }

    buffer[0] = '\0';

    map_insert(gpu_info_map, "name", gpu_info.name);
    map_insert(gpu_info_map, "vendor", gpu_info.vendor);

    sprintf(buffer, "%0.3f", gpu_info.dedicated_video_memory);
    map_insert(gpu_info_map, "dedicated_video_memory", buffer);

    sprintf(buffer, "%0.3f", gpu_info.dedicated_system_memory);
    map_insert(gpu_info_map, "dedicated_system_memory", buffer);

    sprintf(buffer, "%0.3f", gpu_info.shared_system_memory);
    map_insert(gpu_info_map, "shared_system_memory", buffer);

    free(buffer);

    return result_success(gpu_info_map);
}