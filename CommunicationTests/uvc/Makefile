CC=g++
CFLAGS=-I../../libuvc/include -luvc

all: test

test: test.o
	$(CC) $(CFLAGS) test.o -o test

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f test test.o
