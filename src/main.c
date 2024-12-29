#include "include/server.h"
#include "include/utils.h"

int main(int argc, const char* argv[]) {
    #if defined(_WIN32)
        winsock_init();
    #endif

    SOCKET server_socket = create_server();

    close_socket(server_socket);
}