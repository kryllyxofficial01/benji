#include "include/utils.h"

#ifndef BENJI_NO_SERVER_INCLUDES
    #if defined(_WIN32)
        BENJI_ABI void winsock_init() {
            struct WSAData wsa_data;

            printf("Initializing Winsock2 ... ");

            if (WSAStartup(WINSOCK_VERSION, &wsa_data) != NO_ERROR) {
                printf("Failed to initialize Winsock2");

                exit(EXIT_FAILURE);
            }

            printf("Success\n\n");
        }

        BENJI_ABI void winsock_cleanup() {
            if (WSACleanup() == SOCKET_ERROR) {
                printf("Failed to cleanup Winsock2\n");

                exit(EXIT_FAILURE);
            }
        }
    #endif

    BENJI_ABI SOCKET create_socket() {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock == SOCKET_ERROR) {
            printf("Failed to close socket\n");

            #if defined(_WIN32)
                winsock_cleanup();
            #endif

            exit(EXIT_FAILURE);
        }

        return sock;
    }

    BENJI_ABI void close_socket(SOCKET sock) {
        int return_code;

        #if defined(_WIN32)
            return_code = closesocket(sock);
        #elif defined(__linux__)
            return_code = close(sock);
        #endif

        if (return_code == SOCKET_ERROR) {
            printf("Failed to close socket\n");

            #if defined(_WIN32)
                winsock_cleanup();
            #endif

            exit(EXIT_FAILURE);
        }
    }

    void stop(const unsigned int exit_code) {
        #if defined(_WIN32)
            winsock_cleanup();
        #endif

        exit(EXIT_FAILURE);
    }
#endif

void remove_whitespace(char* string) {
    char* i = string;

    do {
        while (isspace(*i)) { ++i; }
    } while (*string++ = *i++);
}