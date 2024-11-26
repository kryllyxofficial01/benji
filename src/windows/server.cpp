#include "include/server.hpp"

void winsock_init() {
    WSAData wsa_data;

    LogInfo("Initializing Winsock...");

    int winsock_init_code = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (winsock_init_code != 0) {
        LogCritical(std::string("Init failed with error code ") + std::to_string(winsock_init_code));
        std::exit(EXIT_FAILURE);
    }

    LogInfo("Success");
}


SOCKET create_socket() {
    LogInfo("Creating default TCP socket with IPv4 address family...");

    SOCKET _socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket == INVALID_SOCKET) {
        LogCritical("Failed, socket declared invalid");

        winsock_cleanup();

        std::exit(EXIT_FAILURE);
    }

    LogInfo("Success");

    return _socket;
}

void close_socket(SOCKET _socket) {
    LogInfo("Closing socket...");

    if (closesocket(_socket) == SOCKET_ERROR) {
        LogCritical(std::string("Close failed with error code ") + std::to_string(WSAGetLastError()));
    }
    else {
        LogInfo("Success");
    }
}

sockaddr_in server_connect(const char* ip, int port, SOCKET _socket) {
    sockaddr_in server;

    LogInfo(std::string("Connecting to server at ") + ip + ":" + std::to_string(port) + "...");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);

    if (connect(_socket, (sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        LogCritical("Failed to connect");

        server_cleanup(_socket);

        std::exit(EXIT_FAILURE);
    }

    LogInfo("Success");

    return server;
}

void send_json_data(SOCKET _socket, json_t data) {
    std::string request = json_data_to_post_request(data);

    LogInfo(std::string("Sending data: ") + serialize_json(data));

    if (send(_socket, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
        LogError(std::string("Failed to send with error code ") + std::to_string(WSAGetLastError()));
    }
    else {
        LogInfo("Success");
    }
}

void server_cleanup(SOCKET _socket) {
    close_socket(_socket);
    winsock_cleanup();
}

void winsock_cleanup() {
    LogInfo("Cleaning up Winsock...");

    if (WSACleanup() == SOCKET_ERROR) {
        LogError(std::string("Cleanup failed with error code ") + std::to_string(WSAGetLastError()));
    }
    else {
        LogInfo("Success");
    }
}