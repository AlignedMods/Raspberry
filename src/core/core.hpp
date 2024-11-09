#pragma once

#include "pch.hpp"

inline static void Error(const char* message, int errorCode) {
	std::cerr << "\n\n\033[1;31mERROR: \033[0m" << message << "\n\n\n";
	exit(errorCode);
}

template <typename T>
inline static void Debug(T msg) {
	#ifdef RDEBUG
	   std::cout << "\n\n\033[1;32mDEBUG: \033[0m" << msg << "\n\n\n";
	#endif
}


//peeepepepepep
