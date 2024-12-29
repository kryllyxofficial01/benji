#include "include/server.h"

SOCKET create_server() {
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // ipv4 address family
    server_address.sin_addr.S_un.S_addr = INADDR_ANY;

    printf("Creating server socket ... ");

    SOCKET server_socket = create_socket();

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Failed to bind to server address\n");

        winsock_cleanup();

        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_SOCK_CONNS) == SOCKET_ERROR) {
        printf("Failed to put into listening mode\n");

        winsock_cleanup();

        exit(EXIT_FAILURE);
    }

    printf("Success\n\n");

    socklen_t server_address_length = sizeof(server_address);
    getsockname(server_socket, (struct sockaddr*) &server_address, &server_address_length);

    printf("Server created at '127.0.0.1:%d'\n", ntohs(server_address.sin_port));

    return server_socket;
}