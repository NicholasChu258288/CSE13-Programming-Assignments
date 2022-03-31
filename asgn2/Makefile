CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: integrate

integrate: integrate.o functions.o mathlib.o
	$(CC) -o integrate integrate.o functions.o mathlib.o

integrate.o: integrate.c
	$(CC) $(CFLAGS) -c integrate.c -o integrate.o

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c -o functions.o

mathlib.o: mathlib.c
	$(CC) $(CFLAGS) -c mathlib.c -o mathlib.o

clean:
	rm -f functions integrate mathlib *.o

format:
	clang-format -i -style=file *.[ch]
