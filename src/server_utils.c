#define BENJI_USE_SERVER_UTILS

#include "include/utils.h"

#ifdef _WIN32
    BENJI_SC_ABI void winsock_init() {
        struct WSAData wsa_data;

        printf("Initializing Winsock2 ... ");

        if (WSAStartup(WINSOCK_VERSION, &wsa_data) != BENJI_NO_ERROR) {
            printf("Failed to initialize Winsock2");

            terminate(WSAGetLastError()); // idk about this, but ehhhhhhhhhhhh
        }

        printf("Success\n\n");
    }

    BENJI_SC_ABI void winsock_cleanup() {
        if (WSACleanup() == BENJI_SOCKET_ERROR) {
            printf("Failed to cleanup Winsock2\n");

            terminate(WSAGetLastError());
        }
    }
#endif

BENJI_SC_ABI BENJI_SOCKET create_socket() {
    BENJI_SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == BENJI_SOCKET_ERROR) {
        printf("Failed to close socket\n");

        #if defined(_WIN32)
            int error_code = WSAGetLastError();
        #elif defined(__linux__)
            int error_code = -1;
        #endif

        terminate(error_code);
    }

    return sock;
}

BENJI_SC_ABI void close_socket(BENJI_SOCKET sock) {
    int return_code;

    #if defined(_WIN32)
        return_code = closesocket(sock);
    #elif defined(__linux__)
        return_code = close(sock);
    #endif

    if (return_code == BENJI_SOCKET_ERROR) {
        printf("Failed to close socket\n");

        #if defined(_WIN32)
            int error_code = WSAGetLastError();
        #elif defined(__linux__)
            int error_code = -1;
        #endif

        terminate(error_code);
    }
}

BENJI_SC_ABI void terminate(const int exit_code) {
    #ifdef _WIN32
        winsock_cleanup();
    #endif

    exit(exit_code);
}