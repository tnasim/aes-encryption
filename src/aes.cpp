
#include <iostream>
#include <algorithm>
//#include <stdio.h>
//#include <string.h>
#include "../includes/aes.h"

using namespace std;

AES::AES(unsigned char key[], int keySize) {
	
	initKey(key);
	
	Nb = AES::BLOCK_SIZE/AES::WORD_SIZE;
	
	Nk = keySize/AES::WORD_SIZE;
	
	Nr = Nk + 6; // if Key Length is 4, there will be 10 rounds.
}

/**
 * Perform 'SubBytes' operation on the state.
 */
void AES::SubBytes(State *state) {
	state->SubBytes();
    //printf("SubBytes - not defined yet\n");
}

/**
 * Perform 'ShiftRows' operation on the state.
 */
void AES::ShiftRows(State *state) {
    printf("ShiftRows - not defined yet\n");
}

/**
 * Perform 'MixColumns' operation on the state.
 */
void AES::MixColumns(State *state) {
    printf("MixColumns - not defined yet\n");
}

/**
 * Perform 'AddRoundKey' operation on the state.
 */
void AES::AddRoundKey(State *state) {
    printf("AddRoundKey - not defined yet\n");
}

/**
 * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
 */
void AES::Cipher(unsigned char input[], unsigned char output[], unsigned char w[]) {
	printf("AES properties: Nb = %d, Nk = %d, Nr = %d\n", Nb, Nk, Nr);
	
	// build the 'state' using input:
	State *state = new State::State(input);
	
	
	printf("Initial 'state': \n");
	state->display();
	printf("\n");
	
	
	//TODO: AddRoundKey(state, w[0, Nb-1]) // Sec. 5.1.4
	
	//TODO: perform other operations for each round:
	/*
	 for round = 1 step 1 to Nr–1
		SubBytes(state) // See Sec. 5.1.1
		ShiftRows(state) // See Sec. 5.1.2
		MixColumns(state) // See Sec. 5.1.3
		AddRoundKey(state, w[round*Nb, (round+1)*Nb-1])
	 end for
	 */
	
	//TODO: finalize
	/*
	SubBytes(state)
	ShiftRows(state)
	AddRoundKey(state, w[Nr*Nb, (Nr+1)*Nb-1])
	*/
	
	printf("Final 'state': \n");
	state->display();
	printf("\n");
	
	unsigned char* out = state->getOutput();
	std::copy(out, (out + AES::WORD_SIZE), output);
	
	printf("Cipher -- not fully implemented yet\n");
}
