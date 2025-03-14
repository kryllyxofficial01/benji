#include "include/service.h"

BENJIAPI void service_main(unsigned long argc, LPTSTR* argv) {
    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    service_status.dwServiceSpecificExitCode = 0;
    service_status.dwCheckPoint = 0;

    report_service_status(SERVICE_START_PENDING, 0, 0);

    service_status_handle = RegisterServiceCtrlHandler(
        TEXT("BenjiService"),
        (LPHANDLER_FUNCTION) service_control_handler
    );

    if (service_status_handle == NULL) {
        // TODO: error catching + logging

        return;
    }

    result_t* server_socket_result = server_init();
    if (server_socket_result->is_error) {
        result_error_payload_t server_socket_result_error = result_unwrap_error(server_socket_result);

        log_error(server_socket_result_error);

        return;
    }

    server_socket = (BENJI_SOCKET) (uintptr_t) result_unwrap_value(server_socket_result);

    report_service_status(SERVICE_RUNNING, 0, 0);

    HANDLE worker_thread = CreateThread(NULL, 0, service_worker_thread, NULL, 0, NULL);

    // basically run this indefinitely and rely on the thread to close properly
    WaitForSingleObject(worker_thread, INFINITE);
}

BENJIAPI void service_control_handler(unsigned long request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN: {
            report_service_status(SERVICE_STOP_PENDING, 0, 0);

            #ifdef _WIN32
                winsock_cleanup();
            #endif

            report_service_status(SERVICE_STOPPED, 0, 0);

            break;
        }

        default: {
            report_service_status(service_status.dwCurrentState, 0, 0);

            break;
        }
    }
}

BENJIAPI unsigned long service_worker_thread() {
    while (WaitForSingleObject(service_stop_event, 0) != WAIT_OBJECT_0) {
        result_t* server_update_result = server_update(server_socket);
        if (server_update_result->is_error) {
            result_error_payload_t server_update_result_error = result_unwrap_error(server_update_result);

            log_warning(server_update_result_error);
        }
    }

    return BENJI_NO_ERROR;
}

BENJIAPI void report_service_status(unsigned long current_state, unsigned long exit_code, unsigned long wait_hint) {
    service_status.dwCurrentState = current_state;
    service_status.dwWin32ExitCode = exit_code;
    service_status.dwWaitHint = wait_hint;

    SetServiceStatus(service_status_handle, &service_status);
}