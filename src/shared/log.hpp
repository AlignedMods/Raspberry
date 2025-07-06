#pragma once

//#include <print>
#include <iostream>

inline static void Error(const char* message, int errorCode) {
    //std::println("\033[1;31mERROR: \033[0m {} \n", message);
	std::cout << "ERROR: " << message << std::endl;
    exit(errorCode);
}

template <typename T>
inline static void Debug(T message) {
#ifdef RDEBUG
    //std::println("\033[1;32mDEBUG: \033[0m {} \n", message);
	std::cout << "DEBUG: " << message << std::endl;
#endif
}

// peeepepepepep
