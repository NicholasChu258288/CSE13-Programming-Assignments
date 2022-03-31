CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: identify

identify: identify.o pq.o text.o bf.o bv.o ht.o node.o parser.o speck.o
	$(CC) $(CFLAGS) -o identify identify.o pq.o text.o bf.o bv.o ht.o node.o parser.o speck.o -lm

identify.o: identify.c
	$(CC) $(CFLAGS) -c identify.c -o identify.o

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c -o pq.o

text.o: text.c
	$(CC) $(CFLAGS) -c text.c -o text.o

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c -o bf.o

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c -o bv.o

ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c -o ht.o

node.o: node.c
	$(CC) $(CFLAGS) -c node.c -o node.o

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c -o speck.o

clean:
	rm -f identify pq text bf bv ht node parser speck *.o
	rm -f identify

format:
	clang-format -i --style=file identify.c pq.c text.c bf.c bv.c ht.c node.c parser.c speck.c
	clang-format -i --style=file pq.h text.h bf.h bv.h ht.h node.h parser.h speck.h metric.h
