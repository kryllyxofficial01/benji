#define BENJI_USE_SERVER_UTILS

#include "include/utils.h"

#ifdef _WIN32
    BENJI_SC_ABI void winsock_init() {
        struct WSAData wsa_data;

        if (WSAStartup(WINSOCK_VERSION, &wsa_data) != BENJI_NO_ERROR) {
            terminate(WSAGetLastError());
        }
    }

    BENJI_SC_ABI void winsock_cleanup() {
        if (WSACleanup() == BENJI_SOCKET_ERROR) {
            terminate(WSAGetLastError());
        }
    }
#endif

BENJI_SC_ABI result_t* create_socket() {
    BENJI_SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == BENJI_SOCKET_ERROR) {
        #if defined(_WIN32)
            int error_code = WSAGetLastError();
        #elif defined(__linux__)
            int error_code = -1;
        #endif

        return result_error(error_code, "Failed to create socket");
    }

    return result_success((void*) (uintptr_t) sock);
}

BENJI_SC_ABI result_t* close_socket(BENJI_SOCKET sock) {
    #if defined(_WIN32)
        int return_code = closesocket(sock);
    #elif defined(__linux__)
        int return_code = close(sock);
    #endif

    if (return_code == BENJI_SOCKET_ERROR) {
        #if defined(_WIN32)
            int error_code = WSAGetLastError();
        #elif defined(__linux__)
            int error_code = -1;
        #endif

        return result_error(error_code, "Failed to close socket");
    }

    return result_success(NULL);
}

BENJI_SC_ABI void terminate(const int exit_code) {
    #ifdef _WIN32
        winsock_cleanup();
    #endif

    exit(exit_code);
}