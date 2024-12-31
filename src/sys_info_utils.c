#define BENJI_USE_SYS_INFO_UTILS

#include "include/utils.h"

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

        return run_command(command);
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

    const char* run_command(const char* command) {
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