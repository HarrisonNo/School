CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: encode decode

encode: encode.o huffman.o code.o io.o pq.o stack.o node.o
	$(CC) $(CFLAGS) encode.o huffman.o code.o io.o pq.o stack.o node.o -o encode

decode: decode.o huffman.o code.o io.o pq.o stack.o node.o
	$(CC) $(CFLAGS) decode.o huffman.o code.o io.o pq.o stack.o node.o -o decode

#Object files
huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

code.o: code.c code.h
	$(CC) $(CFLAGS) -c code.c

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c

pq.o: pq.c pq.h
	$(CC) $(CFLAGS) -c pq.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f encode decode *.o