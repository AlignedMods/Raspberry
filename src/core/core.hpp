#pragma once

#include <cstdlib>
#include <iostream>

inline void Error(const char* message, int errorCode) {
	std::cerr << "\n\n\033[1;31mERROR: \033[0m" << message << "\n\n\n";
	exit(errorCode);
}
