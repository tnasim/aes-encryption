/*
# This file is used for testing the various functions
# and operations used throughout the project
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "util/util.h"

int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		std::cout << "Need arguments: <text to encrypt>" << std::endl;
		exit(1);
	}

	char* input = argv[1];
	int size = strlen(input);
	//char foo[4] = {'f','o','o',0};

	std::string output = util::charToHex(input, size);
	std::cout << "Testing: " << output << std::endl;
	return 0;
}