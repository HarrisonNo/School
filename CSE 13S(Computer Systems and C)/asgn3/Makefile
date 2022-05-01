#Mostly copied from my own asgn2 Makefile
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: sorting

sorting: sorting.o insert.o heap.o quick.o shell.o stats.o
	$(CC) $(CFLAGS) sorting.o insert.o heap.o quick.o shell.o stats.o -o sorting -lm

#Object files
insert.o: insert.c insert.h
	$(CC) $(CFLAGS) -c insert.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

quick.o: quick.c quick.h
	$(CC) $(CFLAGS) -c quick.c

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c

sorting.o: sorting.c stats.h
	$(CC) $(CFLAGS) -c sorting.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f sorting *.o *.exe