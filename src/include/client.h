#ifndef __BENJI_CLIENT_H
#define __BENJI_CLIENT_H

#define BENJI_USE_SERVER_UTILS

#include "utils.h"

BENJI_SC_ABI BENJI_SOCKET accept_client(BENJI_SOCKET server_socket);
BENJI_SC_ABI char* receive_from_client(BENJI_SOCKET client_socket);

#endif