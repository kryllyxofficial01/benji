#include "include/utils.h"

#if defined(_WIN32)
    void winsock_init() {
        struct WSAData wsa_data;

        printf("Initializing Winsock2 ... ");

        if (WSAStartup(WINSOCK_VERSION, &wsa_data) != NO_ERROR) {
            printf("Failed to initialize Winsock2");

            exit(EXIT_FAILURE);
        }

        printf("Success\n\n");
    }

    void winsock_cleanup() {
        if (WSACleanup() == SOCKET_ERROR) {
            printf("Failed to cleanup Winsock2\n");

            exit(EXIT_FAILURE);
        }
    }
#endif

SOCKET create_socket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    #if defined(_WIN32)
        if (sock == INVALID_SOCKET) {
            printf("Failed to create socket\n");

            winsock_cleanup();

            exit(EXIT_FAILURE);
        }
    #elif defined(__linux__)
        if (sock == SOCKET_ERROR) {
            printf("Failed to create socket: %s\n", strerror(errno));

            exit(EXIT_FAILURE);
        }
    #endif

    return sock;
}

void close_socket(SOCKET sock) {
    #if defined(_WIN32)
        if (closesocket(sock) == SOCKET_ERROR) {
            printf("Failed to close socket\n");

            winsock_cleanup();

            exit(EXIT_FAILURE);
        }
    #elif defined(__linux__)
        if (close(sock) != NO_ERROR) {
            printf("Failed to close socket\n");

            exit(EXIT_FAILURE);
        }
    #endif
}

void stop(const unsigned int exit_code) {
    #if defined(_WIN32)
        winsock_cleanup();
    #endif

    exit(EXIT_FAILURE);
}