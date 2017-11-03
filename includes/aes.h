#ifndef AES_H
#define AES_H

#include <string.h>
#include <algorithm>
#include <stdio.h>
#include "../includes/state.h"
#include "../src/util/util.h"

using namespace std;
using namespace util;

class AES {
    
private:
	int Nb;							// Number of words in a block
	int Nk;							// Number of words in the key
	int Nr;							// Number of rounds
    char key_[32];
	inline void initKey(unsigned char key[]) {
		std::copy(key, key+32, key_);
		//strcpy(key_, key);
	}

	/**
	 * Perform 'SubBytes' operation on the state.
	 */
	void SubBytes(State *state);

    /**
     * Perform 'ShiftRows' operation on the state.
     */
    void ShiftRows(State *state);
    
    /**
     * Perform 'MixColumns' operation on the state.
     */
    void MixColumns(State *state);
    
    /**
     * Perform 'AddRoundKey' operation on the state.
     */
    void AddRoundKey(State *state);
	
	//void KeyExpansion(unsigned char key[4*Nk], word w[Nb*(Nr+1)], Nk)

public:

	static const int WORD_SIZE = 32;
	static const int BLOCK_SIZE = 128;
	
	/**
	 * Constructor - takes the key and initializes
	 **/
	AES(unsigned char key[], int keySize);
	
    /**
     * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
     */
    void Cipher(unsigned char input[], unsigned char output[], unsigned char w[]);
     

};

#endif
