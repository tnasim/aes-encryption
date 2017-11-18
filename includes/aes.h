#ifndef AES_H
#define AES_H

#include <string.h>
#include <algorithm>
#include <stdio.h>
#include "../includes/state.h"
#include "../src/util/util.h"

#include "../src/util/logger.h"

using namespace std;
using namespace util;

const unsigned int CONSTANT = 0x11b;

class AES {
    
private:
	int Nb;							// Number of words in a block
	int Nk;							// Number of words in the key
	int Nr;							// Number of rounds
	int currentround;
    char key_[32];

	inline void initKey(unsigned char key[]) {
		std::copy(key, key+32, key_);

		/**
		*	Secure Coding, SEI - MEM52-CPP. Detect and handle memory allocation errors.
		*/
		// TODO: need to handle if there is any memory allocation error. (use std::nothrow or handle std::bad_alloc exception.)
		
		//The only thing non-deterministic about this is how many rounds should be used.
		//there are. They rcon is always (hex) 01, 02, 04, 08, 10, 20, 40, 80, 1b, 36.. etc
		//arranged as a array of words, the last 3 bytes of the word are ignored. 
		//example: rcon[0] = {0x01, 0x00, 0x00, 0x00}
//		log(DEBUG) << "Creating Round Constants...";
		//initialize round constant array
		rcon = new struct word[Nr];
		for (int i = 0; i < Nr; i++) 
		{	
			if (i == 0) 
			{
				//the first one is always this.
				rcon[i] = word(0x01, 0x00, 0x00, 0x00);
			} else {
				//XOR rcon * 2 with 0x11b (constant) AND with -(rcon>>7).
				//where rcon>>7 is the first bit of rcon. 
				unsigned char value = rcon[i-1].getByte(0);
				value = (value<<1) ^ (CONSTANT & -(value>>7));
				rcon[i] = word(value, 0x00, 0x00, 0x00);
			}
//			log(DEBUG) << "Constant " << (i+1) << ": " << charToHex(rcon[i].getByte(0));
		}
	}

	/**
	 * Round Constant represented as a 2D array of bytes
	 */ 
	//unsigned char** rcon;
	struct word *rcon;


	/**
	 * Key Schedule. Generated using the key.
	 */
	struct word *w;

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
    void AddRoundKey(State *state, int round);

    /**
	 * Perform 'InvSubBytes' operation on the state.
	 */
	void InvSubBytes(State *state);

	/**
	 * Perform 'InvShiftRows' operation on the state.
	 */
	void InvShiftRows(State *state);

	/**
	 * Perform 'InvMixColumns' operation on the state.
	 */
	void InvMixColumns(State *state);
	
	/**
	 * Expands key 
	 */
	void KeyExpansion();

public:

	static const int WORD_SIZE = 32;
	static const int BLOCK_SIZE = 128;
	
	/**
	 * Constructor - takes the key and initializes
	 **/
	 // TODO: should use 'keySize' as an enum so that only specific values can be passed. Also the constructor should throw some sort of exception if the key is not of correct size.
	AES(unsigned char key[], int keySize);

	~AES();
	
    /**
     * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
     */
	// TODO: should change the method signature --- 'w' is not needed here
    void Cipher(unsigned char plaintext[], unsigned char ciphertext[], unsigned char w[]);

    /**
	 * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
	 */
	// TODO: should change the method signature --- 'w' is not needed here
	void InvCipher(unsigned char ciphertext[], unsigned char plaintext[], unsigned char w[]);
     

};

#endif
