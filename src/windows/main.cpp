#include <iostream>

#include "service_manager.hpp"
#include "server.hpp"

int main(int argc, char** argv) {
    SERVICE_TABLE_ENTRY service_table[] = {
        {
            TEXT("ServiceManagerService"),
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