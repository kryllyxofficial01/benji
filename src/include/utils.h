#ifndef __BENJI_UTILS_H
#define __BENJI_UTILS_H

#ifndef UNICODE
    #define UNICODE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN /* compact Win32 to only common utilities */

    #include <windows.h>
#endif

#ifndef BENJI_BASIC_STRING_LENGTH
    #define BENJI_BASIC_STRING_LENGTH (1024) /* good general beginning size for a string */
#endif

#ifndef BENJI_CAPACITY
    #define BENJI_CAPACITY(n, t) ((n) * sizeof(t))
#endif

#ifndef BENJI_SLEEP
    #if defined(_WIN32)
        #define BENJI_SLEEP(ms) Sleep(ms)
    #elif defined(__linux__)
        #define BENJI_SLEEP(ms) sleep(ms)
    #endif
#endif

#ifndef BENJI_NO_ERROR
    #ifdef NOERROR
        #define BENJI_NO_ERROR NOERROR
    #else
        #define BENJI_NO_ERROR (0)
    #endif
#endif

#ifndef BENJI_NO_FLAGS
    #define BENJI_NO_FLAGS (0)
#endif

#ifdef BENJI_USE_SERVER_UTILS
    #include "result.h"

    #if defined(_WIN32)
        #pragma comment(lib, "ws2_32.lib")

        #include <winsock2.h>
        #include <ws2tcpip.h>

        #ifndef BENJIAPI
            #define BENJIAPI WINAPI /* Benji Server Call */
        #endif

        #ifndef BENJI_SOCKET_ERROR
            #define BENJI_SOCKET_ERROR SOCKET_ERROR
        #endif

        #ifndef BENJI_INVALID_SOCKET
            #define BENJI_INVALID_SOCKET INVALID_SOCKET
        #endif

        typedef SOCKET BENJI_SOCKET;

        BENJIAPI void winsock_init();
        BENJIAPI void winsock_cleanup();
    #elif defined(__linux__)
        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <unistd.h>
        #include <fcntl.h>
        #include <errno.h>

        #ifndef BENJIAPI
            #define BENJIAPI /* Benji Server Call */
        #endif

        #ifndef BENJI_SOCKET_ERROR
            #define BENJI_SOCKET_ERROR (-1)
        #endif

        #ifndef BENJI_INVALID_SOCKET
            #define BENJI_INVALID_SOCKET (unsigned long long)(~0)
        #endif

        typedef unsigned long long BENJI_SOCKET;
    #endif

    #ifndef BENJI_MAX_SOCK_CONNS
        #define BENJI_MAX_SOCK_CONNS (5) /* maximum number of tries to connect to a socket */
    #endif

    #ifndef BENJI_MAX_TRIES
        #define BENJI_MAX_TRIES (3) /* max attempts to try something */
    #endif

    static enum _BENJI_SERVER_STATUS {
        BENJI_SERVER_STOPPED,
        BENJI_SERVER_RUNNING
    } server_status;

    BENJIAPI result_t* create_socket();
    BENJIAPI result_t* close_socket(BENJI_SOCKET sock);

    BENJIAPI void terminate(const int exit_code);
#endif

#ifdef BENJI_USE_SYS_INFO_UTILS
    #include "result.h"
    #include "map.h"
    #include "logger.h"

    #if defined(_WIN32)
        #include <dxgi.h>
    #elif defined(__linux__)
        /* TODO: add linux stuff */
    #endif

    #ifndef collect_map_data
        #define collect_map_data(info_type, get_info, convert_to_map, map_data) \
            result_t* info_result = get_info(); \
            return_if_error(info_result); \
            info_type cpu_info = *(info_type*) result_unwrap_value(info_result); \
            result_t* map_data_result = convert_to_map(cpu_info); \
            return_if_error(map_data_result); \
            map_data = (map_t*) result_unwrap_value(map_data_result);
    #endif

    result_t* get_hardware_info();
#endif

void strtrim(char* string);
size_t strsplit(const char* string, char*** tokens, const char character); // returns token count

#ifdef _WIN32
    char* wcharp_to_charp(const wchar_t* wchar);
#endif

#endif