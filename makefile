all: test clean

util.o: src/util/util.cpp src/util/util.h
	g++ -c src/util/util.cpp -o bin/util.o

keyGenerator.o: src/util/keyGenerator.cpp src/util/keyGenerator.h
	g++ -c src/util/keyGenerator.cpp -o bin/keyGenerator.o

state.o: src/state.cpp includes/state.h
	g++ -c src/state.cpp -o bin/state.o

aes.o: src/aes.cpp includes/aes.h
	g++ -c src/aes.cpp -o bin/aes.o

keyexpansiontest.o: test/keyExpansionTest.cpp test/keyExpansionTest.h
	g++ -c test/keyExpansionTest.cpp -o bin/keyexpansiontest.o

test: src/test.cpp util.o keyGenerator.o aes.o state.o keyexpansiontest.o
	g++ src/test.cpp bin/util.o bin/keyGenerator.o bin/aes.o bin/state.o bin/keyexpansiontest.o -o bin/test

clean:
	rm -f bin/*.o
