#pragma once

#include <iostream>
#include <format>

enum class LogLevel {
    Info     = 0,
    Debug    = 1,
    Warning  = 2,
    Error    = 3,
    Critical = 4
};

inline void RInfo(const std::string& message) {
    std::cout << "INFO: " << message << std::endl;
}

inline void Debug(const std::string& message) {
    std::cout << "DEBUG: " << message << std::endl;
}

inline void Warning(const std::string& message) {
    std::cout << "WARNING: " << message << std::endl;
}

inline void Error(const std::string& message) {
    std::cout << "ERROR: " << message << std::endl;
}

inline void Critical(const std::string& message) {
    std::cout << "CRITICAL: " << message << std::endl;
    std::exit(-1);
}

inline static void Log(LogLevel level, const std::string& message) {
    switch (level) {
        case LogLevel::Info:
            RInfo(message);
            break;

        case LogLevel::Debug:
            Debug(message);
            break;

        case LogLevel::Warning:
            Warning(message);
            break;

        case LogLevel::Error:
            Error(message);
            break;

        case LogLevel::Critical:
            Critical(message);
            break;

        default:
            Warning(std::format("Non-existant LogLevel provided"));
            break;
    }
}
