/*
# This file is used for testing the various functions
# and operations used throughout the project
*/

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "util/util.h"
#include "../includes/aes.h"

const int KEY_SIZE = 128;

//std::string sample_key		= "2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c";
//std::string sample_input	= "32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34";

std::string sample_key			= "00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f";
std::string sample_input		= "00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff";

int main(int argc, char** argv)
{
	//if (argc < 2)
	//{
	//	std::cout << "Need arguments: <text to encrypt>" << std::endl;
	//	exit(1);
	//}

	//char* input = argv[1];
	//int size = strlen(input);
	//char foo[4] = {'f','o','o',0};

	// make char (byte-) array from key
	unsigned char* key = util::hexToChar(sample_key);
	
	std::string key_str = util::charToHex(key, 16);
	std::cout << "Key: " << key_str << std::endl;
	
	std::cout << "Input: " << sample_input << std::endl;
	
	// make char (byte-) array from input
	unsigned char *in = util::hexToChar(sample_input);
	
	unsigned char out[16] = {0};
    unsigned char w[16] = {0};
    
    // Test AES class:
    AES::AES *aes = new AES::AES(key, KEY_SIZE);
    aes->Cipher(in, out, w);
	
	std::string out_str = util::charToHex(out, 16);
	
	std::cout << "Output: " << out_str << std::endl;
	
	
	return 0;
}
