CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: sorting

sorting: sorting.o quick.o insert.o heap.o batcher.o stats.o
	$(CC) -o sorting sorting.o quick.o insert.o heap.o batcher.o stats.o -lm

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c -o sorting.o

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c -o quick.o

insert.o: insert.c
	$(CC) $(CFLAGS) -c insert.c -o insert.o

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c -o heap.o

batcher.o: batcher.c
	$(CC) $(CFLAGS) -c batcher.c -o batcher.o

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c -o stats.o


clean:
	rm -f sorting quick insert heap batcher stats *.o

format:
	clang-format -i -style=file *.[ch]
