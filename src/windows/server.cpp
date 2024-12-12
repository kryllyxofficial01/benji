#include "include/server.hpp"

void winsock_init() {
    WSAData wsa_data;

    LogInfo("Initializing Winsock...");

    int winsock_init_code = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (winsock_init_code != 0) {
        LogCritical("Init failed with error code " + std::to_string(winsock_init_code));
        std::exit(EXIT_FAILURE);
    }

    LogInfo("Success");
}

SOCKET create_socket() {
    LogInfo("Creating default TCP server socket with IPv4 address family...");

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        LogCritical("Failed, socket declared invalid");

        winsock_cleanup();

        std::exit(EXIT_FAILURE);
    }

    LogInfo("Success");

    return sock;
}

sockaddr_in create_server(const char* ip, int port, SOCKET sock) {
    LogInfo(std::string("Creating server at ") + ip + ":" + std::to_string(port) + "...");

    // allow for socket reuse
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, "1", sizeof("1")); // "1" == true

    sockaddr_in server;

    server.sin_family = AF_INET; // ipv4 address family
    server.sin_addr.S_un.S_addr = inet_addr(ip);
    server.sin_port = htons(port);

    // bind socket to ip and port
    LogInfo("Binding socket to address...");
    if (bind(sock, (sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        LogCritical(std::string("Failed to bind socket to ") + ip + ":" + std::to_string(port) + " with error code " + std::to_string(WSAGetLastError()));

        winsock_cleanup();

        std::exit(EXIT_FAILURE);
    }
    LogInfo("Success");

    LogInfo("Attempting to put socket in listening mode...");
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        LogCritical("Failed to put socket in listening mode" + std::to_string(WSAGetLastError()));

        winsock_cleanup();

        std::exit(EXIT_FAILURE);
    }
    LogInfo("Success");

    LogInfo("Server created and running");

    return server;
}

void run_server(SOCKET server_socket) {
    server_state = SERVER_STATE::RUNNING;

    while (true) {
        LogInfo(std::to_string(server_state));

        if (server_state == SERVER_STATE::STOPPING) {
            server_state = SERVER_STATE::STOPPED;
            break;
        }

        sockaddr_in client;

        int client_size = sizeof(client);
        SOCKET client_socket = accept(server_socket, (sockaddr*) &client, &client_size);

        u_long imode = 1;
        if (ioctlsocket(client_socket, FIONBIO, &imode) != 0) {
            LogError("Failed to set client socket to non-blocking mode with error code " + std::to_string(WSAGetLastError()));

            close_socket(client_socket);

            continue;
        }

        if (client_socket == INVALID_SOCKET) {
            LogError("Failed to accept new client with error code " + std::to_string(WSAGetLastError()));

            close_socket(client_socket);

            continue;
        }

        int error = 0;
        socklen_t error_code_size = sizeof(error);

        if (getsockopt(client_socket, SOL_SOCKET, SO_ERROR, (char*) &error, &error_code_size) != 0) {
            LogError("Unable to get error code from getsockopt()");

            close_socket(client_socket);

            continue;
        }

        if (error != 0) {
            LogError("getsockopt() failed with exit code " + std::to_string(WSAGetLastError()));

            close_socket(client_socket);

            continue;
        }

        LogInfo("New client connected");

        handle_client(client_socket);

        close_socket(client_socket); // once the client disconnects, close the socket
    }
}

void handle_client(SOCKET client_socket) {
    bool keep_alive = true;

    while (keep_alive) {
        if (server_state == SERVER_STATE::STOPPING) {
            keep_alive = false;

            close_socket(client_socket);

            continue;
        }

        std::string request = receive_data(client_socket);
        std::string method, body;

        std::string path = parse_http_request(request, method, body, keep_alive);

        std::string response;
        if (method == "GET") { // GET request
            const std::string response_body = "received GET request"; // TODO: replace with actual JSON data

            response =  "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: text/plain\r\n"; // TODO: change to "application/json" once JSON data is collected
            response += "Content-Length: " + std::to_string(response_body.size()) + "\r\n";
            response += (keep_alive ? "Connection: keep-alive" : "Connection: close");
            response += "\r\n\r\n";
            response += response_body;
        }
        else if (method == "POST") { // POST request
            const std::string response_body = "received POST request: " + body; // maybe make something more "official"?

            response =  "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: text/plain\r\n";
            response += "Content-Length: " + std::to_string(response_body.size()) + "\r\n";
            response += (keep_alive ? "Connection: keep-alive" : "Connection: close");
            response += "\r\n\r\n";
            response += response_body;
        }
        else {
            response =  "HTTP/1.1 405 Method Not Allowed\r\n";
            response += "Content-Length: 0\r\n";
            response += (keep_alive ? "Connection: keep-alive" : "Connection: close");
            response += "\r\n";

            LogError("405 Method Not Allowed: " + method);
        }

        LogInfo("Sending response to client...");

        if (send(client_socket, response.c_str(), response.size(), 0) == SOCKET_ERROR) {
            LogError("Failed to send with error code " + std::to_string(WSAGetLastError()));
        }
        else {
            LogInfo("Success");
        }

        if (!keep_alive) {
            LogInfo("Connection closed by client");
        }
    }
}

std::string receive_data(SOCKET client_socket) {
    std::string data;

    char buffer[4096];
    size_t bytes_received = 0;

    do {
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        // check if all data has been received
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            data += buffer;
        }
        else if (bytes_received == SOCKET_ERROR) {
            LogCritical("recv() failed with error code " + std::to_string(WSAGetLastError()));

            winsock_cleanup();

            std::exit(EXIT_FAILURE);
        }
    } while (bytes_received > 0);

    return data;
}

std::string parse_http_request(const std::string& request, std::string& method, std::string& body, bool& keep_alive) {
    std::string path;

    std::istringstream stream(request);
    std::string line;

    std::getline(stream, line); // get request line

    // get method and path
    std::istringstream line_stream(line);
    line_stream >> method;
    line_stream >> path;

    // check for keep-alive connection
    keep_alive = false;
    while (std::getline(stream, line) && line != "\r") {
        keep_alive = (line.find("Connection: keep-alive") != std::string::npos); // kinda janky but if it works it works
    }

    // if POST request, fetch the content
    if (method == "POST") {
        size_t position = request.find("\r\n\r\n");

        if (position != std::string::npos) {
            body = request.substr(position + 4);
        }
        else {
            LogError("Received malformed POST request, unable to locate body content");
        }
    }

    return path;
}

// sockaddr_in server_connect(const char* ip, int port, SOCKET sock) {
//     sockaddr_in server;

//     LogInfo(std::string("Connecting to server at ") + ip + ":" + std::to_string(port) + "...");

//     server.sin_family = AF_INET;
//     server.sin_port = htons(port);
//     inet_pton(AF_INET, ip, &server.sin_addr);

//     if (connect(sock, (sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
//         LogCritical("Failed to connect with error code" + std::to_string(WSAGetLastError()));

//         server_cleanup(sock);

//         std::exit(EXIT_FAILURE);
//     }

//     LogInfo("Success");

//     return server;
// }

// void send_json_data(SOCKET sock, json_t data) {
//     std::string request = json_data_to_post_request(data);

//     LogInfo(std::string("Sending data: ") + serialize_json(data));

//     if (send(sock, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
//         LogError("Failed to send with error code " + std::to_string(WSAGetLastError()));
//     }
//     else {
//         LogInfo("Success");
//     }
// }

void close_socket(SOCKET sock) {
    LogInfo("Closing socket...");

    if (closesocket(sock) == SOCKET_ERROR) {
        LogCritical("Close failed with error code " + std::to_string(WSAGetLastError()));
    }
    else {
        LogInfo("Success");
    }
}

void server_cleanup(SOCKET sock) {
    close_socket(sock);

    // server_state = SERVER_STATE::STOPPING;

    // LogInfo("Waiting for server to close...");

    // while (server_state != SERVER_STATE::STOPPED) {}

    winsock_cleanup();
}

void winsock_cleanup() {
    LogInfo("Cleaning up Winsock...");

    if (WSACleanup() == SOCKET_ERROR) {
        LogError("Cleanup failed with error code " + std::to_string(WSAGetLastError()));
    }
    else {
        LogInfo("Success");
    }
}