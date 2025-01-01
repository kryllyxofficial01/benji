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
    // cpu_info_t cpu_info = get_cpu_info();
    // map_t* cpu_info_map_data = serialize_cpu_info(cpu_info);

    // char* cpu_info_json_block = map_make_json_block(cpu_info_map_data, "cpu_info");

    // char* data = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
    // sprintf(data, "{%s}", cpu_info_json_block);

    // write_to_file("./tests/sysinfo.json", data);

    // map_free(cpu_info_map_data);

    BENJI_SOCKET client_socket = accept_client(server_socket);

    char* data = receive_from_client(client_socket);

    printf("%s\n", data);

    close_socket(client_socket);
}