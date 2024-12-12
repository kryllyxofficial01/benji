#ifndef _BENJI_LOGGER_HPP
#define _BENJI_LOGGER_HPP

#include <windows.h>
#include <string>

#define LogInfo(msg) OutputDebugStringA(( \
    std::string("[BenjiService, INFO] ") + (msg) \
).c_str())

#define LogDebug(msg) OutputDebugStringA(( \
    std::string("[BenjiService, DEBUG] ") + (msg) \
).c_str())

#define LogWarning(msg) OutputDebugStringA(( \
    std::string("[BenjiService, WARNING] ") + (msg) \
).c_str())

#define LogError(msg) OutputDebugStringA(( \
    std::string("[BenjiService, ERROR] ") + (msg) \
).c_str())

#define LogCritical(msg) OutputDebugStringA(( \
    std::string("[BenjiService, CRITICAL] ") + (msg) \
).c_str())

#endif