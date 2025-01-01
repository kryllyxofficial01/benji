#include "include/client.h"

BENJI_SC_ABI BENJI_SOCKET accept_client(BENJI_SOCKET server_socket) {
    struct sockaddr_storage client;

    socklen_t client_length = sizeof(client);

    BENJI_SOCKET client_socket = accept(server_socket, (struct sockaddr*) &client, &client_length);

    return (client_socket != BENJI_INVALID_SOCKET) ? client_socket : 0;
}

BENJI_SC_ABI char* receive_from_client(BENJI_SOCKET client_socket) {
    char buffer[BENJI_BASIC_STRING_LENGTH];

    return (recv(client_socket, buffer, sizeof(buffer), 0) != BENJI_SOCKET_ERROR) ? buffer : "";
}