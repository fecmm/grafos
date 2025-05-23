CC = gcc
CFLAGS = -Wall -Wextra -O2

all: floyd

floyd: main.o floydWarshall.o
	$(CC) $(CFLAGS) -o floyd main.o floydWarshall.o

main.o: main.c ../headers/floydWarshall.h
	$(CC) $(CFLAGS) -c main.c

floydWarshall.o: floydWarshall.c ../headers/floydWarshall.h
	$(CC) $(CFLAGS) -c floydWarshall.c

clean:
	rm -f *.o floyd
