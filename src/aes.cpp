
#include <iostream>
#include <algorithm>
#include "../includes/aes.h"

using namespace std;

AES::AES(unsigned char key[], int keySize) {
	/**
	 * Secure Coding, SEI - DCL38-C. Use the correct syntax when declaring a flexible array member
	 * Secure Coding, SEI - ARR32-C. Ensure size arguments for variable length arrays are in a valid range
	 * 'key' is kept flexible instead of a fixed sized array because there might be keys of different size.
	 */

	log(DEBUG) << "Starting AES Service...";

	currentround = 0;

	Nb = AES::BLOCK_SIZE/AES::WORD_SIZE;

	Nk = keySize/AES::WORD_SIZE;

	Nr = Nk + 6; // if Key Length is 4, there will be 10 rounds.

	log(DEBUG) << "AES Properties -->" << "\n\tNb: " << Nb << ",\n\tNk: " << Nk << ",\n\tNr: " << Nr;

	log(DEBUG) << "Initializing Key...";

	initKey(key);

	w = new struct word[Nb*(Nr + 1)];

	log(DEBUG) << "Beginning Key Expansion";
	KeyExpansion();
}

AES::~AES() {
	/**
	 * Secure Coding, SEI - MEM51-CPP. "Properly deallocate dynamically allocated resources"
	 */
	free(w);
	// TODO: free(rcon)
}

/**
 * Perform 'SubBytes' operation on the state.
 */
void AES::SubBytes(State *state) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
	state->SubBytes();
}

/**
 * Perform 'ShiftRows' operation on the state.
 */
void AES::ShiftRows(State *state) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
    state->ShiftRows();
}

/**
 * Perform 'MixColumns' operation on the state.
 */
void AES::MixColumns(State *state) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
    state->MixColumns();
}

/**
 * Perform 'AddRoundKey' operation on the state.
 */
void AES::AddRoundKey(State *state, size_t round) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
    state->AddRoundKey(w, round, Nb);
}

/**
 * Perform 'InvSubBytes' operation on the state.
 */
void AES::InvSubBytes(State *state) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
	state->InvSubBytes();
}

/**
 * Perform 'InvShiftRows' operation on the state.
 */
void AES::InvShiftRows(State *state) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
    state->InvShiftRows();
}

/**
 * Perform 'InvMixColumns' operation on the state.
 */
void AES::InvMixColumns(State *state) {
	/**
	 * Secure Coding, SEI - EXP33-C. Do not read uninitialized memory
	 * Should check if 'state' is valid or NULL
	 */
    state->InvMixColumns();
}

void AES::KeyExpansion() {
	//temporary word to hold a value
	struct word temp;

	int i = 0;
	while (i < Nk)
	{
		/**
		 *	Secure Coding, SEI - EXP30-C. Do not depend on the order of evaluation for side effects
		 * used i++ in a separate statement instead of writing 'w[i++]'
		 */
		w[i] = word(key_[4*i],key_[4*i+1],key_[4*i+2], key_[4*i+3]);
		i++;
	}

	i = Nk;

	while (i < Nb * (Nr + 1))
	{
		temp = w[i-1];
		if (i%Nk == 0)
		{
			temp.rotWord();
			temp.subWord();
			//need to use i-1 because index starts at 1 here.
			temp = temp ^ rcon[(i-1)/Nk];
		} else if (Nk > 6 && (i % Nk) == 4) {
			temp.subWord();
		}
		
		/**
		 *	Secure Coding, SEI - EXP30-C. Do not depend on the order of evaluation for side effects
		 * used i++ in a separate statement instead of writing 'w[i++]'
		 */
		
		w[i] = temp ^ w[i-Nk];
		i++;
	}
	return;
}

/**
 * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
 */
void AES::Cipher(unsigned char plaintext[], unsigned char ciphertext[],
		unsigned char w[]) {

	/**
	 *	Secure Coding, SEI - MEM52-CPP. Detect and handle memory allocation errors.
	 */
	// TODO: need to handle if there is any memory allocation error. (use std::nothrow or handle std::bad_alloc exception.)
	// build the 'state' using input:
	State *state = new State(plaintext);
	
	/**
	 *	Secure Coding, SEI - ERR05-C. Application-independent code should provide error detection without dictating error handling
	 */
	// TODO: need some error handling. If state initialization is failed.

	// AddRoundKey(state, w[0, Nb-1); // Sec. 5.1.4
	AddRoundKey(state, 0);

	for (int round = 1; round < Nr; round++) {
		SubBytes(state); // Sec. 5.1.1
		ShiftRows(state); // Sec. 5.1.2
		MixColumns(state); // Sec. 5.1.3
		AddRoundKey(state, round);
	}

	SubBytes(state);
	ShiftRows(state);

	AddRoundKey(state, Nr);

	unsigned char* out = state->getOutput();
	std::copy(out, (out + AES::WORD_SIZE), ciphertext);
	
	/**
	* Secure Coding, SEI - MEM51-CPP. Properly deallocate dynamically allocated resources
	*/
	// TODO: properly deallocate the memory allocated for 'state' object. (use 'delete' since it has been allocated using 'new')
}


/**
 * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
 */
void AES::InvCipher(unsigned char ciphertext[], unsigned char plaintext[],
		unsigned char w[]) {

	/**
	 *	Secure Coding, SEI - MEM52-CPP. Detect and handle memory allocation errors.
	 */
	// TODO: need to handle if there is any memory allocation error. (use std::nothrow or handle std::bad_alloc exception.)
	// build the 'state' using input:
	State *state = new State(ciphertext);

	// AddRoundKey(state, w[Nr*Nb, (Nr+1)(Nb-1)); // Sec. 5.3.4
	AddRoundKey(state, Nr);

	for (int round = Nr - 1; round > 0; round--) {
		InvShiftRows(state); // Sec. 5.4.1
		InvSubBytes(state); // Sec. 5.4.2
		AddRoundKey(state, round); // Sec. 5.3.4
		InvMixColumns(state); // Sec. 5.3.3
	}

	InvShiftRows(state);
	InvSubBytes(state);

	AddRoundKey(state, 0);

	unsigned char* out = state->getOutput();
	std::copy(out, (out + AES::WORD_SIZE), plaintext);
	
	/**
	 * Secure Coding, SEI - MEM51-CPP. Properly deallocate dynamically allocated resources
	 */
	// TODO: properly deallocate the memory allocated for 'state' object. (use 'delete' since it has been allocated using 'new')
}
