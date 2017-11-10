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
#include "util/logger.h"
#include "../includes/state.h"
#include "../includes/aes.h"
#include "../test/keyExpansionTest.h"


/** Set the log level */
logLevel LOG_LEVEL = TEST_PASS;
//logLevel LOG_LEVEL = ERROR;
//logLevel LOG_LEVEL = DEBUG;
//logLevel LOG_LEVEL = INFO;
//logLevel LOG_LEVEL = WARN;


const int KEY_SIZE = 128;

std::string sample_key_128_2			= "00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f";
std::string sample_input_128_2			= "00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff";
std::string sample_cipher_128_2			= "69 c4 e0 d8 6a 7b 04 30 d8 cd b7 80 70 b4 c5 5a";

std::string sample_key_128_1 			= "2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c";
std::string sample_input_128_1			= "32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34";
std::string sample_cipher_128_1			= "39 25 84 1d 02 dc 09 fb dc 11 85 97 19 6a 0b 32";

std::string sample_key_expanded_128_1[] =
				{
						"2b7e1516", //w[ 0]
						"28aed2a6", //w[ 1]
						"abf71588", //w[ 2]
						"09cf4f3c", //w[ 3]
						"a0fafe17", //w[ 4]
						"88542cb1", //w[ 5]
						"23a33939", //w[ 6]
						"2a6c7605", //w[ 7]
						"f2c295f2", //w[ 8]
						"7a96b943", //w[ 9]
						"5935807a", //w[10]
						"7359f67f", //w[11]
						"3d80477d", //w[12]
						"4716fe3e", //w[13]
						"1e237e44", //w[14]
						"6d7a883b", //w[15]
						"ef44a541", //w[16]
						"a8525b7f", //w[17]
						"b671253b", //w[18]
						"db0bad00", //w[19]
						"d4d1c6f8", //w[20]
						"7c839d87", //w[21]
						"caf2b8bc", //w[22]
						"11f915bc", //w[23]
						"6d88a37a", //w[24]
						"110b3efd", //w[25]
						"dbf98641", //w[26]
						"ca0093fd", //w[27]
						"4e54f70e", //w[28]
						"5f5fc9f3", //w[29]
						"84a64fb2", //w[30]
						"4ea6dc4f", //w[31]
						"ead27321", //w[32]
						"b58dbad2", //w[33]
						"312bf560", //w[34]
						"7f8d292f", //w[35]
						"ac7766f3", //w[36]
						"19fadc21", //w[37]
						"28d12941", //w[38]
						"575c006e", //w[39]
						"d014f9a8", //w[40]
						"c9ee2589", //w[41]
						"e13f0cc8", //w[42]
						"b6630ca6", //w[43]
				};

using namespace util;
using namespace keyExpansionTest;

bool runAllTests();
bool testSubBytes(std::string input, std::string expected_output, bool inverse=false);
bool testShiftRows(std::string input, std::string expected_output, bool inverse=false);
bool testMixColumns(std::string input, std::string expected_output, bool inverse=false);
bool testAddRoundKey(std::string input, std::string expected_output);
bool testKeyExpansion(std::string key, std::string expected[]);
bool testCipher(std::string input_data, std::string k, std::string expected_cipher);

int main(int argc, char** argv)
{
	//if (argc < 2)
	//{
	//	log(DEBUG) << "Need arguments: <text to encrypt>";
	//	exit(1);
	//}

	//char* input = argv[1];
	//int size = strlen(input);
	//char foo[4] = {'f','o','o',0};

	if(runAllTests()) {
		log(TEST_PASS) << "ALL TESTS PASSED\n";
	} else {
		log(TEST_FAIL) << "SOME TEST(S) FAILED";
	}

	testCipher(sample_input_128_1, sample_key_128_1, sample_cipher_128_1);
	testCipher(sample_input_128_1, sample_key_128_1, sample_cipher_128_1);
	
	return 0;
}

const int sub_bytes_total_samples = 3;
const std::string sub_bytes_sample[][2] =
		{
			{ // Appendix B: round 1
				"19 3d e3 be a0 f4 e2 2b 9a c6 8d 2a e9 f8 48 08",
				"d4 27 11 ae e0 bf 98 f1 b8 b4 5d e5 1e 41 52 30"
			},
			{ // Appendix B: round 2
				"a4 9c 7f f2 68 9f 35 2b 6b 5b ea 43 02 6a 50 49",
				"49 de d2 89 45 db 96 f1 7f 39 87 1a 77 02 53 3b"
			},
			{ // Appendix B: round 5
				"e0 92 7f e8 c8 63 63 c0 d9 b1 35 50 85 b8 be 01",
				"e1 4f d2 9b e8 fb fb ba 35 c8 96 53 97 6c ae 7c"
			}
		};

const int shift_rows_total_samples = 3;
const std::string shift_rows_sample[][2] =
		{
			{ // Appendix B: round 1
				"d4 27 11 ae e0 bf 98 f1 b8 b4 5d e5 1e 41 52 30",
				"d4 bf 5d 30 e0 b4 52 ae b8 41 11 f1 1e 27 98 e5"
			},
			{ // Appendix B: round 2
				"49 de d2 89 45 db 96 f1 7f 39 87 1a 77 02 53 3b",
				"49 db 87 3b 45 39 53 89 7f 02 d2 f1 77 de 96 1a"
			},
			{ // Appendix B: round 5
				"e1 4f d2 9b e8 fb fb ba 35 c8 96 53 97 6c ae 7c",
				"e1 fb 96 7c e8 c8 ae 9b 35 6c d2 ba 97 4f fb 53"
			}
		};
/**
 * Tests individual methods using sample inputs.
 * Test sample values are taken from FIPS-197 specification.
 */
bool runAllTests() {
	bool passed = true;
	int i;
	// SubBytes tests:
	for(i = 0; i < sub_bytes_total_samples && passed; i++) {
		passed = passed && testSubBytes(sub_bytes_sample[i][0], sub_bytes_sample[i][1]);
	}
	
	// ShiftRows tests:
	for(i = 0; i < shift_rows_total_samples && passed; i++) {
		passed = passed && testShiftRows(shift_rows_sample[i][0], shift_rows_sample[i][1]);
	}


	// AddRoundKey tests:
    passed = passed &&
    		testAddRoundKey(
    				"32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34", 
    				"19 3d e3 be a0 f4 e2 2b 9a c6 8d 2a e9 f8 48 08");

	// Key Expansion Test:
	// A.1 Expansion of a 128-bit Cipher Key
	passed = passed &&
    		testKeyExpansion(sample_key_128_1, sample_key_expanded_128_1);
	

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


	// InvSubBytes tests:
	for(i = 0; i < sub_bytes_total_samples && passed; i++) {
		passed = passed && testSubBytes(sub_bytes_sample[i][1], sub_bytes_sample[i][0], true);
	}

	// InvShiftRows tests:
	for(i = 0; i < shift_rows_total_samples && passed; i++) {
		passed = passed && testShiftRows(shift_rows_sample[i][1], shift_rows_sample[i][0], true);
	}
	
	return passed;
	
}

bool testCipher(std::string input_data, std::string k, std::string expected_cipher) {
	bool passed = false;
	log(DEBUG) << "test - Cipher() -->";

	unsigned char* key = util::hexToChar(k);

	std::string key_str = util::charToHex(key, 16);
	log(DEBUG) << "\tKey:\t" << key_str;

	log(DEBUG) << "\tData:\t" << input_data;


	// make char (byte-) array from input
	unsigned char *in = util::hexToChar(input_data);

	unsigned char out[16] = {0};
	unsigned char w[16] = {0};

	// Test AES class:
	AES *aes = new AES(key, KEY_SIZE);
	aes->Cipher(in, out, w);

	std::string ciphex = util::charToHex(out, 16);

	if(!ciphex.compare(expected_cipher)) {
		log(TEST_PASS) << "\t - test Cipher() PASSED !!\n";
		passed = true;
	} else {
		log(DEBUG) << "\t - Output Cipher:\t" << ciphex;
		log(DEBUG) << "\t - Expected Cipher:\t" << expected_cipher;
		log(TEST_FAIL) << "\t - test Cipher() FAILED\n";
		passed = false;
	}

	return passed;
}

bool testSubBytes(std::string input, std::string expected_output, bool inverse) {
	bool passed = false;
	log(DEBUG) << "test - "<<(inverse?"Inv":"")<<"SubBytes() -->";
	log(DEBUG) << "\tInput:  " << input;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State(in);
	if(!inverse)
		state->SubBytes();
	else
		state->InvSubBytes();
	unsigned char *out = state->getOutput();
	
	std::string substituted_str = util::charToHex(out, 16);
	log(DEBUG) << "\tOutput: " << substituted_str;
	if(!substituted_str.compare(expected_output)) {
		log(TEST_PASS) << "\t - test "<<(inverse?"Inv":"")<<"SubBytes() PASSED\n";
		passed = true;
	} else {
		log(DEBUG) << "\t - Output:\t" << substituted_str;
		log(DEBUG) << "\t - Expected:\t" << expected_output;
		log(TEST_FAIL) << "\t - test "<<(inverse?"Inv":"")<<"SubBytes() FAILED\n";
		passed = false;
	}
	free(out);

	return passed;
}

bool testShiftRows(std::string input, std::string expected_output, bool inverse) {
	bool passed = false;
	log(DEBUG) << "test - "<<(inverse?"Inv":"")<<"ShiftRows() -->";
	log(DEBUG) << "\tInput:  " << input;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State(in);
	if(!inverse)
		state->ShiftRows();
	else
		state->InvShiftRows();
	unsigned char *out = state->getOutput();
	
	std::string shifted_str = util::charToHex(out, 16);
	log(DEBUG) << "\tOutput: " << shifted_str;
	if(!shifted_str.compare(expected_output)) {
		log(TEST_PASS) << "\t - test "<<(inverse?"Inv":"")<<"ShiftRows() PASSED\n";
		passed = true;
	} else {
		log(DEBUG) << "\t - Output:\t" << shifted_str;
		log(DEBUG) << "\t - Expected:\t" << expected_output;
		log(TEST_FAIL) << "\t - test "<<(inverse?"Inv":"")<<"ShiftRows() FAILED\n";
		passed = false;
	}
	free(out);

	return passed;
}

bool testMixColumns(std::string input, std::string expected_output, bool inverse) {
	bool passed = false;
	log(DEBUG) << "test - "<<(inverse?"Inv":"")<<"MixColumns() -->";
	log(DEBUG) << "\tInput:  " << input;
	
	unsigned char *in = util::hexToChar(input);
	
	State *state = new State(in);
	if(!inverse)
		state->MixColumns();
	else
		state->InvMixColumns();
	unsigned char *out = state->getOutput();
	
	std::string mixed_str = util::charToHex(out, 16);
	log(DEBUG) << "\tOutput: " << mixed_str;
	if(!mixed_str.compare(expected_output)) {
		log(TEST_PASS) << "\t - test "<<(inverse?"Inv":"")<<"MixColumns() PASSED\n";
		passed = true;
	} else {
		log(DEBUG) << "\t - Output:\t" << mixed_str;
		log(DEBUG) << "\t - Expected:\t" << expected_output;
		log(TEST_FAIL) << "\t - test "<<(inverse?"Inv":"")<<"MixColumns() FAILED\n";
		passed = false;
	}
	free(out);

	return passed;
}

bool testAddRoundKey(std::string input, std::string expected_output) {
	bool passed = false;
	log(DEBUG) << "Test - AddRoundKey() --";
	log(DEBUG) << "\tInput:  " << input;
	unsigned char* key = util::hexToChar(sample_key_128_1);
	unsigned char* in = util::hexToChar(input);
	State *state = new State(in);
	State *sKey = new State(key);
	struct word* w = new word[4];
	int q = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			w[i].setByte(key[q], j);
			q++;
		}

	log(DEBUG) << "Input: ";
	state->display();
	log(DEBUG) <<  "XOR: ";
	sKey->display();

	state->AddRoundKey(w, 4, 0);

	unsigned char* out = state->getOutput();
	std::string addKey_str = util::charToHex(out, 16);
	log(DEBUG) << "\tOutput: " << addKey_str;
	if(!addKey_str.compare(expected_output)) {
		log(TEST_PASS) << "\t - PASSED\n";
		passed = true;
	} else {
		log(TEST_FAIL) << "\t - FAILED\n";
		passed = false;
	}
//	delete(state);
//	delete(word);
//	free(out);

	return passed;
}

bool testKeyExpansion(std::string key, std::string expected[]) {
	bool passed = false;
	log(DEBUG) << "test - KeyExpansion() -->";
	log(DEBUG) << "\tInput key:  " << key;

	if( keyExpansionTest::test(key, expected, KEY_SIZE) ) {
		log(TEST_PASS) << "\t - test KeyExpansion() PASSED\n";
		passed = true;
	} else {
		log(TEST_FAIL) << "\t - test KeyExpansion() FAILED\n";
	}

	return passed;
}
