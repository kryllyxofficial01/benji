#ifndef __BENJI_UTILS_H
#define __BENJI_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN /* compact WIN32 to only common utilities */
#endif

#ifndef BENJI_NO_SERVER_INCLUDES
    #if defined(_WIN32)
        #define BENJI_ABI WINAPI

        #pragma comment(lib, "ws2_32.lib")

        #include <winsock2.h>
        #include <ws2tcpip.h>

        typedef SOCKET BENJI_SOCKET;

        BENJI_ABI void winsock_init();
        BENJI_ABI void winsock_cleanup();
    #elif defined(__linux__)
        #define BENJI_ABI

        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <unistd.h>

        typedef unsigned long long BENJI_SOCKET;
    #endif

    #define BENJI_NO_ERROR 0
    #define BENJI_SOCKET_ERROR -1

    #define BENJI_MAX_SOCK_CONNS 5 // maximum number of tries to connect to a socket

    BENJI_ABI BENJI_SOCKET create_socket();
    BENJI_ABI void close_socket(BENJI_SOCKET sock);

    BENJI_ABI void stop(const size_t exit_code);
#endif

#ifndef BENJI_NO_SYS_INFO_INCLUDES
    #if defined(_WIN32)
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
            WMI_GPU_CURRENT_REFRESH_RATES
        };

        const char* wmi_get_data(enum BENJI_WMI_TYPE type, enum BENJI_WMI_ENTRY entry);
        const char* wmi_type_to_string(enum BENJI_WMI_TYPE type);
        const char* wmi_entry_to_string(enum BENJI_WMI_ENTRY entry);

        const char* run_powershell_command(const char* command); // returns command output
    #endif
#endif

void strtrim(char* string);
size_t splitstr(const char* string, char*** tokens, const char* character); // returns token count

#endif