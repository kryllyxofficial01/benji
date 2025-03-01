#include "include/server.h"

int main(int argc, const char* argv[]) {
    #ifdef _WIN32
        winsock_init();
    #endif

    result_t* server_socket_result = server_init();
    if (server_socket_result->is_error) {
        log_error(server_socket_result);

        result_free(server_socket_result);

        terminate(server_socket_result->payload.error.code);
    }

    BENJI_SOCKET server_socket = (BENJI_SOCKET) (uintptr_t) result_unwrap(server_socket_result);

    result_t* server_status_result = server_run(server_socket);
    if (server_status_result->is_error) {
        log_error(server_status_result);

        result_free(server_status_result);

        result_t* close_server_socket_result = close_socket(server_socket);
        if (close_server_socket_result->is_error) {
            log_error(close_server_socket_result);

            result_free(close_server_socket_result);

            terminate(close_server_socket_result->payload.error.code);
        }

        result_free(close_server_socket_result);

        terminate(server_status_result->payload.error.code);
    }

    result_t* close_server_socket_result = close_socket(server_socket);
    if (close_server_socket_result->is_error) {
        log_error(close_server_socket_result);

        result_free(close_server_socket_result);

        terminate(close_server_socket_result->payload.error.code);
    }

    result_free(close_server_socket_result);

    #ifdef _WIN32
        winsock_cleanup();
    #endif

    return EXIT_SUCCESS;
}