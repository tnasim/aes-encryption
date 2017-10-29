#include "util.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

std::string util::charToHex(unsigned char *c, int size)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i <size; i++)
	{
		ss << std::setw(2) << static_cast<unsigned>(c[i]);
		if (i < size-1)
			ss << " ";
	}
	return ss.str();
}

unsigned char* util::hexToChar(std::string s) {
	// Remove spaces (if any) from the hex string
	std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
	s.erase(end_pos, s.end());
	
	const char *hex_arr = s.c_str();
	unsigned char* char_arr = new unsigned char[s.length()/2+1];
	
	for( unsigned i = 0, uchr ; i < s.length() ; i += 2 ) {
		sscanf( hex_arr+ i, "%2x", &uchr ); // conversion
		char_arr[i/2] = (unsigned char)uchr; // save as char
	}
	char_arr[s.length()/2+1] = '\0';
	return char_arr;
	
}
