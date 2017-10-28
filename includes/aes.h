#ifndef AES_H
#define AES_H

#include "../includes/state.h"

class AES {
    
private:
    char key_[32];
	inline void initKey(char key[]) {
		strcpy(key_, key);
	}

public:
    
    /**
     * Constructor - takes the key and initializes
     **/
    AES(char key[]);

	/**
	 * Perform 'SubBytes' operation on the state.
	 */
	void SubBytes(State state);

    /**
     * Perform 'ShiftRows' operation on the state.
     */
    void ShiftRows(State state);
    
    /**
     * Perform 'MixColumns' operation on the state.
     */
    void MixColumns(State state);
    
    /**
     * Perform 'AddRoundKey' operation on the state.
     */
    void AddRoundKey(State state);
    
    /**
     * Perform the AES Cipher operation on 'input' and puts the resulting cipher in 'output'.
     */
    void Cipher(char input[], char output[], char w[]);
     

};

#endif
