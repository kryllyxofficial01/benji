#include "service_manager.hpp"

void service_main(int argc, char** argv) {
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

    nlohmann::json json = {
        {"test", 19},
        {"tset", 17}
    };
    std::string json_string = json.dump();

    server_config.ip = "127.0.0.1";
    server_config.port = 8080;

    server_config._socket = create_socket();

    server_connect(server_config.ip, server_config.port, server_config._socket);

    service.status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(service.status_handle, &service.status);

    std::string request;
    while (service.status.dwCurrentState == SERVICE_RUNNING) {
        request = json_data_to_post_request(json_string);

        send(server_config._socket, request.c_str(), request.size(), 0);

        Sleep(1000);

        service.status.dwCurrentState = SERVICE_RUNNING;
        SetServiceStatus(service.status_handle, &service.status);
    }
}

void control_handler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN: {
            service.status.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(service.status_handle, &service.status);

            // cleanup
            closesocket(server_config._socket);
            WSACleanup();

            service.status.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(service.status_handle, &service.status);

            break;
        }

        default: break;
    }
}