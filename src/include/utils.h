#ifndef __BENJI_UTILS_H
#define __BENJI_UTILS_H

#include <stdio.h>
#include <ctype.h>

#define WIN32_LEAN_AND_MEAN // compact the Win32 library into only common utilities

#ifndef BENJI_NO_SERVER_INCLUDES
    #if defined(_WIN32)
        #define BENJI_ABI __cdecl

        #pragma comment(lib, "ws2_32.lib")

        #include <winsock2.h>
        #include <ws2tcpip.h>

        BENJI_ABI void winsock_init();
        BENJI_ABI void winsock_cleanup();
    #elif defined(__linux__)
        #define BENJI_ABI

        #include <stdlib.h>
        #include <string.h>
        #include <errno.h>

        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <unistd.h>

        typedef int SOCKET;
    #endif

    #ifndef NO_ERROR
        #define NO_ERROR 0
    #endif

    #ifndef SOCKET_ERROR
        #define SOCKET_ERROR -1
    #endif

    #define MAX_SOCK_CONNS 5 // maximum number of tries to connect to a socket

    BENJI_ABI SOCKET create_socket();
    BENJI_ABI void close_socket(SOCKET sock);

    BENJI_ABI void stop(const unsigned int exit_code);
#endif

void remove_whitespace(char* string);

#endif