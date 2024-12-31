#include "include/server.h"

BENJI_SC_ABI BENJI_SOCKET create_server() {
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // ipv4 address family

    #if defined(_WIN32)
        server_address.sin_addr.S_un.S_addr = INADDR_ANY;
    #elif defined(__linux__)
        server_address.sin_addr.s_addr = INADDR_ANY;
    #endif

    printf("Creating server socket ... ");

    BENJI_SOCKET server_socket = create_socket();

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == BENJI_SOCKET_ERROR) {
        printf("Failed to bind to server address\n");

        terminate(EXIT_FAILURE);
    }

    if (listen(server_socket, BENJI_MAX_SOCK_CONNS) == BENJI_SOCKET_ERROR) {
        printf("Failed to put into listening mode\n");

        terminate(EXIT_FAILURE);
    }

    printf("Success\n\n");

    socklen_t server_address_length = sizeof(server_address);
    getsockname(server_socket, (struct sockaddr*) &server_address, &server_address_length);

    printf("Server created at '127.0.0.1:%d'\n", ntohs(server_address.sin_port));

    return server_socket;
}