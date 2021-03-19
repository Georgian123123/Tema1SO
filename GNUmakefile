CC=gcc
CFLAGS=-Wall -g

build: so-cpp.o
	$(CC) $^ -o so-cpp

so-cpp.o: so-cpp.c hashmap.c utilities.c hashmap.h utilities.h
	$(CC) $(CFLAGS) -c $<

run:
	./so-cpp
	
.PHONY: clean
clean:
	rm -f *.o *~ so-cpp