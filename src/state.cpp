
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

const char State::sbox_inv[16][16][3]
{
	{"52", "09", "6a", "d5", "30", "36", "a5", "38", "bf", "40", "a3", "9e", "81", "f3", "d7", "fb"},
	{"7c", "e3", "39", "82", "9b", "2f", "ff", "87", "34", "8e", "43", "44", "c4", "de", "e9", "cb"},
	{"54", "7b", "94", "32", "a6", "c2", "23", "3d", "ee", "4c", "95", "0b", "42", "fa", "c3", "4e"},
	{"08", "2e", "a1", "66", "28", "d9", "24", "b2", "76", "5b", "a2", "49", "6d", "8b", "d1", "25"},
	{"72", "f8", "f6", "64", "86", "68", "98", "16", "d4", "a4", "5c", "cc", "5d", "65", "b6", "92"},
	{"6c", "70", "48", "50", "fd", "ed", "b9", "da", "5e", "15", "46", "57", "a7", "8d", "9d", "84"},
	{"90", "d8", "ab", "00", "8c", "bc", "d3", "0a", "f7", "e4", "58", "05", "b8", "b3", "45", "06"},
	{"d0", "2c", "1e", "8f", "ca", "3f", "0f", "02", "c1", "af", "bd", "03", "01", "13", "8a", "6b"},
	{"3a", "91", "11", "41", "4f", "67", "dc", "ea", "97", "f2", "cf", "ce", "f0", "b4", "e6", "73"},
	{"96", "ac", "74", "22", "e7", "ad", "35", "85", "e2", "f9", "37", "e8", "1c", "75", "df", "6e"},
	{"47", "f1", "1a", "71", "1d", "29", "c5", "89", "6f", "b7", "62", "0e", "aa", "18", "be", "1b"},
	{"fc", "56", "3e", "4b", "c6", "d2", "79", "20", "9a", "db", "c0", "fe", "78", "cd", "5a", "f4"},
	{"1f", "dd", "a8", "33", "88", "07", "c7", "31", "b1", "12", "10", "59", "27", "80", "ec", "5f"},
	{"60", "51", "7f", "a9", "19", "b5", "4a", "0d", "2d", "e5", "7a", "9f", "93", "c9", "9c", "ef"},
	{"a0", "e0", "3b", "4d", "ae", "2a", "f5", "b0", "c8", "eb", "bb", "3c", "83", "53", "99", "61"},
	{"17", "2b", "04", "7e", "ba", "77", "d6", "26", "e1", "69", "14", "63", "55", "21", "0c", "7d"}
};


/**
 * Takes in the input byte array and initializes the 'state' array with this input.
 */
State::State(unsigned char input[])
{
	/**
	 *  Secure Coding, SEI - ARR30-C. Do not form or use out-of-bounds pointers or array subscripts
	 */
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
	/**
	 *  Secure Coding, SEI - ARR30-C. Do not form or use out-of-bounds pointers or array subscripts
	 */
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
word State::getWord(size_t col) {
	/**
	 * Secure Coding, SEI - MSC11-C. Incorporate diagnostic tests using assertions (should not be used for validating user input)
	 * Secure Coding, SEI - ARR30-C. Do not form or use out-of-bounds pointers or array subscripts
	 */
	
	// TODO: must ensure that the value of 'col' is within permitted range. Should not use assert here.
	struct word w = word(getByte(state_pos(col, 0)), getByte(state_pos(col,1)),
		getByte(state_pos(col, 2)), getByte(state_pos(col, 3)));
	return w;
}

void State::setWord(struct word w, size_t col) {
	for (int i = 0; i < 4; i++)
		s[i][col] = w.getByte(i);
}

/**
 * Get the final result from the state
 **/
unsigned char* State::getOutput() {
	unsigned char* output;
	int r, c;
	
	/**
	 *	Secure Coding, SEI - ERR33-C. Detect and handle standard library errors (check return value from malloc --- it might be NULL)
	 */
	// TODO: need to check whether the allocated space is valid or NULL.
	
	output = (unsigned char*) malloc(sizeof(unsigned char)*16);
	
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			output[r+4*c] = s[r][c];
		}
	}
	/**
	 *	Secure Coding, SEI - DCL30-C. Declare objects with appropriate storage durations
	 */
	// TODO: need to pass 'output' from outside of the method.
	
	/**
	 * Secure Coding, SEI - MEM51-CPP. "Properly deallocate dynamically allocated resources"
	 */
	
	// TODO: must ensure that the 'output' gets 'free'ed after it is no longer needed.
	//		we can use std::malloc() and then std::free()
	//	or we can take 'output' as an input argument of this method and
	//		make it the responsibility of the caller to free the memory after it is no longer needed.
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
			/**
			 *	Secure Coding, SEI - ERR33-C. Detect and handle standard library errors (for sscanf, it might return EOF/negative)
			 */
			// TODO: need to check if sscanf was successful or not.
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
	
	int i;
	unsigned char s0, s1, s2, s3;
	unsigned char a, b, c, d;


	for(i=0;i<4;i++){

		s0 = s[0][i];
		s1 = s[1][i];
		s2 = s[2][i];
		s3 = s[3][i];

		/**
		 * Multiply the column by a(x) in GF(2^8)
		 * a(x) = {03}.x^3 + {01}.x^2 + {01}.x + {02}
		 * */
		a = util::polyMultiply( s0, 2 ) ^ util::polyMultiply( s1, 3 ) ^ s2 ^ s3;
		b = s0 ^ util::polyMultiply( s1, 2 ) ^ util::polyMultiply( s2, 3 ) ^ s3;
		c = s0 ^ s1 ^ util::polyMultiply( s2, 2 ) ^ util::polyMultiply( s3, 3 );
		d = util::polyMultiply( s0, 3 ) ^ s1 ^ s2 ^ util::polyMultiply( s3, 2 );

		s[0][i] = a;
		s[1][i] = b;
		s[2][i] = c;
		s[3][i] = d;
		
	}

}	
	
/**
 * Performs 'AddRoundKey' operation on this state
 *
 * Reference: FIPS-197, section 5.1.4
 */
void State::AddRoundKey(struct word w[], size_t round, size_t Nb) {
	//for each column
	for (int c = 0; c < 4; c++) {
		struct word key = w[round*Nb + c];
		struct word w = getWord(c);
		setWord(key ^ w, c);
	}
	return;
}


/**
 * Performs 'InvSubBytes' operation on this state
 *
 * Reference: FIPS-197, section 5.3.2
 */
void State::InvSubBytes() {
	int r, c;
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			unsigned char value = s[r][c];
			unsigned char upper_half = (unsigned char)(value>>4);
			unsigned char lower_half = (unsigned char)(value & ((unsigned char)15));

			const char sub_hex[3] = {
				sbox_inv[(int)upper_half][(int)lower_half][0],
				sbox_inv[(int)upper_half][(int)lower_half][1]
			};

			unsigned sub;
			sscanf( sub_hex, "%2x", &sub );

			// Update state-entry with substitute value.
			s[r][c] = (unsigned char)sub;
		}
	}
}

/**
 * Performs 'InvShiftRows()' operation on current state
 * Cyclically shifts (to the left) the last three rows
 *
 * Reference: FIPS-197, section 5.3.1
 */
void State::InvShiftRows() {
	int r, c, Nb = 4;
	unsigned char sp[4][4];
	unsigned char temp;

	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			sp[r][ (c+r%Nb) % Nb ] = s[r][c];
		}
	}

	update(sp); // update current state using the values new state sp;

}

/**
 * Performs 'InvMixColumns()' operation on current state
 *
 * Reference: FIPS-197, section 5.3.3
 */
void State::InvMixColumns() {

	int i;
	unsigned char s0, s1, s2, s3;
	unsigned char a, b, c, d;


	for(i=0;i<4;i++){

		s0 = s[0][i];
		s1 = s[1][i];
		s2 = s[2][i];
		s3 = s[3][i];

		/**
		 * Multiply the column by a(x) in GF(2^8)
		 * a(x) = {0b}.x^3 + {0d}.x^2 + {09}.x + {0e}
		 *
		 * hex: 0e, 0b, 0d, 09
		 * dec: 14, 11, 13, 09
		 * */
		a = 	util::polyMultiply( s0, 14 )
			^ 	util::polyMultiply( s1, 11 )
			^	util::polyMultiply( s2, 13 )
			^	util::polyMultiply( s3,  9 );

		b = 	util::polyMultiply( s0,  9 )
			^ 	util::polyMultiply( s1, 14 )
			^	util::polyMultiply( s2, 11 )
			^	util::polyMultiply( s3, 13 );

		c = 	util::polyMultiply( s0, 13 )
			^ 	util::polyMultiply( s1,  9 )
			^	util::polyMultiply( s2, 14 )
			^	util::polyMultiply( s3, 11 );

		d = 	util::polyMultiply( s0, 11 )
			^ 	util::polyMultiply( s1, 13 )
			^	util::polyMultiply( s2,  9 )
			^	util::polyMultiply( s3, 14 );

		s[0][i] = a;
		s[1][i] = b;
		s[2][i] = c;
		s[3][i] = d;

	}

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
