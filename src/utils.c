#include "include/utils.h"

#ifndef BENJI_NO_SERVER_INCLUDES
    #if defined(_WIN32)
        BENJI_ABI void winsock_init() {
            struct WSAData wsa_data;

            printf("Initializing Winsock2 ... ");

            if (WSAStartup(WINSOCK_VERSION, &wsa_data) != BENJI_NO_ERROR) {
                printf("Failed to initialize Winsock2");

                exit(EXIT_FAILURE);
            }

            printf("Success\n\n");
        }

        BENJI_ABI void winsock_cleanup() {
            if (WSACleanup() == BENJI_SOCKET_ERROR) {
                printf("Failed to cleanup Winsock2\n");

                exit(EXIT_FAILURE);
            }
        }
    #endif

    BENJI_ABI BENJI_SOCKET create_socket() {
        BENJI_SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock == BENJI_SOCKET_ERROR) {
            printf("Failed to close socket\n");

            #if defined(_WIN32)
                winsock_cleanup();
            #endif

            exit(EXIT_FAILURE);
        }

        return sock;
    }

    BENJI_ABI void close_socket(BENJI_SOCKET sock) {
        int return_code;

        #if defined(_WIN32)
            return_code = closesocket(sock);
        #elif defined(__linux__)
            return_code = close(sock);
        #endif

        if (return_code == BENJI_SOCKET_ERROR) {
            printf("Failed to close socket\n");

            #if defined(_WIN32)
                winsock_cleanup();
            #endif

            exit(EXIT_FAILURE);
        }
    }

    void stop(const size_t exit_code) {
        #if defined(_WIN32)
            winsock_cleanup();
        #endif

        exit(EXIT_FAILURE);
    }
#endif

#ifndef BENJI_NO_SYS_INFO_INCLUDES
    #if defined(_WIN32)
        const char* wmi_get_data(enum BENJI_WMI_TYPE type, enum BENJI_WMI_ENTRY entry) {
            char* command = malloc(sizeof(char) * 1024);

            command[0] = '\0';

            sprintf(
                command,
                "powershell.exe @(Get-WmiObject %s).%s",
                wmi_type_to_string(type),
                wmi_entry_to_string(entry)
            );

            return run_powershell_command(command);
        }

        const char* wmi_type_to_string(enum BENJI_WMI_TYPE type) {
            switch (type) {
                case WMI_WIN32_PROCESSOR: return "Win32_Processor";
                case WMI_WIN32_VIDEOCONTROLLER: return "Win32_VideoController";
            }
        }

        const char* wmi_entry_to_string(enum BENJI_WMI_ENTRY entry) {
            switch (entry) {
                case WMI_CPU_CLOCK_SPEED: return "CurrentClockSpeed";
                case WMI_CPU_CORE_COUNT: return "NumberOfCores";
                case WMI_CPU_LOGICAL_PROCESSORS_COUNT: return "NumberOfLogicalProcessors";

                case WMI_GPU_NAMES: return "Name";
                case WMI_GPU_CURRENT_HORIZONTAL_RESOLUTIONS: return "CurrentHorizontalResolution";
                case WMI_GPU_CURRENT_VERTICAL_RESOLUTIONS: return "CurrentVerticalResolution";
                case WMI_GPU_CURRENT_REFRESH_RATES: return "CurrentRefreshRate";
            }
        }

        const char* run_powershell_command(const char* command) {
            FILE* command_pipe = _popen(command, "r");
            if (command_pipe == NULL) {
                _pclose(command_pipe);

                // dont care about doing error checking, just assume the command will work as intended
                return NULL;
            }
            else {
                char* output = malloc(sizeof(char) * 1024);
                char buffer[1024];

                output[0] = '\0';

                while ((fgets(buffer, sizeof(buffer) - 1, command_pipe)) != NULL) {
                    strcat(output, buffer);
                }

                strtrim(output);

                _pclose(command_pipe);

                return output;
            }
        }
    #endif
#endif

void strtrim(char* string) {
    int i = 0;
    while (isspace(string[i])) {
        i++;
    }
    if (i > 0) {
        memmove(string, string + i, strlen(string) - i + 1);
    }

    int length = strlen(string);
    while (length > 0 && isspace(string[length - 1])) {
        length--;
    }
    string[length] = '\0';
}

size_t splitstr(const char* string, char*** tokens, const char* character) {
    char* temp_string = strdup(string);

    if (temp_string == NULL) {
        return 0;
    }

    size_t token_count = 0;
    char* token = strtok(temp_string, character);

    while (token != NULL) {
        char** temp_tokens = realloc(*tokens, (token_count + 1) * sizeof(char*));

        if (temp_tokens == NULL) {
            free(temp_tokens);

            break;
        }

        *tokens = temp_tokens;

        (*tokens)[token_count] = strdup(token);

        if ((*tokens)[token_count] == NULL) {
            break;
        }

        token_count++;
        token = strtok(NULL, character);
    }

    free(temp_string);

    return token_count;
}