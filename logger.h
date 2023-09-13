#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define LOG_LEVEL 3

void LogDebug(const char* function, int line, char* format);
void LogInfo(const char* function, int line, char* format);
void LogWarning(const char* function, int line, char* format);
void LogError(const char* function, int line, char* format);


#endif

