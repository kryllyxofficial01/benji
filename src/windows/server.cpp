#include "include/server.hpp"

void winsock_init() {
    WSAData wsa_data;

    std::cout << "Initializing Winsock..." << std::endl;

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Success" << std::endl;
}

SOCKET create_socket() {
    std::cout << "Creating socket..." << std::endl;

    SOCKET _socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;

        WSACleanup();

        std::exit(EXIT_FAILURE);
    }

    std::cout << "Success" << std::endl;

    return _socket;
}

sockaddr_in server_connect(const char* ip, int port, SOCKET _socket) {
    sockaddr_in server;

    std::cout << "Connecting to server at " << ip << ":" << port << "..." << std::endl;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);

    if (connect(_socket, (sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;

        closesocket(_socket);
        WSACleanup();

        std::exit(EXIT_FAILURE);
    }

    std::cout << "Success" << std::endl;

    return server;
}

std::string json_data_to_post_request(std::string data) {
    std::string request = "POST / HTTP/1.1\r\n";
    request += "Host: localhost:8080\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + std::to_string(data.length()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += data;

    return request;
}