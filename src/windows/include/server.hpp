#ifndef _BENJI_SERVER_HPP
#define _BENJI_SERVER_HPP

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "utils.hpp"

void winsock_init();
SOCKET create_socket();
sockaddr_in server_connect(const char* ip, int port, SOCKET _socket);

void send_json_data(SOCKET _socket, JSON data);

std::string json_data_to_post_request(JSON data);

#endif