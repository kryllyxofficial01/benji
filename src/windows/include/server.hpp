#ifndef _BENJI_SERVER_HPP
#define _BENJI_SERVER_HPP

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "utils.hpp"
#include "logger.hpp"

void winsock_init();

SOCKET create_socket();
sockaddr_in server_connect(const char* ip, int port, SOCKET _socket);

void send_json_data(SOCKET _socket, json_t data);

void server_cleanup(SOCKET _socket);
void winsock_cleanup();
void close_socket(SOCKET _socket);

#endif