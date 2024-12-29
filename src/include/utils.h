#ifndef __BENJI_UTILS_H

#include <stdio.h>

#if defined(_WIN32)
    #pragma comment(lib, "ws2_32.lib")

    #define WIN32_LEAN_AND_MEAN // compact the Win32 library into only common utilities

    #include <winsock2.h>
    #include <ws2tcpip.h>

    void winsock_init();
    void winsock_cleanup();
#elif defined(__linux__)
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>

    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    #define NO_ERROR 0
    #define SOCKET_ERROR -1

    typedef int SOCKET;
#endif

#define MAX_SOCK_CONNS 5 // maximum number of tries to connect to a socket

SOCKET create_socket();
void close_socket(SOCKET sock);

void stop(const unsigned int exit_code);

#endif