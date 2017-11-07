/*
# This file is used for testing the various functions
# and operations used throughout the project
*/

#include "keyExpansionTest.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "../src/util/util.h"
#include "../includes/state.h"

// this was necessary to access private properties of AES class.
// I could not find better way to do this.
#define private public

#include "../includes/aes.h"

const int KEY_SIZE = 128;

using namespace util;

bool keyExpansionTest::test(std::string key_hex, std::string expected_key_expansion[], int key_size) {
	bool passed = true;

	unsigned char* key = hexToChar(key_hex);

	AES *aes = new AES(key, key_size);
//	aes->KeyExpansion();
	word *w = aes->w;
	for(int i = 0; i < aes->Nb*(aes->Nr+1); i++) {
//		string word_hex = wordToHex(w[i]);
		word w_expected = word(expected_key_expansion[i]);
		if(w[i] != w_expected) {
			passed = false;
			printf("\tKey Expansion FAILED at w[%d]\n\t\t- generated:\t\033[1;31m%s\033[0m\n\t\t- expected:\t%s\n", i, w[i].hex().c_str(), w_expected.hex().c_str());
			break;
		}

		// remove whitespace from the word value
		/*std::string::iterator end_pos = std::remove(word_hex.begin(), word_hex.end(), ' ');
		word_hex.erase(end_pos, word_hex.end());

		if(word_hex.compare(expected_key_expansion[i]) != 0) {
			passed = false;
			printf("\tKey Expansion FAILED at w[%d]\n\t\t- generated:\t\033[1;31m%s\033[0m\n\t\t- expected:\t%s\n", i, word_hex.c_str(), expected_key_expansion[i].c_str());
			break;
		}*/
	}

	delete(aes);
	free(key);

	return passed;
}
