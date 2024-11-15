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
        std::cerr << "RegisterServiceCtrlHandler failed: " << GetLastError() << std::endl;
        return;
    }

    // daemon work goes here

    service.status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(service.status_handle, &service.status);

    while (service.status.dwCurrentState == SERVICE_RUNNING) {
        Sleep(1000);
    }
}

void control_handler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN: {
            service.status.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(service.status_handle, &service.status);

            // do cleanup stuff here

            service.status.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(service.status_handle, &service.status);

            break;
        }

        default: break;
    }
}