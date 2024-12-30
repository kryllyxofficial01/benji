#include "include/server.h"
#include "include/cpu_info.h"
#include "include/utils.h"

int main(int argc, const char* argv[]) {
    #if defined(_WIN32)
        winsock_init();
    #endif
}