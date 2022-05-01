CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: banhammer

banhammer: banhammer.o bf.o bst.o bv.o ht.o node.o speck.o parser.o
	$(CC) $(CFLAGS) banhammer.o bf.o bst.o bv.o ht.o node.o speck.o parser.o -o banhammer

#Object files
bf.o: bf.c bf.h
	$(CC) $(CFLAGS) -c bf.c

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c

bv.o: bv.c bv.h
	$(CC) $(CFLAGS) -c bv.c

ht.o: ht.c ht.h
	$(CC) $(CFLAGS) -c ht.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

speck.o: speck.c speck.h
	$(CC) $(CFLAGS) -c speck.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f banhammer *.o