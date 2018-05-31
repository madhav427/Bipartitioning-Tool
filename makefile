parprog: parprog.o
	g++ -o parprog parprog.o

parprog.o: parprog.o
	g++ -c -O3 -std=c++0x parprog.cpp

clean:
	rm parprog.o
