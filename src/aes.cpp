
#include <iostream>
#include <algorithm>
//#include <stdio.h>
//#include <string.h>
#include "../includes/aes.h"

using namespace std;

AES::AES(unsigned char key[], int keySize) {
	cout << "Starting AES Service..." << endl;

	currentround = 0;

	Nb = AES::BLOCK_SIZE/AES::WORD_SIZE;
	
	Nk = keySize/AES::WORD_SIZE;
	
	Nr = Nk + 6; // if Key Length is 4, there will be 10 rounds.

	cout << "Nb: " << Nb << ", Nk: " << Nk << ", Nr: " << Nr << endl;

	cout << "Initializing Key..." << endl;

	initKey(key);

	w = new struct word[Nb*(Nr + 1)];

	cout << "Beginning Key Expansion" << endl;
	KeyExpansion();
}

AES::~AES() {
	free(w);
}

/**
 * Perform 'SubBytes' operation on the state.
 */
void AES::SubBytes(State *state) {
	state->SubBytes();
}

/**
 * Perform 'ShiftRows' operation on the state.
 */
void AES::ShiftRows(State *state) {
    state->ShiftRows();
}

/**
 * Perform 'MixColumns' operation on the state.
 */
void AES::MixColumns(State *state) {
    state->MixColumns();
}

/**
 * Perform 'AddRoundKey' operation on the state.
 */
void AES::AddRoundKey(State *state, int round) {
    state->AddRoundKey(w, round, Nb);
}

void AES::KeyExpansion() {
//	printf("Key Expansion - Testing\n");
	//temporary word to hold a value
	struct word temp;

	int i = 0;
	while (i < Nk)
	{
		w[i] = word(key_[4*i],key_[4*i+1],key_[4*i+2], key_[4*i+3]);
		i++;
	}


	i = Nk;

	while (i < Nb * (Nr + 1)) 
	{
		temp = w[i-1];
//		std::cout << "temp: " << temp.hex() << std::endl;
		if (i%Nk == 0)
		{
			temp.rotWord();
//			std::cout << "After RotWord(): " << temp.hex() << std::endl;
			temp.subWord();
//			std::cout << "After SubWord(): " << temp.hex() << std::endl;
			//need to use i-1 because index starts at 1 here.
			temp = temp ^ rcon[(i-1)/Nk];
//			std::cout << "Rcon[i/Nk]: " << rcon[(i-1)/Nk].hex() << std::endl;
//			std::cout << "XOR with Rcon: " << temp.hex() << std::endl;
			//temp = SubWord(RotWord(temp))^Rcon[i/Nk];
		} else if (Nk > 6 && (i % Nk) == 4) {
			temp.subWord();
		}
//		std::cout << "w[i-Nk]: " << w[i-Nk].hex() << std::endl;
		w[i] = temp ^ w[i-Nk];
//		std::cout << "w[i]: " << w[i].hex() << std::endl;
		i++;
	}
	return;
}

/**
 * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
 */
void AES::Cipher(unsigned char input[], unsigned char output[], unsigned char w[]) {
//	printf("AES properties: Nb = %d, Nk = %d, Nr = %d\n", Nb, Nk, Nr);
	
	// build the 'state' using input:
	State *state = new State(input);
	
	
	/*printf("Initial 'state': \n");
	state->display();
	printf("\n");*/
	
	
	//TODO: add the parameter to indicate the range of w to be used in the 'AddRoundKey' operation
//	AddRoundKey(state, w[0, Nb-1); // Sec. 5.1.4
	AddRoundKey(state, 0);
	std::cout << "After AddRoundKey, 0" << std::endl;
	state->display();

	 for (int round = 1; round < Nr; round++) {
		SubBytes(state); // See Sec. 5.1.1
		ShiftRows(state); // See Sec. 5.1.2
		MixColumns(state); // See Sec. 5.1.3
		AddRoundKey(state, round);
	 }

	SubBytes(state);
	ShiftRows(state);

	AddRoundKey(state, Nr);
	
	/*printf("Final 'state': \n");
	state->display();
	printf("\n");*/
	
	unsigned char* out = state->getOutput();
	std::copy(out, (out + AES::WORD_SIZE), output);
}
