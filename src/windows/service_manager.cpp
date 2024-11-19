#include "include/service_manager.hpp"

void WINAPI service_main(int argc, char** argv) {
    service.status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service.status.dwCurrentState = SERVICE_START_PENDING;
    service.status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    service.status.dwWin32ExitCode = 0;
    service.status.dwServiceSpecificExitCode = 0;

    service.status.dwCheckPoint = 0;
    service.status.dwWaitHint = 0;

    service.status_handle = RegisterServiceCtrlHandler(
        TEXT("ServerManagerService"),
        (LPHANDLER_FUNCTION) control_handler
    );

    if (service.status_handle == NULL) {
        std::cerr << "RegisterServiceCtrlHandler failed: " << WSAGetLastError() << std::endl;
        return;
    }

    std::map<std::string, std::string> json_data = {
        {"test", "17"},
        {"tset", "19"}
    };
    std::string json_string = serialize_map(json_data);

    server_config.ip = "127.0.0.1";
    server_config.port = 8080;

    server_config._socket = create_socket();

    server_connect(server_config.ip, server_config.port, server_config._socket);

    report_service_status(SERVICE_RUNNING, NO_ERROR, 0);

    std::string request = json_data_to_post_request(json_string);
    send(server_config._socket, request.c_str(), request.size(), 0);

    boolean running = true;
    while (running) {
        json_data["test"] = std::to_string(stoi(json_data["test"]) + 1);
        json_string = serialize_map(json_data);

        OutputDebugStringA(json_string.c_str());

        request = json_data_to_post_request(json_string);
        send(server_config._socket, request.c_str(), request.size(), 0);

        Sleep(5000);
    }
}

void WINAPI control_handler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN: {
            OutputDebugStringA("Stopping service...");

            report_service_status(SERVICE_STOP_PENDING, NO_ERROR, 0);

            // cleanup
            closesocket(server_config._socket);
            WSACleanup();

            report_service_status(SERVICE_STOPPED, NO_ERROR, 0);

            OutputDebugStringA("Stopped");

            break;
        }

        default: {
            report_service_status(service.status.dwCurrentState, NO_ERROR, 0);

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