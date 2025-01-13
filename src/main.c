#include "include/server.h"
#include "include/cpu_info.h"
#include "include/utils.h"

int main(int argc, const char* argv[]) {
    #if defined(_WIN32)
        winsock_init();
    #endif

    const char* string = "cpu_all;gpu_all";
    char** tokens;

    int count = splitstr(string, &tokens, ';');

    for (int i = 0; i < count; i++) {
        printf("%s\n", tokens[i]);
    }

    BENJI_SOCKET server_socket = server_init();

    server_run(server_socket);

    close_socket(server_socket);
}