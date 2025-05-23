CC = gcc
CFLAGS = -Wall -Wextra -g
INCLUDES = -Iheaders

OBJ = main.o prim.o

prim: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o prim

main.o: main.c headers/prim.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c

prim.o: prim.c headers/prim.h
	$(CC) $(CFLAGS) $(INCLUDES) -c prim.c

clean:
	rm -f *.o prim
