#ifndef __BENJI_RAM_INFO_H
#define __BENJI_RAM_INFO_H

#define BENJI_USE_SYS_INFO_UTILS

#include "utils.h"

#include "map.h"

typedef struct BENJIRAMINFO {
    double total_memory; // in GB
    double memory_load; // in GB
    double free_memory; // in GB
} ram_info_t;

result_t* get_ram_info();

result_t* get_ram_total_memory();
result_t* get_ram_memory_load();
result_t* get_ram_free_memory();

#ifdef _WIN32
    result_t* get_memory_status();
#endif

map_t* ram_info_to_map(ram_info_t ram_info);

#endif