/* Implementation */
#include <stdio.h>
#include <stdlib.h>
#include "../includes/state.h"

/**
 * Takes in the input byte array and initializes the 'state' array with this input.
 */
State::State(unsigned char input[])
{
	int r, c; // c is actually Nb
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			s[r][c] = input[r+4*c];
		}
	}
}

/**
 * retrieves byte at position pos.
 */
unsigned char State::getByte(state_pos pos) {
	return s[pos.row][pos.col];
}

/**
 * Swaps byte at position 'b1'and position 'b2'
 */
void State::swapBytes(state_pos b1, state_pos b2) {
	;
}

/**
 * Build an array and return the pointer to the
 * first element in the array.
 */
unsigned char* State::getWord(int col) {
	unsigned char* word;
	word = (unsigned char*) malloc(sizeof(unsigned char)*16);
	return word;
}

/**
 * Get the final result from the state
 **/
unsigned char* State::getOutput() {
	unsigned char* output;
	int r, c;
	output = (unsigned char*) malloc(sizeof(unsigned char)*16);
	
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			output[r+4*c] = s[r][c];
		}
	}
	
	return output;
}

void State::display() {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			printf("%c ", s[i][j]);
		}
		printf("\n");
	}
}
