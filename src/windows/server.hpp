#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "include/nlohmann-json.hpp"

void winsock_init();
SOCKET create_socket();
sockaddr_in server_connect(const char* ip, int port, SOCKET _socket);

#endif