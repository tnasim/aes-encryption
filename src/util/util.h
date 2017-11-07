#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdio>

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

		word(std::string s) {
			std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
			s.erase(end_pos, s.end());

			const char *hex_arr = s.c_str();
			unsigned char* char_arr = new unsigned char[s.length()/2+1];

			for( unsigned i = 0, uchr ; i < s.length() ; i += 2 ) {
				sscanf( hex_arr+ i, "%2x", &uchr ); // conversion
				byte[i/2] = (unsigned char)uchr; // save as char
			}
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

		std::string hex() {
			std::stringstream ss;
			ss << std::hex << std::setfill('0');

			for (int i = 0; i < 4; i++) {
				if (i == 3)
					ss << std::setw(2) << static_cast<unsigned>(getByte(i));
				else
					ss << std::setw(2) << static_cast<unsigned>(getByte(i)) << " ";
			}
			return ss.str();
		}

		void subWord();

		void rotWord();

		struct word operator^(unsigned char b[]);

		struct word operator^(word w);

		bool operator==(word w);
		bool operator!=(word w);

	};
	
	std::string charToHex(unsigned char*, int size);
	std::string charToHex(unsigned char value);
	unsigned char* hexToChar(std::string s);
}

#endif
