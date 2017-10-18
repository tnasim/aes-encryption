/*
# This file is used for testing the various functions
# and operations used throughout the project
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"

int main(int argc, char** argv)
{
	if (argc < 3) 
	{
		std::cout << "Need arguments: <key-size> <text to encrypt>" << std::endl;
		exit(1);
	}
	int keysize = atoi(argv[1]);


	char foo[4] = {'f','o','o',0};
	std::string output = util::charToHex(foo, 4);
	std::cout << "Testing: " << output << std::endl;
	return 0;
}