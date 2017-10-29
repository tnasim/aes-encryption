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

//using namespace std;

std::string input_str = "This is an input";
//std::string key_str = "This is the key.";

std::string sample_key = "2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c";

int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		std::cout << "Need arguments: <text to encrypt>" << std::endl;
		exit(1);
	}

	char* input = argv[1];
	int size = strlen(input);
	//char foo[4] = {'f','o','o',0};

	unsigned char* hex_char_array = util::hexToChar(sample_key);
	
	std::string key_str = util::charToHex(hex_char_array, 16);
	
	std::string::iterator end_pos = std::remove(key_str.begin(), key_str.end(), ' ');
	key_str.erase(end_pos, key_str.end());
	
	std::cout << "Hex key: " << key_str << std::endl;
	
    unsigned char *key = new unsigned char[key_str.size()+1];
	std::copy(key_str.begin(), key_str.end(), key);
	key[key_str.size()] = '\0';
	
	unsigned char *in = new unsigned char[input_str.size()+1];
    std::copy(input_str.begin(), input_str.end(), in);
	in[input_str.size()] = '\0';
	
	unsigned char out[16] = {0};
    unsigned char w[16] = {0};
    
    // Test AES class:
    AES::AES *aes = new AES::AES(key, KEY_SIZE);
    aes->Cipher(in, out, w);
	printf("%s", out);
	
	
	return 0;
}
