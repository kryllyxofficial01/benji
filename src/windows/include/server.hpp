#ifndef _BENJI_SERVER_HPP
#define _BENJI_SERVER_HPP

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "logger.hpp"
#include "utils.hpp"

void winsock_init();

sockaddr_in create_server(const char* ip, int port, SOCKET sock);

void handle_client(SOCKET client_socket);
std::string receive_data(SOCKET client_socket);

std::string parse_http_request(const std::string& request, std::string& method, std::string& body, bool& keep_alive);

SOCKET create_socket();
void close_socket(SOCKET sock);

sockaddr_in server_connect(const char* ip, int port, SOCKET sock);

void send_json_data(SOCKET sock, json_t data);

void server_cleanup(SOCKET sock);
void winsock_cleanup();

#endif