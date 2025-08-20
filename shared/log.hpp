#pragma once

#include "types.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>

enum class Log : u32 {
    Info     = 0,
    Debug    = 1,
    Warning  = 2,
    Error    = 3,
    Critical = 4
};

inline static void Log(Log level, const char* message, ...) {
    bool showWarning = false;

    va_list args;
    va_start(args, message);

    char buffer[1024]{};

    switch (level) {
        case Log::Info:     strcpy(buffer, "[INFO]: ");    break;
        case Log::Debug:    strcpy(buffer, "[DEBUG]: ");   break;
        case Log::Warning:  strcpy(buffer, "[WARNING]: "); break;
        case Log::Error:    strcpy(buffer, "[ERROR]: ");   break;
        case Log::Critical: strcpy(buffer, "[CRITIAL]: "); break;

        default: showWarning = true; break;
    }

    strcat(buffer, message);
    strcat(buffer, "\n");
    vprintf(buffer, args);
    fflush(stdout);

    va_end(args);
}
