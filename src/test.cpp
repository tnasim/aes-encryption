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

std::string sample_key		= "2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c";
//std::string sample_input	= "32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34";

//std::string sample_key			= "00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f";
//std::string sample_input		= "00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff";
std::string sample_input		= "19 3d e3 be a0 f4 e2 2b 9a c6 8d 2a e9 f8 48 08";

using namespace util;

bool runAllTests();
bool testSubBytes(std::string input, std::string expected_output);
bool testShiftRows(std::string input, std::string expected_output);
bool testMixColumns(std::string input, std::string expected_output);
bool testKeyExpansion(std::string key);

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

	unsigned char** rcon;
	rcon = new unsigned char*[10];
	for (int i = 0; i < 10; i++) {
		rcon[i] = new unsigned char[4];
		if (i == 0) 
		{
			rcon[i][0] = 0x01;
		} else {
			//XOR rcon * 2 with 0x11b (constant) AND with -(rcon>>7).
			//where rcon>>7 is the first bit of rcon.
			rcon[i][0] = (rcon[i-1][0]<<1) ^ (0x11b & -(rcon[i-1][0]>>7));
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%s ", charToHex(&rcon[j][i], 1).c_str());
		}
		printf("\n");
	}
		
	if(runAllTests()) {
		std::cout << "\033[1;32mALL TESTS PASSED\033[0m\n" << endl;
	} else {
		std::cout << "SOME TEST(S) FAILED" << endl;
	}
	 
	/*
	unsigned char* key = util::hexToChar(sample_key);
	
	std::string key_str = util::charToHex(key, 16);
	std::cout << "Key: " << key_str << std::endl;
	
	std::cout << "Input: " << sample_input << std::endl;
	
	
	// make char (byte-) array from input
	unsigned char *in = util::hexToChar(sample_input);
	
	unsigned char out[16] = {0};
    unsigned char w[16] = {0};
    
    // Test AES class:
    AES *aes = new AES(key, KEY_SIZE);
    aes->Cipher(in, out, w);
	
	std::string out_str = util::charToHex(out, 16);
	
	std::cout << "Output: " << out_str << std::endl;
	*/
	
	
	return 0;
}

/**
 * Tests individual methods using sample inputs.
 * Test sample values are taken from FIPS-197 specification.
 */
bool runAllTests() {
	bool passed = true;
	
	// SubBytes tests:
	// Appendix B: round 1
	passed = passed &&
			testSubBytes(
						 "19 3d e3 be a0 f4 e2 2b 9a c6 8d 2a e9 f8 48 08", //input
						 "d4 27 11 ae e0 bf 98 f1 b8 b4 5d e5 1e 41 52 30"); //expected output
	// Appendix B: round 2
	passed = passed &&
			testSubBytes(
						 "a4 9c 7f f2 68 9f 35 2b 6b 5b ea 43 02 6a 50 49",
						 "49 de d2 89 45 db 96 f1 7f 39 87 1a 77 02 53 3b");
	// Appendix B: round 5
	passed = passed &&
			testSubBytes(
						 "e0 92 7f e8 c8 63 63 c0 d9 b1 35 50 85 b8 be 01",
						 "e1 4f d2 9b e8 fb fb ba 35 c8 96 53 97 6c ae 7c");
	
	
	// ShiftRows tests:
	// Appendix B: round 1
	passed = passed &&
			testShiftRows(
						  "d4 27 11 ae e0 bf 98 f1 b8 b4 5d e5 1e 41 52 30", //input
						  "d4 bf 5d 30 e0 b4 52 ae b8 41 11 f1 1e 27 98 e5"); //expected output
	// Appendix B: round 2
	passed = passed &&
			testShiftRows(
						  "49 de d2 89 45 db 96 f1 7f 39 87 1a 77 02 53 3b",
						  "49 db 87 3b 45 39 53 89 7f 02 d2 f1 77 de 96 1a");
	// Appendix B: round 5
	passed = passed &&
			testShiftRows(
						  "e1 4f d2 9b e8 fb fb ba 35 c8 96 53 97 6c ae 7c",
						  "e1 fb 96 7c e8 c8 ae 9b 35 6c d2 ba 97 4f fb 53");
   passed = passed &&
    		testKeyExpansion(sample_key);	
	
	// MixColumns tests:
	// Appendix B: round 1
	passed = passed &&
			testMixColumns(
						  "d4 bf 5d 30 e0 b4 52 ae b8 41 11 f1 1e 27 98 e5", //input
						  "04 66 81 e5 e0 cb 19 9a 48 f8 d3 7a 28 06 26 4c"); //expected output
	// Appendix B: round 2
	passed = passed &&
			testMixColumns(
						  "49 db 87 3b 45 39 53 89 7f 02 d2 f1 77 de 96 1a",
						  "58 4d ca f1 1b 4b 5a ac db e7 ca a8 1b 6b b0 e5");
	// Appendix B: round 5
	passed = passed &&
			testMixColumns(
						  "e1 fb 96 7c e8 c8 ae 9b 35 6c d2 ba 97 4f fb 53",
						  "25 d1 a9 ad bd 11 d1 68 b6 3a 33 8e 4c 4c c0 b0");

 
	
	return passed;
	
}

bool testSubBytes(std::string input, std::string expected_output) {
	bool passed = false;
	std::cout << "test - SubBytes() -->" << endl;
	std::cout << "\tInput:  " << input << endl;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State(in);
	state->SubBytes();
	unsigned char *out = state->getOutput();
	
	std::string substituted_str = util::charToHex(out, 16);
	std::cout << "\tOutput: " << substituted_str << endl;
	if(!substituted_str.compare(expected_output)) {
		std::cout << "\t\033[1;32m - PASSED\033[0m\n";
		passed = true;
	} else {
		std::cout << "\t\033[1;31m - FAILED\033[0m\n";
		passed = false;
	}
	free(out);
	std::cout << endl;
	return passed;
}

bool testShiftRows(std::string input, std::string expected_output) {
	bool passed = false;
	std::cout << "test - ShiftRows() -->" << endl;
	std::cout << "\tInput:  " << input << endl;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State(in);
	state->ShiftRows();
	unsigned char *out = state->getOutput();
	
	std::string shifted_str = util::charToHex(out, 16);
	std::cout << "\tOutput: " << shifted_str << endl;
	if(!shifted_str.compare(expected_output)) {
		std::cout << "\t\033[1;32m - PASSED\033[0m\n";
		passed = true;
	} else {
		std::cout << "\t\033[1;31m - FAILED\033[0m\n";
		passed = false;
	}
	free(out);
	std::cout << endl;
	return passed;
}

bool testMixColumns(std::string input, std::string expected_output) {
	bool passed = false;
	std::cout << "test - MixColumns() -->" << endl;
	std::cout << "\tInput:  " << input << endl;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State(in);
	state->MixColumns();
	unsigned char *out = state->getOutput();
	
	std::string mixed_str = util::charToHex(out, 16);
	std::cout << "\tOutput: " << mixed_str << endl;
	if(!mixed_str.compare(expected_output)) {
		std::cout << "\t\033[1;32m - PASSED\033[0m\n";
		passed = true;
	} else {
		std::cout << "\t\033[1;31m - FAILED\033[0m\n";
		passed = false;
	}
	free(out);
	std::cout << endl;
	return passed;
}

bool testKeyExpansion(std::string key) {
	bool passed = false;
	std::cout << "Test - Key Expansion() -->" << std::endl;
	std::cout << "\tKey: " << key << std::endl;

	unsigned char* k = hexToChar(key);

	AES *aes = new AES(k, 128);

	free(k);

	return passed;
}
