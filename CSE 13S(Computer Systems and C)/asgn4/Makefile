#Mostly copied from my own asgn2 Makefile
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: tsp

tsp: tsp.o graph.o stack.o path.o
	$(CC) $(CFLAGS) tsp.o graph.o stack.o path.o -o tsp

#Object files
graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

path.o: path.c path.h
	$(CC) $(CFLAGS) -c path.c

tsp.o: tsp.c
	$(CC) $(CFLAGS) -c tsp.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f tsp *.o *.exe