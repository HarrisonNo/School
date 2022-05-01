CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: decrypt encrypt keygen

decrypt: decrypt.o numtheory.o randstate.o rsa.o
	$(CC) $(CFLAGS) decrypt.o numtheory.o randstate.o rsa.o -o decrypt -lgmp

encrypt: encrypt.o numtheory.o randstate.o rsa.o
	$(CC) $(CFLAGS) encrypt.o numtheory.o randstate.o rsa.o -o encrypt -lgmp

keygen: keygen.o numtheory.o randstate.o rsa.o
	$(CC) $(CFLAGS) keygen.o numtheory.o randstate.o rsa.o -o keygen -lgmp

#Object files
numtheory.o: numtheory.c numtheory.h
	$(CC) $(CFLAGS) -c numtheory.c

randstate.o: randstate.c randstate.h
	$(CC) $(CFLAGS) -c randstate.c

rsa.o: rsa.c rsa.h
	$(CC) $(CFLAGS) -c rsa.c

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

format:
	clang-format -i -style=file *.c *.h

clean:
	rm -f decrypt encrypt keygen *.o