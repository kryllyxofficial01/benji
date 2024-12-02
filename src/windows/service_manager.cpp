#include "include/service_manager.hpp"

void WINAPI service_main(int argc, char** argv) {
    service.status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service.status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    service.status.dwServiceSpecificExitCode = 0;
    service.status.dwCheckPoint = 0;

    report_service_status(SERVICE_START_PENDING, 0, 0);

    service.status_handle = RegisterServiceCtrlHandler(
        TEXT("BenjiService"),
        (LPHANDLER_FUNCTION) control_handler
    );

    if (service.status_handle == NULL) {
        LogCritical(std::string("RegisterServiceCtrlHandler failed with error code ") + std::to_string(WSAGetLastError()));
        std::exit(EXIT_FAILURE);
    }

    server_config.ip = "127.0.0.1";
    server_config.port = 8080;

    server_config.sock = create_socket();

    // server_connect(server_config.ip, server_config.port, server_config.sock);
    create_server(server_config.ip, server_config.port, server_config.sock);

    report_service_status(SERVICE_RUNNING, 0, 0);

    boolean running = true;
    while (running) {
        Sleep(1000); // TODO: make this a configurable value, will be the update period for the main dashboard
    }
}

void WINAPI control_handler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN: {
            LogInfo("Stopping service...");

            report_service_status(SERVICE_STOP_PENDING, 0, 0);

            // cleanup
            server_cleanup(server_config.sock);

            report_service_status(SERVICE_STOPPED, 0, 0);

            LogInfo("Service stopped");

            break;
        }

        default: {
            report_service_status(service.status.dwCurrentState, 0, 0);

            break;
        }
    }
}

void report_service_status(DWORD current_state, DWORD exit_code, DWORD wait_hint) {
    service.status.dwCurrentState = current_state;
    service.status.dwWin32ExitCode = exit_code;
    service.status.dwWaitHint = wait_hint;

    SetServiceStatus(service.status_handle, &service.status);
}