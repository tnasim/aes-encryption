#ifndef STATE_H
#define STATE_H


struct state_pos {
	int col;
	int row;

};

class State {

private:
	/**
	 * each state is a 4x4 block of bytes
	 */
	unsigned char s[4][4];

public:

	/**
	 * Takes in the input byte array and initializes the 'state' array with this input.
	 */
	State(unsigned char input[]);

	/**
	 * retrieves byte at position pos.
	 */
	unsigned char getByte(state_pos pos);

	/**
	 * Swaps byte at position 'b1'and position 'b2'
	 */
	void swapBytes(state_pos b1, state_pos b2);

	/**
	 * Build an array and return the pointer to the
	 * first element in the array.
	 */
	unsigned char* getWord(int col);
	
	unsigned char* getOutput();

};

#endif
