all: sum

sum: main.o
	g++ main.o -pthread -o sum

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o sum
