#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util
{
	std::string charToHex(unsigned char*, int size);
	unsigned char* hexToChar(std::string s);
}

#endif
