#ifndef __BENJI_SERVER_H
#define __BENJI_SERVER_H

#ifndef BENJI_USE_SERVER_UTILS
    #define BENJI_USE_SERVER_UTILS
#endif

#ifndef BENJI_USE_SYS_INFO_UTILS
    #define BENJI_USE_SYS_INFO_UTILS
#endif

#include "utils.h"
#include "hardware.h"
#include "logger.h"

BENJI_SC_ABI result_t* server_init();
BENJI_SC_ABI result_t* server_run(BENJI_SOCKET server_socket);

BENJI_SC_ABI result_t* server_handle_client(BENJI_SOCKET server_socket, char*** data_groups, size_t* data_group_count);
BENJI_SC_ABI result_t* server_accept_client(BENJI_SOCKET server_socket);
BENJI_SC_ABI result_t* server_receive_from_client(BENJI_SOCKET client_socket);
BENJI_SC_ABI result_t* server_send_to_client(BENJI_SOCKET client_socket, const char* data);

size_t server_parse_client_data(const char* client_data, char*** data_groups);

#endif