#include "include/server.h"
#include "include/cpu_info.h"
#include "include/utils.h"

int main(int argc, const char* argv[]) {
    #if defined(_WIN32)
        winsock_init();
    #endif

    BENJI_SOCKET server_socket = (BENJI_SOCKET) (uintptr_t) result_unwrap(server_init());

    server_run(server_socket);

    close_socket(server_socket);

    winsock_cleanup();

    return EXIT_SUCCESS;
}