#define BENJI_USE_SERVER_UTILS

#include "include/utils.h"

#ifdef _WIN32
    BENJI_SC_ABI void winsock_init() {
        struct WSAData wsa_data;

        printf("Initializing Winsock2 ... ");

        if (WSAStartup(WINSOCK_VERSION, &wsa_data) != BENJI_NO_ERROR) {
            printf("Failed to initialize Winsock2");

            exit(EXIT_FAILURE);
        }

        printf("Success\n\n");
    }

    BENJI_SC_ABI void winsock_cleanup() {
        if (WSACleanup() == BENJI_SOCKET_ERROR) {
            printf("Failed to cleanup Winsock2\n");

            exit(EXIT_FAILURE);
        }
    }
#endif

BENJI_SC_ABI BENJI_SOCKET create_socket() {
    BENJI_SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == BENJI_SOCKET_ERROR) {
        printf("Failed to close socket\n");

        #ifdef _WIN32
            winsock_cleanup();
        #endif

        exit(EXIT_FAILURE);
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

        #ifdef _WIN32
            winsock_cleanup();
        #endif

        exit(EXIT_FAILURE);
    }
}

BENJI_SOCKET unwrap_socket_result(result_t* result) {
    return (BENJI_SOCKET) (uintptr_t) result_unwrap(result);
}

result_t* make_socket_result(BENJI_SOCKET sock) {
    // assume that this is only being used in a context with no errors
    return result_success((void*) (uintptr_t) sock);
}

BENJI_SC_ABI void terminate(const int exit_code) {
    #ifdef _WIN32
        winsock_cleanup();
    #endif

    exit(EXIT_FAILURE);
}