#include "include/server.h"
#include "include/utils.h"

int main(int argc, const char* argv[]) {
    winsock_init();

    SOCKET server_socket = create_server();

    closesocket(server_socket);
}