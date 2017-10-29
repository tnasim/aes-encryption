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
#include "../includes/state.h"
#include "../includes/aes.h"

const int KEY_SIZE = 128;

//std::string sample_key		= "2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c";
//std::string sample_input	= "32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34";

std::string sample_key			= "00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f";
//std::string sample_input		= "00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff";
std::string sample_input		= "19 3d e3 be a0 f4 e2 2b 9a c6 8d 2a e9 f8 48 08";

using namespace util;

void runAllTests();
void testSubBytes(std::string input, std::string expected_output);

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

	
	runAllTests();
	 
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

void runAllTests() {
	
	// SubBytes tests:
	testSubBytes("19 3d e3 be a0 f4 e2 2b 9a c6 8d 2a e9 f8 48 08", "d4 27 11 ae e0 bf 98 f1 b8 b4 5d e5 1e 41 52 30");
	testSubBytes("a4 9c 7f f2 68 9f 35 2b 6b 5b ea 43 02 6a 50 49", "49 de d2 89 45 db 96 f1 7f 39 87 1a 77 02 53 3b");
	testSubBytes("e0 92 7f e8 c8 63 63 c0 d9 b1 35 50 85 b8 be 01", "e1 4f d2 9b e8 fb fb ba 35 c8 96 53 97 6c ae 7c");
	
}

void testSubBytes(std::string input, std::string expected_output) {
	std::cout << "test - SubBytes() -->" << endl;
	std::cout << "\tInput: " << input << endl;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State::State(in);
	state->SubBytes();
	unsigned char *out = state->getOutput();
	
	std::string substituted_str = util::charToHex(out, 16);
	std::cout << "\tOutput: " << substituted_str << endl;
	if(!substituted_str.compare(expected_output)) {
		std::cout << "\t\033[1;32m - PASSED\033[0m\n";
		//std::cout << "\t - PASSED" << endl;
	} else {
		std::cout << "\t\033[1;31m - FAILED\033[0m\n";
		//std::cout << "\t" << endl;
	}
	free(out);
	std::cout << endl;
}
