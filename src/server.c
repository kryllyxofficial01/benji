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
        char** data_groups;

        size_t data_group_count = server_parse_client_data(data, &data_groups);

        char* json = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));
        json[0] = '\0';

        for (size_t i = 0; i < data_group_count; i++) {
            map_t* map_data;
            char* header;

            if (strcmp(data_groups[i], "cpu_all") == 0) {
                cpu_info_t cpu_info = get_cpu_info();
                map_data = cpu_info_to_map(cpu_info);

                header = "cpu_info";
            }
            else if (strcmp(data_groups[i], "gpu_all") == 0) {
                gpu_info_t gpu_info = get_gpu_info();
                map_data = gpu_info_to_map(gpu_info);

                header = "gpu_info";
            }

            char* json_block = malloc(BENJI_CAPACITY(BENJI_BASIC_STRING_LENGTH, char));

            sprintf(json_block, "%s,", map_serialize(map_data, header));

            strcat(json, json_block);

            free(json_block);
            map_free(map_data);
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

    if (client_socket == BENJI_INVALID_SOCKET) {
        return 0;
    }

    u_long non_blocking_mode = true;
    ioctlsocket(client_socket, FIONBIO, &non_blocking_mode);

    return client_socket;
}

BENJI_SC_ABI char* server_receive_from_client(BENJI_SOCKET client_socket) {
    char* data = NULL;
    size_t data_size = 0;

    char buffer[BENJI_BASIC_STRING_LENGTH];
    size_t bytes_received = 0;

    unsigned int tries = 0;

    do {
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received == BENJI_SOCKET_ERROR) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) {
                if (tries < BENJI_MAX_TRIES) {
                    tries++;

                    Sleep(50); // wait 50ms and try again

                    continue; // no data is available, just try again
                }
                else {
                    break;
                }
            }
        }

        buffer[bytes_received] = '\0';

        printf("Chunk received (%lli bytes): %s\n", bytes_received, buffer);

        data = realloc(data, data_size + bytes_received + 1);
        if (data == NULL) {
            free(data);

            return "";
        }

        memcpy(data + data_size, buffer, bytes_received + 1);

        data_size += bytes_received;
    } while (bytes_received > 0);

    printf("Full data received (%lli bytes): %s\n", data_size, data);

    return data;
}

BENJI_SC_ABI int server_send_to_client(BENJI_SOCKET client_socket, const char* data) {
    return send(client_socket, data, strlen(data) + 1, 0);
}

size_t server_parse_client_data(const char* client_data, char*** data_groups) {
    return splitstr(client_data, data_groups, ';');
}