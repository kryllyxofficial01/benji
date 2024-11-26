#include <iostream>

#include "include/service_manager.hpp"
#include "include/system_info.hpp"
#include "include/utils.hpp"

int main(int argc, char** argv) {
    winsock_init();

    SERVICE_TABLE_ENTRY service_dispatch_table[] = {
        {
            TEXT("BenjiService"),
            (LPSERVICE_MAIN_FUNCTION) service_main
        },
        {NULL, NULL}
    };

    if (!StartServiceCtrlDispatcher(service_dispatch_table)) {
        LogCritical(std::string("StartServiceCtrlDispatcher failed with error code ") + std::to_string(WSAGetLastError()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}