
#include <iostream>
//#include <stdio.h>
//#include <string.h>
#include "../includes/aes.h"

AES::AES(char key[]) {
    initKey(key);
}

/**
 * Perform 'SubBytes' operation on the state.
 */
void AES::SubBytes(State state) {
    printf("SubBytes - not defined yet\n");
}

/**
 * Perform 'ShiftRows' operation on the state.
 */
void AES::ShiftRows(State state) {
    printf("ShiftRows - not defined yet\n");
}

/**
 * Perform 'MixColumns' operation on the state.
 */
void AES::MixColumns(State state) {
    printf("MixColumns - not defined yet\n");
}

/**
 * Perform 'AddRoundKey' operation on the state.
 */
void AES::AddRoundKey(State state) {
    printf("AddRoundKey - not defined yet\n");
}

/**
 * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
 */
void AES::Cipher(char input[], char output[], char w[]) {
    printf("Cipher -- not implemented yet\n");
}
