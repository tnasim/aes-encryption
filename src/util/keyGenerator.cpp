#include "keyGenerator.h"

#include <unistd.h>
#include <fcntl.h>

/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

void keyGenerator::generateRandomKey(size_t size, unsigned char outputKey[])
{
	int myFile = open("/dev/urandom", O_RDONLY);
	unsigned char randomkey[size];
	uint16_t randomNum = read(myFile, &randomkey, sizeof(randomkey[0])*size);
	std::copy(randomkey, randomkey+size, outputKey);
	close(myFile);
}

