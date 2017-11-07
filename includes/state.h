#ifndef STATE_H
#define STATE_H
#include "../src/util/util.h"

#define		DISPLAY_NORMAL	0
#define		DISPLAY_FANCY	1

using namespace util;
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

	/**
	 * Display the current contents of the state (with lines)
	 */
	void displayFancy();
	
public:
	
	// 5.1.1: Fig. 7
	static const char sbox[16][16][3];	

	static int const display_style = DISPLAY_FANCY;
	//static int const display_style = DISPLAY_NORMAL;
	
	/**
	 * Takes in the input byte array and initializes the 'state' array with this input.
	 */
	State(unsigned char input[]);
	
	~State();
	
	void update(unsigned char sp[4][4]);

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
	
	/**
	 * Performs 'SubBytes' operation on this state
	 */
	void SubBytes();
	
	/**
	 * Performs 'ShiftRows' operation on this state
	 */
	void ShiftRows();
	
	/**
	 * Performs 'MixColumns' operation on this state
	 */
	void MixColumns();
	
	/**
	 * Performs 'AddRoundKey' operation on this state
	 */
	void AddRoundKey(struct word w[]);
	
	/**
	 * Display the current contents of the state
	 */
	void display();

};

#endif
