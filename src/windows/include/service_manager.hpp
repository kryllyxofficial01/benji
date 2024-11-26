#ifndef _BENJI_SERVICE_MANAGER_HPP
#define _BENJI_SERVICE_MANAGER_HPP

#define UNICODE // i hate C sometimes

#include <iostream>

#include "server.hpp"
#include "system_info.hpp"
#include "logger.hpp"
#include "utils.hpp"

inline struct BENJI_SERVICE_STATUS {
    SERVICE_STATUS status;
    SERVICE_STATUS_HANDLE status_handle;
} service;

inline struct BENJI_SERVER_CONFIG {
    const char* ip;
    int port;
    SOCKET _socket;
} server_config;

void WINAPI service_main(int argc, char** argv);
void WINAPI control_handler(DWORD request);

void report_service_status(DWORD current_state, DWORD exit_code, DWORD wait_hint);

#endif