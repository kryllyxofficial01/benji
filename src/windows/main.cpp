#include <iostream>
#include <windows.h>

SERVICE_STATUS service_status;
SERVICE_STATUS_HANDLE service_status_handle;

void service_main(int argc, char** argv);
void control_handler(DWORD request);

int main(int argc, char** argv) {
    SERVICE_TABLE_ENTRY service_table[] = {
        {
            (LPSTR) TEXT("ServerManagerService"),
            (LPSERVICE_MAIN_FUNCTION) service_main
        },
        {NULL, NULL}
    };

    if (!StartServiceCtrlDispatcher(service_table)) {
        std::cerr << "StartServiceCtrlDispatcher failed: " << GetLastError() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void service_main(int argc, char** argv) {
    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service_status.dwCurrentState = SERVICE_START_PENDING;
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    service_status.dwWin32ExitCode = 0;
    service_status.dwServiceSpecificExitCode = 0;

    service_status.dwCheckPoint = 0;
    service_status.dwWaitHint = 0;

    service_status_handle = RegisterServiceCtrlHandler(
        TEXT("ServerManagerService"),
        (LPHANDLER_FUNCTION) control_handler
    );

    if (service_status_handle == NULL) {
        std::cerr << "RegisterServiceCtrlHandler failed: " << GetLastError() << std::endl;
        return;
    }

    // daemon work goes here

    service_status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(service_status_handle, &service_status);

    while (service_status.dwCurrentState == SERVICE_RUNNING) {
        Sleep(1000);
    }
}

void control_handler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN: {
            service_status.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(service_status_handle, &service_status);

            // do cleanup stuff here

            service_status.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(service_status_handle, &service_status);

            break;
        }

        default: break;
    }
}