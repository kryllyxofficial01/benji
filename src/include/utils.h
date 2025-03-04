#ifndef __BENJI_UTILS_H
#define __BENJI_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "result.h"
#include "map.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN /* compact Win32 to only common utilities */

    #include <windows.h>
#endif

#ifdef BENJI_USE_SERVER_UTILS
    #if defined(_WIN32)
        #define BENJI_SC_ABI __cdecl /* Benji Server Call */

        #pragma comment(lib, "ws2_32.lib")

        #include <winsock2.h>
        #include <ws2tcpip.h>

        typedef SOCKET BENJI_SOCKET;

        #define BENJI_SOCKET_ERROR SOCKET_ERROR
        #define BENJI_INVALID_SOCKET INVALID_SOCKET

        BENJI_SC_ABI void winsock_init();
        BENJI_SC_ABI void winsock_cleanup();
    #elif defined(__linux__)
        #define BENJI_SC_ABI

        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <unistd.h>
        #include <fcntl.h>
        #include <errno.h>

        typedef unsigned long long BENJI_SOCKET;

        #define BENJI_SOCKET_ERROR -1
        #define BENJI_INVALID_SOCKET (BENJI_SOCKET)(~0)
    #endif

    static enum BENJISERVERSTATUS {
        BENJI_SERVER_STOPPED,
        BENJI_SERVER_RUNNING
    } server_status;

    #define BENJI_MAX_SOCK_CONNS 5 /* maximum number of tries to connect to a socket */
    #define BENJI_MAX_TRIES 3 /* max attempts to try something */

    BENJI_SC_ABI result_t* create_socket();
    BENJI_SC_ABI result_t* close_socket(BENJI_SOCKET sock);

    BENJI_SC_ABI void terminate(const int exit_code);
#endif

#ifdef BENJI_USE_SYS_INFO_UTILS
    #if defined(_WIN32)
        #include <dxgi.h>

        #define BENJI_CPUID_BUFFER_LENGTH 4
        #define BENJI_CPUID_CPU_NAME_SECTIONS_COUNT 3
        #define BENJI_CPUID_CPU_NAME_START 0x80000002

        #define BENJI_GPU_VENDOR_INTEL 0x8086
        #define BENJI_GPU_VENDOR_AMD 0x1002
        #define BENJI_GPU_VENDOR_NVIDIA 0x10de

        #define BENJI_SMBIOS_MEMORY_DEVICE_TYPE 17
        #define BENJI_SMBIOS_SPEED_OFFSET 0x15

        typedef uint32_t (*processor_info_callback_t)(SYSTEM_LOGICAL_PROCESSOR_INFORMATION*);
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif

    #define collect_map_data(info_type, get_info, convert_to_map, map_data) \
        result_t* info_result = get_info(); \
        return_if_error(info_result); \
        info_type cpu_info = *(info_type*) result_unwrap(info_result); \
        map_data = convert_to_map(cpu_info);

    result_t* get_hardware_info();
#endif

#define BENJI_BASIC_STRING_LENGTH 1024 /* good general beginning size for a string */

#define BENJI_CAPACITY(n, t) ((n) * sizeof(t))

#if defined(_WIN32)
    #define SLEEP(ms) Sleep(ms)
#elif defined(__linux__)
    #define SLEEP(ms) sleep(ms)
#endif

#ifdef NOERROR
    #define BENJI_NO_ERROR NOERROR
#else
    #define BENJI_NO_ERROR 0
#endif

void strtrim(char* string);
size_t splitstr(const char* string, char*** tokens, const char character); // returns token count

#ifdef _WIN32
    char* wcharp_to_charp(const WCHAR* wchar);
#endif

#endif