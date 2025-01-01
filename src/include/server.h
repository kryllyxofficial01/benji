#ifndef __BENJI_SERVER_H
#define __BENJI_SERVER_H

#define BENJI_USE_SERVER_UTILS

#include "client.h"
#include "cpu_info.h"
#include "utils.h"

BENJI_SC_ABI BENJI_SOCKET create_server();
BENJI_SC_ABI void run_server(BENJI_SOCKET server_socket);

#endif