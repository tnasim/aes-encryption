#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util
{
	struct Word {
		unsigned char byte[4];
		
		void setBytes(unsigned char b[4]) {
			for(int i = 0; i < 4; i++) {
				byte[i] = b[i];
			}
		}
		
		void setByte(unsigned char b, int pos) {
			byte[pos] = b;
		}
		
		unsigned char getByte(int pos) {
			return byte[pos];
		}
	};
	
	std::string charToHex(unsigned char*, int size);
	unsigned char* hexToChar(std::string s);
}

#endif
