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
#include "util/keyGenerator.h"

#include <unistd.h>
#include <fcntl.h>

/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>

using namespace util;
using namespace keyGenerator;
using namespace keyExpansionTest;

/** Set the log level */
logLevel LOG_LEVEL = TEST;
//logLevel LOG_LEVEL = TEST_PASS;
//logLevel LOG_LEVEL = ERROR;
//logLevel LOG_LEVEL = DEBUG;
//logLevel LOG_LEVEL = INFO;
//logLevel LOG_LEVEL = WARN;

/** the transformation types **/
enum trans_type { SUB_BYTES, SHIFT_ROWS, MIX_COLUMNS, ADD_ROUND_KEY};
std::string trans_name[] = { "SubBytes", "ShiftRows", "MixColumns", "AddRoundKey"};

struct test_set {
	std::string plain;
	std::string cipher;
	std::string key;
	KEY_SIZE key_size;
	test_set(std::string p, std::string c, std::string k, KEY_SIZE keySize) {
		plain = p;
		cipher = c;
		key = k;
		key_size = keySize;
	}
};

bool runAllTests();
bool testSubBytes(std::string input, std::string expected_output, bool inverse=false);
bool testShiftRows(std::string input, std::string expected_output, bool inverse=false);
bool testMixColumns(std::string input, std::string expected_output, bool inverse=false);
bool testAddRoundKey(std::string input, std::string expected_output);
bool testXTimes(std::string input, std::string expected_output);
bool testPolyMultiply(std::string x, std::string y, std::string expected_output);

bool testTransformation(trans_type transformation, std::string input, std::string expected_output, bool inverse=false);

bool testKeyExpansion(std::string key, std::string expected[]);
bool testCipher(std::string data, std::string k, KEY_SIZE key_size, std::string result, bool inverse=false);
bool testCipherDecipher_Random(KEY_SIZE key_size);
test_set getRandomTestData(KEY_SIZE key_size);

int main(int argc, char** argv)
{
	//if (argc < 2)
	//{
	//	log(TEST) << "Need arguments: <text to encrypt>";
	//	exit(1);
	//}

	//char* input = argv[1];
	//int size = strlen(input);
	//char foo[4] = {'f','o','o',0};

	// TODO: Generate Random Key.
	
	/**
	 * Use a random key generator to generator the key. Follow the secure SEI CERT C++ Coding guidelines at: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=88046554
	 */
	
	/*test_set test_data = getRandomTestData(KEY_SIZE_128);
	cout << "plain: " << test_data.plain << endl << "key: " << test_data.key << endl;*/
	
	if(runAllTests()) {
		log(TEST_PASS) << "ALL TESTS PASSED\n";
	} else {
		log(TEST_FAIL) << "SOME TEST(S) FAILED";
		return 0;
	}

	return 0;
}

const int test_128_total_samples = 2;
test_set test_128[] =
	{
		test_set(
				"32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34", // plain
				"39 25 84 1d 02 dc 09 fb dc 11 85 97 19 6a 0b 32", // cipher
				"2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c",
				KEY_SIZE_128), // key
		test_set(
				"00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff", // plain
				"69 c4 e0 d8 6a 7b 04 30 d8 cd b7 80 70 b4 c5 5a", // cipher
				"00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f",
				KEY_SIZE_128) // key
	};

std::string sample_key_128_1 			= "2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c";
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

const int mix_columns_total_samples = 3;
const std::string mix_columns_sample[][2] =
		{
			{ // Appendix B: round 1
				"d4 bf 5d 30 e0 b4 52 ae b8 41 11 f1 1e 27 98 e5",
				"04 66 81 e5 e0 cb 19 9a 48 f8 d3 7a 28 06 26 4c"
			},
			{ // Appendix B: round 2
				"49 db 87 3b 45 39 53 89 7f 02 d2 f1 77 de 96 1a",
				"58 4d ca f1 1b 4b 5a ac db e7 ca a8 1b 6b b0 e5"
			},
			{ // Appendix B: round 5
				"e1 fb 96 7c e8 c8 ae 9b 35 6c d2 ba 97 4f fb 53",
				"25 d1 a9 ad bd 11 d1 68 b6 3a 33 8e 4c 4c c0 b0"
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
	

	passed = passed && testXTimes("57", "ae");
	passed = passed && testXTimes("ae", "47");
	passed = passed && testXTimes("47", "8e");
	passed = passed && testXTimes("8e", "07");

	passed = passed && testPolyMultiply("57", "13", "fe");
	passed = passed && testPolyMultiply("57", "83", "c1");
	passed = passed && testPolyMultiply("bf", "03", "da");
	passed = passed && testPolyMultiply("bf", "02", "65");

	// MixColumns tests:
	for(i = 0; i < mix_columns_total_samples && passed; i++) {
		passed = passed && testMixColumns(mix_columns_sample[i][0], mix_columns_sample[i][1]);
	}

	// InvSubBytes tests:
	for(i = 0; i < sub_bytes_total_samples && passed; i++) {
		passed = passed && testSubBytes(sub_bytes_sample[i][1], sub_bytes_sample[i][0], true);
	}

	// InvShiftRows tests:
	for(i = 0; i < shift_rows_total_samples && passed; i++) {
		passed = passed && testShiftRows(shift_rows_sample[i][1], shift_rows_sample[i][0], true);
	}

	// InvMixColumns tests:
	for(i = 0; i < mix_columns_total_samples && passed; i++) {
		passed = passed && testMixColumns(mix_columns_sample[i][1], mix_columns_sample[i][0], true);
	}

	// Cipher tests:
	for(i = 0; i < test_128_total_samples && passed; i++) {
		passed = passed && testCipher(test_128[i].plain, test_128[i].key, test_128[i].key_size, test_128[i].cipher);
	}

	// InvCipher tests:
	for(i = 0; i < test_128_total_samples && passed; i++) {
		passed = passed && testCipher(test_128[i].cipher, test_128[i].key, test_128[i].key_size, test_128[i].plain, true);
	}

	passed = passed && testCipherDecipher_Random(KEY_SIZE_128);
	return passed;
	
}

bool testCipher(std::string data, std::string k, KEY_SIZE key_size, std::string result, bool inverse) {
	bool passed = false;
	log(TEST) << "test - " << (inverse?"Inv":"") << "Cipher() -->";

	unsigned char* key = util::hexToChar(k);

	std::string key_str = util::charToHex(key, 16);
	log(TEST) << "\tKey:\t" << key_str;

	log(TEST) << "\tData:\t" << data;


	// make char (byte-) array from input
	unsigned char *in = util::hexToChar(data);

	unsigned char out[16] = {0};
	unsigned char w[16] = {0};

	// Test AES class:
	AES *aes = new AES(key, key_size);
	if(!inverse)
		aes->Cipher(in, out, w);
	else
		aes->InvCipher(in, out, w);

	std::string result_hex = util::charToHex(out, 16);

	if(!result_hex.compare(result)) {
		log(TEST) << "\t - Output:\t" << result_hex;
		log(TEST) << "\t - Expected:\t" << result;
		log(TEST_PASS) << "\t - test " << (inverse?"Inv":"") << "Cipher() PASSED !!";
		passed = true;
	} else {
		log(TEST) << "\t - Output:\t" << result_hex;
		log(TEST) << "\t - Expected:\t" << result;
		log(TEST_FAIL) << "\t - test " << (inverse?"Inv":"") << "Cipher() FAILED";
		passed = false;
	}

	return passed;
}

bool testCipherDecipher_Random(KEY_SIZE key_size) {
	bool passed = false;
	log(TEST) << "test - testCipherDecipher_Random() -->";

	test_set test_data = getRandomTestData(key_size);
	unsigned char* key = util::hexToChar(test_data.key);

	std::string key_str = util::charToHex(key, 16);
	log(TEST) << "\tKey:\t" << key_str;

	log(TEST) << "\tData:\t" << test_data.plain;


	// make char (byte-) array from input
	unsigned char *in = util::hexToChar(test_data.plain);

	unsigned char out[16] = {0};
	unsigned char in2[16] = {0};
	unsigned char w[16] = {0};

	// Test AES class:
	AES *aes = new AES(key, key_size);

	aes->Cipher(in, out, w);

	aes->InvCipher(out, in2, w);

	std::string result_hex = util::charToHex(in2, 16);

	if(!result_hex.compare(test_data.plain)) {
		log(TEST) << "\t - Output:\t" << result_hex;
		log(TEST) << "\t - Expected:\t" << test_data.plain;
		log(TEST_PASS) << "\t - testCipherDecipher_Random()  PASSED !!";
		passed = true;
	} else {
		log(TEST) << "\t - Output:\t" << result_hex;
		log(TEST) << "\t - Expected:\t" << test_data.plain;
		log(TEST_FAIL) << "\t - testCipherDecipher_Random() FAILED";
		passed = false;
	}

	return passed;
}

bool testSubBytes(std::string input, std::string expected_output, bool inverse) {
	return testTransformation(SUB_BYTES, input, expected_output, inverse);
}

bool testShiftRows(std::string input, std::string expected_output, bool inverse) {
	return testTransformation(SHIFT_ROWS, input, expected_output, inverse);
}

bool testMixColumns(std::string input, std::string expected_output, bool inverse) {
	return testTransformation(MIX_COLUMNS, input, expected_output, inverse);
}

bool testAddRoundKey(std::string input, std::string expected_output) {
	bool passed = false;
	log(TEST) << "Test - AddRoundKey() --";
	log(TEST) << "\tInput:  " << input;
	unsigned char* key = util::hexToChar(sample_key_128_1);
	unsigned char* in = util::hexToChar(input);
	State *state = new State(in);
	State *sKey = new State(key);
	struct word* w = new word[4];
	int q = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			w[i].setByte(key[q], j);
			q++;
		}
	}


	state->AddRoundKey(w, 4, 0);

	unsigned char* out = state->getOutput();
	std::string addKey_str = util::charToHex(out, 16);
	log(TEST) << "\tOutput: " << addKey_str;
	if(!addKey_str.compare(expected_output)) {
		log(TEST_PASS) << "\t - PASSED";
		passed = true;
	} else {
		log(TEST_FAIL) << "\t - FAILED";
		passed = false;
	}
	
	// TODO: delete 'state', 'sKey' and 'w'.
	
//	delete(state);
//	delete(word);
//	free(out);

	return passed;
}

bool testKeyExpansion(std::string key, std::string expected[]) {
	bool passed = false;
	log(TEST) << "test - KeyExpansion() -->";
	log(TEST) << "\tInput key:  " << key;

	if( keyExpansionTest::test(key, expected, KEY_SIZE_128) ) {
		log(TEST_PASS) << "\t - test KeyExpansion() PASSED\n";
		passed = true;
	} else {
		log(TEST_FAIL) << "\t - test KeyExpansion() FAILED\n";
	}

	return passed;
}

bool testTransformation(trans_type transformation, std::string input, std::string expected_output, bool inverse) {
	bool passed = false;
	log(TEST) << "test - "<<(inverse?"Inv":"") << trans_name[transformation] <<"() -->";
	log(TEST) << "\tInput:  " << input;

	unsigned char *in = util::hexToChar(input);

	State *state = new State(in);
	if(!inverse) {
		if(transformation == SUB_BYTES)
			state->SubBytes();
		else if(transformation == SHIFT_ROWS)
			state->ShiftRows();
		else if (transformation == MIX_COLUMNS)
			state->MixColumns();
	} else {
		if(transformation == SUB_BYTES)
			state->InvSubBytes();
		else if(transformation == SHIFT_ROWS)
			state->InvShiftRows();
		else if (transformation == MIX_COLUMNS)
			state->InvMixColumns();
	}
	unsigned char *out = state->getOutput();

	std::string substituted_str = util::charToHex(out, 16);
	log(TEST) << "\tOutput: " << substituted_str;
	if(!substituted_str.compare(expected_output)) {
		log(TEST_PASS) << "\t - test "<<(inverse?"Inv":"") << trans_name[transformation] <<"() PASSED";
		passed = true;
	} else {
		log(TEST) << "\t - Output:\t" << substituted_str;
		log(TEST) << "\t - Expected:\t" << expected_output;
		log(TEST_FAIL) << "\t - test "<<(inverse?"Inv":"") << trans_name[transformation] <<"() FAILED";
		passed = false;
	}
	free(out);
	// TODO: delete 'state' object ... and also the 'in'/'out' pointers may need to be deleted/freed.

	return passed;
}

bool testPolyMultiply(std::string x, std::string y, std::string expected_output) {
	bool passed = false;
	log(TEST) << "test - polyMultiply() -->";
	unsigned char* a = util::hexToChar(x);
	unsigned char* b = util::hexToChar(y);
	unsigned char m = util::polyMultiply(a[0], b[0]);
	std::string m_str = util::charToHex(m);

	if( !m_str.compare(expected_output) ) {
		log(TEST_PASS) << "\t - test polyMulti({" << x << "}, {" << y << "}) PASSED";
		passed = true;
	} else {
		log(TEST) << "\t - Output:\t" << m_str;
		log(TEST) << "\t - Expected:\t" << expected_output;
		log(TEST_FAIL) << "\t - test polyMulti({" << x << "}, {" << y << "}) FAILED";
	}
	
	// TODO: delete 'a' and 'b'

	return passed;
}

bool testXTimes(std::string input, std::string expected_output) {
	bool passed = false;
	log(TEST) << "test - xTimes() -->";
	unsigned char* a = util::hexToChar(input);
	unsigned char xt = util::xTimes(a[0]);
	std::string xt_str = util::charToHex(xt);

	if( !xt_str.compare(expected_output) ) {
		log(TEST_PASS) << "\t - test xTimes({" << input << "}) PASSED";
		passed = true;
	} else {
		log(TEST_FAIL) << "\t - test xTimes({" << input << "}) FAILED";
	}

	return passed;
}

test_set getRandomTestData(KEY_SIZE key_size) {
	size_t key_len = key_size/8;
	size_t data_len = key_size/8;
	unsigned char key[(size_t)key_len];
	unsigned char data[(size_t)data_len];

	keyGenerator::generateRandomKey(key_len, key);
	std::string key_hex = util::charToHex(key, key_len);

	keyGenerator::generateRandomKey(data_len, data);
	std::string data_hex = util::charToHex(data, data_len);

	test_set test_data = test_set(key_hex, "", data_hex, key_size);
	return test_data;
}
