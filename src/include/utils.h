#ifndef __BENJI_UTILS_H

#include <stdio.h>

#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")

    #define WIN32_LEAN_AND_MEAN // compact the Win32 library into only common utilities

    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

#define MAX_SOCK_CONNS 5 // maximum number of tries to connect to a socket

void winsock_init();
void winsock_cleanup();

SOCKET create_socket();

#endif