#include "util.h"
#include <sstream>
#include <string>

std::string util::charToHex(char c[], int size)
{
	std::stringstream ss;
	for (int i = 0; i <size; i++)
	{
		ss << std::hex << (int)c[i];
		if (i < size-1)
			ss << " ";
	}
	return ss.str();
}