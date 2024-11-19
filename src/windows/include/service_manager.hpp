#ifndef _SERVICE_MANAGER_HPP
#define _SERVICE_MANAGER_HPP

#define UNICODE // i hate C sometimes

#include <iostream>

#include "server.hpp"
#include "utils.hpp"

struct {
    SERVICE_STATUS status;
    SERVICE_STATUS_HANDLE status_handle;
} service;

struct {
    const char* ip;
    int port;
    SOCKET _socket;
} server_config;

void WINAPI service_main(int argc, char** argv);
void WINAPI control_handler(DWORD request);

void report_service_status(DWORD current_state, DWORD exit_code, DWORD wait_hint);

#endif