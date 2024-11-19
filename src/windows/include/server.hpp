#ifndef _SERVER_HPP
#define _SERVER_HPP

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

void winsock_init();
SOCKET create_socket();
sockaddr_in server_connect(const char* ip, int port, SOCKET _socket);

std::string json_data_to_post_request(std::string data);

#endif