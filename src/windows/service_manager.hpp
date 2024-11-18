#ifndef _SERVICE_MANAGER_HPP
#define _SERVICE_MANAGER_HPP

#define UNICODE // i hate C sometimes

#include <iostream>

#include "server.hpp"

#include <windows.h>

struct {
    SERVICE_STATUS status;
    SERVICE_STATUS_HANDLE status_handle;
} service;

struct {
    const char* ip;
    int port;
    SOCKET _socket;
} server_config;

void service_main(int argc, char** argv);
void control_handler(DWORD request);

#endif