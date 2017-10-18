#ifndef STATE_H
#define STATE_H


struct state_pos {
	int col;
	int row;

};

class state {

private:
	/**
	 * each state is a 4x4 block of bytes
	 */
	unsigned char array[4][4];

public:

	/**
	 * Takes in one 16-byte "block"
	 */
	state(char block[16]);

	/**
	 * retrieves byte at position pos.
	 */
	unsigned char getbyte(state_pos pos);

	/**
	 * Swaps byte at position 'b1'and position 'b2'
	 */
	void swapbytes(state_pos b1, state_pos b2);

};

#endif
