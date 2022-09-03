CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = guess
OBJS = guess.o practice.o

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

guess.o: guess.c guess.h
	$(CC) $(CFLAGS) -c guess.c

practice.o: practice.c practice.h
	$(CC) $(CFLAGS) -c practice.c

clean:
	rm *.o
