#ifndef __BENJI_UTILS_H
#define __BENJI_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN /* compact Win32 to only common utilities */
#endif

#ifdef NO_ERROR
    #define BENJI_NO_ERROR NO_ERROR
#else
    #define BENJI_NO_ERROR 0
#endif

#define BENJI_BASIC_STRING_LENGTH 1024 /* good general beginning size for a string */

#ifdef BENJI_USE_SERVER_UTILS
    #if defined(_WIN32)
        #define BENJI_SC_ABI __cdecl /* Benji Server Call */

        #pragma comment(lib, "ws2_32.lib")

        #include <winsock2.h>
        #include <ws2tcpip.h>

        typedef SOCKET BENJI_SOCKET;

        #define BENJI_SOCKET_ERROR SOCKET_ERROR

        BENJI_SC_ABI void winsock_init();
        BENJI_SC_ABI void winsock_cleanup();
    #elif defined(__linux__)
        #define BENJI_SC_ABI

        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <unistd.h>

        typedef unsigned long long BENJI_SOCKET;

        #define BENJI_SOCKET_ERROR -1
    #endif

    #define BENJI_MAX_SOCK_CONNS 5 /* maximum number of tries to connect to a socket */

    BENJI_SC_ABI BENJI_SOCKET create_socket();
    BENJI_SC_ABI void close_socket(BENJI_SOCKET sock);

    BENJI_SC_ABI void terminate(const size_t exit_code);
#endif

#ifdef BENJI_USE_SYS_INFO_UTILS
    #if defined(_WIN32)
        // are these too long? i think these are too long
        #define BENJI_CPUID_CPU_NAME_BUFFER_LENGTH 4
        #define BENJI_CPUID_CPU_NAME_SECTIONS_COUNT 3
        #define BENJI_CPUID_CPU_NAME_START 0x80000002

        enum BENJI_WMI_TYPE {
            WMI_WIN32_PROCESSOR,
            WMI_WIN32_VIDEOCONTROLLER
        };

        enum BENJI_WMI_ENTRY {
            WMI_CPU_CLOCK_SPEED,
            WMI_CPU_CORE_COUNT,
            WMI_CPU_LOGICAL_PROCESSORS_COUNT,

            WMI_GPU_NAMES,
            WMI_GPU_CURRENT_HORIZONTAL_RESOLUTIONS,
            WMI_GPU_CURRENT_VERTICAL_RESOLUTIONS,
            WMI_GPU_CURRENT_REFRESH_RATES,
            WMI_GPU_ADAPTER_RAM_AMOUNTS
        };

        const char* wmi_get_data(enum BENJI_WMI_TYPE type, enum BENJI_WMI_ENTRY entry);
        const char* wmi_type_to_string(enum BENJI_WMI_TYPE type);
        const char* wmi_entry_to_string(enum BENJI_WMI_ENTRY entry);

        const char* run_command(const char* command); // returns command output
    #endif
#endif

void strtrim(char* string);
size_t splitstr(const char* string, char*** tokens, const char* character); // returns token count

#endif