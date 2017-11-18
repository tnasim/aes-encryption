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
	
	/**
	 *  Secure Coding, SEI - STR31-C. Guarantee that storage for strings has sufficient space for character data and the null terminator
	 */
	unsigned char* char_arr = new unsigned char[s.length()/2+1];
	
	for( unsigned i = 0, uchr ; i < s.length() ; i += 2 ) {
		sscanf( hex_arr+ i, "%2x", &uchr ); // conversion
		char_arr[i/2] = (unsigned char)uchr; // save as char
	}
	char_arr[s.length()/2+1] = '\0';
	
	// TODO: ensure that 'char_arr' is deleted.
	
	/**
	 *	Secure Coding, SEI - DCL30-C. Declare objects with appropriate storage durations
	 */
	// TODO: need to pass 'char_arr' from outside of the method.
	return char_arr;
	
}

unsigned char util::xTimes(unsigned char x) {
	/**
	 *	Secure Coding, SEI - INT34-C. Do not shift an expression by a negative number of bits or by greater than or equal to the number of bits that exist in the operand
	 *  Secure Coding, SEI - INT35-C. Use correct integer precisions
	 */
	// TODO: need to check if INT34-C and INT35-C has been comlied or not.
	unsigned char a = (unsigned char) 27; // "1b"
	if((int)(x<<1) > 255) { // if xTimes exceeds 8 bits, mod with '1b' (hex)
		x = x << 1;
		x = x^a;
	} else {
		x = x << 1;
	}
	return x;
}

/** Multiplication of two bytes in Polynomial form in GF(2^8) modulo (x^8+x^4+x^3+x+1")
 *
 * FIPS-197: Section 4.2
 * */
unsigned char util::polyMultiply(unsigned char a, unsigned char b) {
	unsigned char m = (unsigned char) ( ( ((int)b)%2==1)?a:0);
	unsigned char x = a;
	int n = (int) b;
	while(n>0) {
		n/=2;
		x = xTimes(x);
		if(n%2==1) {
			m = m ^ x;
		}
	}

	return m;
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
		/**
		 *	Secure Coding, SEI - ERR33-C. Detect and handle standard library errors (for sscanf, it might return EOF/negative)
		 */
		// TODO: need to check if sscanf was successful or not.
		sscanf( sub_hex, "%2x", &sub );
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
