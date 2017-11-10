#include "util.h"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include "../../includes/state.h"

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

std::string util::charToHex(unsigned char value) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	ss << std::setw(2) << static_cast<unsigned>(value);
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

unsigned char util::xTimes(unsigned char x) {
	unsigned char a = (unsigned char) 27;
	if((int)(x<<1) > 255) {
		x = x << 1;
		x = x^a;
	} else {
		x = x << 1;
	}
	return x;
}

void util::word::subWord() 
{
	for (int i = 0; i < 4; i++) 
	{
		unsigned char value = byte[i];
		unsigned char upper_half = (unsigned char)(value>>4);
		unsigned char lower_half = (unsigned char)(value & ((unsigned char)15));

		const char sub_hex[3] = {
			State::sbox[(int)upper_half][(int)lower_half][0],
			State::sbox[(int)upper_half][(int)lower_half][1]
		};
		
		unsigned sub;
		sscanf( sub_hex, "%2x", &sub );
		
		// Update state-entry with substitute value.
		byte[i] = (unsigned char)sub;
	}
	return;
}

void util::word::rotWord()
{
	unsigned char temp = byte[0];
	byte[0] = byte[1];
	byte[1] = byte[2];
	byte[2] = byte[3];
	byte[3] = temp;
}

word util::word::operator^(unsigned char b[]) {
	unsigned char b1 = this->getByte(0) ^ b[0];
	unsigned char b2 = this->getByte(1) ^ b[1];
	unsigned char b3 = this->getByte(2) ^ b[2];
	unsigned char b4 = this->getByte(3) ^ b[3];
	return word(b1,b2,b3,b4);
}

word util::word::operator^(word w) {
	unsigned char b[4];
	for (int i = 0; i < 4; i++)
	{
		b[i] = w.getByte(i) ^ this->getByte(i);
	}
	return word(b[0], b[1], b[2], b[3]);
}

bool util::word::operator==(word w) {
	for (int i = 0; i < 4; i++)
	{
		if(w.getByte(i) != this->getByte(i))
			return false;
	}
	return true;
}

bool util::word::operator!=(word w) {
	for (int i = 0; i < 4; i++)
	{
		if(w.getByte(i) != this->getByte(i))
			return true;
	}
	return false;
}
