#include "include/utils.h"

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

SOCKET create_socket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        printf("Failed to create socket\n");

        winsock_cleanup();

        exit(EXIT_FAILURE);
    }

    return sock;
}