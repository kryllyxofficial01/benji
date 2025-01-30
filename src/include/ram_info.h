#ifndef __BENJI_RAM_INFO_H
#define __BENJI_RAM_INFO_H

#define BENJI_USE_SYS_INFO_UTILS

#include "utils.h"

#include "map.h"

typedef struct BENJIRAMINFO {
    double total_memory; // in GB
    double memory_load; // in GB
    double free_memory; // in GB
    WORD speed; // in MHz
} ram_info_t;

#pragma pack(push, 1)
typedef struct _SMBIOS_MEMORY_DEVICE {
    BYTE type;
    BYTE length;
    WORD handle;

    WORD physical_memory_array_handle;
    WORD memory_error_information_handle;

    WORD total_width;
    WORD data_width;
    WORD size;

    BYTE form_factor;
    BYTE device_set;

    CHAR device_locator[1];
} SMBIOS_MEMORY_DEVICE;

typedef struct _RAW_SMBIOS_DATA {
    DWORD signature;
    DWORD length;
    BYTE data[1];
} RAW_SMBIOS_DATA;
#pragma pack(pop)

result_t* get_ram_info();

result_t* get_ram_total_memory();
result_t* get_ram_memory_load();
result_t* get_ram_free_memory();
result_t* get_ram_speed();

#ifdef _WIN32
    result_t* get_memory_status();
#endif

map_t* ram_info_to_map(ram_info_t ram_info);

#endif