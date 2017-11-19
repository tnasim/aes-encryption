#ifndef KEY_EXP_H
#define KEY_EXP_H

#include <string>

// this was necessary to access private properties of AES class.
// I could not find better way to do this.
#define private public

#include "../includes/aes.h"

namespace keyExpansionTest
{
bool test(std::string key_hex, std::string key_expansion[], KEY_SIZE key_size);
}

#endif
