#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util
{
	struct word {
		unsigned char byte[4];

		word(){};

		word(char c1, char c2, char c3, char c4) 
		{
			byte[0] = c1;
			byte[1] = c2;
			byte[2] = c3;
			byte[3] = c4;
		}

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

		void subWord();

		void rotWord();

		struct word operator^(unsigned char b[]);

		struct word operator^(word w);

	};
	
	std::string charToHex(unsigned char*, int size);
	std::string charToHex(unsigned char value);
	std::string wordToHex(struct word w);
	unsigned char* hexToChar(std::string s);
}

#endif
