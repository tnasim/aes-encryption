all: test

util.o: src/util/util.cpp src/util/util.h
	g++ -c src/util/util.cpp

test: src/test.cpp util.o
	g++ src/test.cpp util.o -o test

clean:
	@- $(RM) test
