CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

all: keygen encrypt decrypt

decrypt: decrypt.o rsa.o numtheory.o randstate.o
	$(CC) -o decrypt decrypt.o rsa.o numtheory.o randstate.o $(LFLAGS)

decrypt.o: decrypt.c
	$(CC) -c decrypt.c

encrypt: encrypt.o rsa.o numtheory.o randstate.o
	$(CC) -o encrypt encrypt.o rsa.o numtheory.o randstate.o $(LFLAGS)

encrypt.o: encrypt.c
	$(CC) -c encrypt.c

keygen: keygen.o rsa.o numtheory.o randstate.o
	$(CC) -o keygen keygen.o rsa.o numtheory.o randstate.o $(LFLAGS)

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

rsa.o: rsa.c
	$(CC) $(CFLAGS) -c rsa.c

numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c

randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c

clean:
	rm -f decrypt encrypt keygen rsa numtheory randstate *.o

format:
	clang-format -i --style=file decrypt.c encrypt.c keygen.c rsa.c rsa.h numtheory.c numtheory.h randstate.c randstate.h
