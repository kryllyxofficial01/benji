#ifndef __BENJI_SERVER_H
#define __BENJI_SERVER_H

#define BENJI_USE_SERVER_UTILS

#include "cpu_info.h"
#include "gpu_info.h"
#include "utils.h"

BENJI_SC_ABI BENJI_SOCKET server_init();
BENJI_SC_ABI void server_run(BENJI_SOCKET server_socket);

BENJI_SC_ABI BENJI_SOCKET server_accept_client(BENJI_SOCKET server_socket);
BENJI_SC_ABI char* server_receive_from_client(BENJI_SOCKET client_socket);
BENJI_SC_ABI int server_send_to_client(BENJI_SOCKET client_socket, const char* data); // returns -1 if failed after 3 tries

size_t server_parse_client_data(const char* client_data, char*** data_groups);

#endif