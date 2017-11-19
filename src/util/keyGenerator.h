#ifndef KEYGEN_H
#define KEYGEN_H

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdio>

namespace keyGenerator
{
	
/**
 * Generate random key of specified size.
 * size in number of bytes
 */
void generateRandomKey(size_t size, unsigned char outputKey[]);
};

#endif
