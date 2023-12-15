all: clean test

test: 
	g++ -Wall -o test -I ./include src/particle.cpp test.cpp

clean:
	rm test
