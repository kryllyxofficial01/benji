#include "include/server.h"

BENJI_SC_ABI BENJI_SOCKET server_init() {
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // ipv4 address family

    #if defined(_WIN32)
        server_address.sin_addr.S_un.S_addr = INADDR_ANY;
    #elif defined(__linux__)
        server_address.sin_addr.s_addr = INADDR_ANY;
    #endif

    server_status = BENJI_SERVER_STOPPED;

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

    server_status = BENJI_SERVER_RUNNING;

    return server_socket;
}

BENJI_SC_ABI void server_run(BENJI_SOCKET server_socket) {
    while (server_status == BENJI_SERVER_RUNNING) {
        BENJI_SOCKET client_socket = server_accept_client(server_socket);

        char* data = server_receive_from_client(client_socket);
        char** data_groups = NULL;

        size_t data_group_count = server_parse_client_data(data, &data_groups);

        char* json = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

        json[0] = '\0';

        for (size_t i = 0; i < data_group_count; i++) {
            char* json_block = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

            if (strcmp(data_groups[i], "cpu_all") == 0) {
                cpu_info_t cpu_info = get_cpu_info();
                map_t* cpu_info_map_data = cpu_info_to_map(cpu_info);

                sprintf(json_block, "%s,", map_serialize(cpu_info_map_data, "cpu_info"));

                strcat(json, json_block);

                map_free(cpu_info_map_data);
            }

            free(json_block);
        }

        size_t json_length = strlen(json);

        json[json_length - 1] = '\0'; // remove the trailing comma

        char response[json_length + 2];
        sprintf(response, "{%s}", json);

        server_send_to_client(client_socket, response);

        free(data);
        free(json);

        close_socket(client_socket);
    }
}

BENJI_SC_ABI BENJI_SOCKET server_accept_client(BENJI_SOCKET server_socket) {
    struct sockaddr_storage client;

    socklen_t client_length = sizeof(client);

    BENJI_SOCKET client_socket = accept(server_socket, (struct sockaddr*) &client, &client_length);

    return (client_socket != BENJI_INVALID_SOCKET) ? client_socket : 0;
}

BENJI_SC_ABI char* server_receive_from_client(BENJI_SOCKET client_socket) {
    char* buffer = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

    buffer[0] = '\0';

    size_t received_bytes = recv(client_socket, buffer, sizeof(buffer), 0);

    if (received_bytes != BENJI_SOCKET_ERROR) {
        buffer[received_bytes] = '\0';
        return buffer;
    }
    else {
        return "";
    }
}

BENJI_SC_ABI int server_send_to_client(BENJI_SOCKET client_socket, const char* data) {
    unsigned int tries = 0;

    retry: {
        if (send(client_socket, data, strlen(data) + 1, 0) == BENJI_SOCKET_ERROR) {
            if (tries < 3) {
                tries++;

                goto retry;
            }
            else if (tries == 3) {
                return -1;
            }
        }
    }
}

size_t server_parse_client_data(const char* client_data, char*** data_groups) {
    return splitstr(client_data, data_groups, ";");
}