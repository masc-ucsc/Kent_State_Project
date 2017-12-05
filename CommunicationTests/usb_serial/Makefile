CC=g++

all: test test.cpp

test: test.o
	$(CC) test.o -o $@

test2: test2.o
	$(CC) test2.o -o $@

test3: test3.o
	$(CC) $< -o $@

.cpp.o:
	$(CC) -c $< -o $@

clean:
	rm test.o test 2>/dev/null
