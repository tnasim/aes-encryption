all: test

util.o: src/util/util.cpp src/util/util.h
	g++ -c src/util/util.cpp -o bin/util.o

test: src/test.cpp util.o
	g++ src/test.cpp bin/util.o -o bin/test

clean:
	@- $(RM) test
