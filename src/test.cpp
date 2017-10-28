/*
# This file is used for testing the various functions
# and operations used throughout the project
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "util/util.h"
#include "../includes/aes.h"

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

	std::string output = util::charToHex(input, size);
	std::cout << "Testing: " << output << std::endl;
    
    char key[32] = {0};
    char in[32] = {0};
    char out[32] = {0};
    char w[32] = {0};
    
    // Test AES class:
    AES::AES *aes = new AES::AES(key);
    aes->Cipher(in, out, w);
	return 0;
}
