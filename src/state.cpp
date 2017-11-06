
#include <stdio.h>
#include <stdlib.h>
#include "../includes/state.h"


const char State::sbox[16][16][3] =
{	{"63", "7c", "77", "7b", "f2", "6b", "6f", "c5", "30", "01", "67", "2b", "fe", "d7", "ab", "76"},
	{"ca", "82", "c9", "7d", "fa", "59", "47", "f0", "ad", "d4", "a2", "af", "9c", "a4", "72", "c0"},
	{"b7", "fd", "93", "26", "36", "3f", "f7", "cc", "34", "a5", "e5", "f1", "71", "d8", "31", "15"},
	{"04", "c7", "23", "c3", "18", "96", "05", "9a", "07", "12", "80", "e2", "eb", "27", "b2", "75"},
	{"09", "83", "2c", "1a", "1b", "6e", "5a", "a0", "52", "3b", "d6", "b3", "29", "e3", "2f", "84"},
	{"53", "d1", "00", "ed", "20", "fc", "b1", "5b", "6a", "cb", "be", "39", "4a", "4c", "58", "cf"},
	{"d0", "ef", "aa", "fb", "43", "4d", "33", "85", "45", "f9", "02", "7f", "50", "3c", "9f", "a8"},
	{"51", "a3", "40", "8f", "92", "9d", "38", "f5", "bc", "b6", "da", "21", "10", "ff", "f3", "d2"},
	{"cd", "0c", "13", "ec", "5f", "97", "44", "17", "c4", "a7", "7e", "3d", "64", "5d", "19", "73"},
	{"60", "81", "4f", "dc", "22", "2a", "90", "88", "46", "ee", "b8", "14", "de", "5e", "0b", "db"},
	{"e0", "32", "3a", "0a", "49", "06", "24", "5c", "c2", "d3", "ac", "62", "91", "95", "e4", "79"},
	{"e7", "c8", "37", "6d", "8d", "d5", "4e", "a9", "6c", "56", "f4", "ea", "65", "7a", "ae", "08"},
	{"ba", "78", "25", "2e", "1c", "a6", "b4", "c6", "e8", "dd", "74", "1f", "4b", "bd", "8b", "8a"},
	{"70", "3e", "b5", "66", "48", "03", "f6", "0e", "61", "35", "57", "b9", "86", "c1", "1d", "9e"},
	{"e1", "f8", "98", "11", "69", "d9", "8e", "94", "9b", "1e", "87", "e9", "ce", "55", "28", "df"},
	{"8c", "a1", "89", "0d", "bf", "e6", "42", "68", "41", "99", "2d", "0f", "b0", "54", "bb", "16"}
};

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

State::~State() {
	free(s);
}

void State::update(unsigned char sp[4][4]) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			s[i][j] = sp[i][j];
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
word State::getWord(int col) {
	return word(getByte(state_pos(col, 0)), getByte(state_pos(col,1)),
		getByte(state_pos(col, 2)), getByte(state_pos(col, 3)));
}

void State::setWord(struct word w, int col) {
	for (int i = 0; i < 4; i++)
		s[i][col] = w.getByte(i);
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

/**
 * Performs 'SubBytes' operation on this state
 *
 * Reference: FIPS-197, section 5.1.1
 */
void State::SubBytes() {
	int r, c;
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			unsigned char value = s[r][c];
			unsigned char upper_half = (unsigned char)(value>>4);
			unsigned char lower_half = (unsigned char)(value & ((unsigned char)15));
			
			const char sub_hex[3] = {
				sbox[(int)upper_half][(int)lower_half][0],
				sbox[(int)upper_half][(int)lower_half][1]
			};
			
			unsigned sub;
			sscanf( sub_hex, "%2x", &sub );
			
			// Update state-entry with substitute value.
			s[r][c] = (unsigned char)sub;
		}
	}
}

/**
 * Performs 'ShiftRows()' operation on current state
 * Cyclically shifts (to the left) the last three rows
 *
 * Reference: FIPS-197, section 5.1.2
 */
void State::ShiftRows() {
	int r, c, Nb = 4;
	unsigned char sp[4][4];
	unsigned char temp;
	
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			sp[r][c] = s[r][ (c+r%Nb) % Nb ];
		}
	}
	
	update(sp); // update current state using the values new state sp;
	
}

/**
 * Performs 'MixColumns()' operation on current state
 *
 * Reference: FIPS-197, section 5.1.3
 */
void State::MixColumns() {
	
	//TODO:  need to implement
	return;
}

/**
 * Performs 'AddRoundKey' operation on this state
 *
 * Reference: FIPS-197, section 5.1.4
 */
void State::AddRoundKey(struct word w[], int round, int Nb) {
	//TODO:  need to implement
	//for each column
	for (int c = 0; c < 4; c++) {
		setWord(getWord(c) ^ w[round*Nb + c], c);
	}
	printf("Result of AddRoundKey --> \n");
	displayFancy();
	return;
}

/**
 * Public method that displays the current contents of the 'state'
 */
void State::display() {
	if(display_style==DISPLAY_FANCY) {
		displayFancy();
		return;
	}
	
	int i, j;
	for(i = 0; i < 4; i++) {
		printf("\t");
		for(j = 0; j < 4; j++) {
			printf("%02x ", s[i][j]);
		}
		printf("\n");
	}
}

/**
 * Private method that displays the current contents of the 'state'
 * This method is supposed to be called by 'display()' method if 'fancy' display is needed.
 */
void State::displayFancy() {
	int i, j;
	printf("\t ");
	for(j = 0; j < 3; j++) {
		printf("---- ");
	}
	printf("----");
	printf("\n");
	
	for(i = 0; i < 4; i++) {
		printf("\t|");
		for(j = 0; j < 4; j++) {
			printf(" %02x |", s[i][j]);
		}
		printf("\n\t ");
		for(j = 0; j < 3; j++) {
			if(i<3) {
				printf("----|");
			} else {
				printf("---- ");
			}
		}
		printf("----");
		printf("\n");
	}
}
