#include "logger.h"

void LogHelper(const char* function, int line, char* level, char* message) {
    printf("%s %s:%d: %s\n", level, function, line, message);
}

void LogDebug(const char *function, int line, char *message) {
    if(LOG_LEVEL >= 3) {
        LogHelper(function, line, "Debug", message);
    }
    return;
}

void LogInfo(const char *function, int line, char *message) {
    if(LOG_LEVEL >= 2) {
        LogHelper(function, line, "Info", message);
    }
    return;
}

void LogWarning(const char *function, int line, char *message) {
    if(LOG_LEVEL >= 1) {
        printf("\033[0;33m");
        LogHelper(function, line, "Warning", message);
        printf("\033[0m");
    }
    return;
}

void LogError(const char *function, int line, char *message) {
    printf("\033[0;31m");
    LogHelper(function, line, "Error", message);
    printf("\033[0m");
    return;
}

