CC=gcc
AR=ar
FLAGS= -Wall -g

all: solution
solution: solution.c   
	$(CC) $(FLAGS) solution.c -o solution -lm -lpthread
primeSlow: primeSlow.c   
	$(CC) $(FLAGS) primeSlow.c -o primeSlow -lm -lpthread

.PHONY: clean all

clean:
	rm -f *.o solution primeSlow
