all:
	g++ -c BigInt.cpp
	g++ -c main.cpp
	g++ -o BigInt BigInt.o main.o

clean:
	rm -f *.o BigInt