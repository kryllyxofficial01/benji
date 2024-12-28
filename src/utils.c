#include "include/utils.h"

void winsock_init() {
    struct WSAData wsa_data;

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR) {
        printf("Failed to initialize Winsock2");

        exit(EXIT_FAILURE);
    }

    printf("Winsock2 initialized\n\n");
}

void winsock_cleanup() {
    if (WSACleanup() == SOCKET_ERROR) {
        printf("Failed to cleanup Winsock2\n");

        exit(EXIT_FAILURE);
    }
}