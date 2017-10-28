all: test

util.o: src/util/util.cpp src/util/util.h
	g++ -c src/util/util.cpp -o bin/util.o

aes.o: src/aes.cpp includes/aes.h
	g++ -c src/aes.cpp -o bin/aes.o

test: src/test.cpp util.o aes.o
	g++ src/test.cpp bin/util.o bin/aes.o -o bin/test

clean:
	@- $(RM) test
