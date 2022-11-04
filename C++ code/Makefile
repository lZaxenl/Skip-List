CFLAGS = -O
CC = gcc
CXX = g++

run: main.o sha1.o
	$(CXX) $(CFLAGS) -o run main.o sha1.o

main.o: main.cpp header.h
	$(CXX) $(CFLAGS) -c main.cpp

sha1.o: sha1.cpp
	$(CXX) $(CFLAGS) -c sha1.cpp

clean:
	rm -f core *.o
