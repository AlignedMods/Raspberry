#pragma once

#include <cstdarg>
#include <cstdlib>
#include <iostream>

inline static void Error(const char* message, int errorCode) {
	std::cerr << "\n\n\033[1;31mERROR: \033[0m" << message << "\n\n\n";
	exit(errorCode);
}

template <typename T>
inline static void Debug(T msg) {
	#ifdef RDEBUG
	   std::cout << msg << "\n";
	#endif
}


//peeepepepepep
