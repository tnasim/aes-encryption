all: test

util.o: util/util.cpp util/util.h
	g++ -c util/util.cpp

test: test.cpp util.o
	g++ test.cpp util.o -o test

clean: 
	@- $(RM) test