#include "include/server.hpp"

void winsock_init() {
    WSAData wsa_data;

    OutputDebugStringA("Initializing Winsock...");

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        OutputDebugStringA("Failed");
        std::exit(EXIT_FAILURE);
    }

    OutputDebugStringA("Success");
}

SOCKET create_socket() {
    OutputDebugStringA("Creating socket...");

    SOCKET _socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket == INVALID_SOCKET) {
        OutputDebugStringA("Failed");

        WSACleanup();

        std::exit(EXIT_FAILURE);
    }

    OutputDebugStringA("Success");

    return _socket;
}

sockaddr_in server_connect(const char* ip, int port, SOCKET _socket) {
    sockaddr_in server;

    OutputDebugStringA((std::string("Connecting to server at ") + ip + ":" + std::to_string(port) + "...").c_str());

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);

    if (connect(_socket, (sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        OutputDebugStringA("Failed");

        closesocket(_socket);
        WSACleanup();

        std::exit(EXIT_FAILURE);
    }

    OutputDebugStringA("Success");

    return server;
}

void send_json_data(SOCKET _socket, JSON data) {
    std::string request = json_data_to_post_request(data);
    send(_socket, request.c_str(), request.size(), 0);
}

std::string json_data_to_post_request(JSON data) {
    std::string data_string = serialize_json(data);

    std::string request = "POST / HTTP/1.1\r\n";
    request += "Host: localhost:8080\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + std::to_string(data_string.length()) + "\r\n";
    request += "Connection: keep-alive\r\n\r\n";
    request += data_string;

    return request;
}