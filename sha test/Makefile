CFLAGS = -O
CC = gcc
CXX = g++

hash: hashmain.o sha1.o
	$(CXX) $(CFLAGS) -o hash hashmain.o sha1.o

main.o: hashmain.cpp 
	$(CXX) $(CFLAGS) -c hashmain.cpp

sha1.o: sha1.cpp
	$(CXX) $(CFLAGS) -c sha1.cpp

clean:
	rm -f core *.o