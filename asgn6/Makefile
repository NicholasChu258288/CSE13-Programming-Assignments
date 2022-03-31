CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: encode decode

decode: decode.o huffman.o stack.o io.o code.o pq.o node.o
	$(CC) $(CFLAGS) -o decode decode.o huffman.o stack.o io.o code.o pq.o node.o

encode: encode.o huffman.o stack.o io.o code.o pq.o node.o
	$(CC) $(CFLAGS) -o encode encode.o huffman.o stack.o io.o code.o pq.o node.o

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c -o decode.o

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c -o encode.o

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c -o huffman.o

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c -o stack.o

io.o: io.c
	$(CC) $(CFLAGS) -c io.c -o io.o

code.o: code.c
	$(CC) $(CFLAGS) -c code.c -o code.o

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c -o pq.o

node.o: node.c
	$(CC) $(CFLAGS) -c node.c -o node.o

clean:
	rm -f huffman stack node io code node *.o
	rm -f encode decode

format:
	clang-format -i --style=file encode.c decode.c huffman.c stack.c io.c code.c pq.c node.c
	clang-format -i --style=file huffman.h stack.h io.h code.h pq.h node.h

