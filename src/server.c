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

BENJI_SC_ABI void run_server(BENJI_SOCKET server_socket) {
    cpu_info_t cpu_info = get_cpu_info();
    json_t* json_data = serialize_cpu_info(cpu_info);

    char* block = json_create_block(json_data, "cpu_info");

    char* data = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
    sprintf(data, "{%s}", block);

    write_to_file("./test.json", data);

    json_free(json_data);
}