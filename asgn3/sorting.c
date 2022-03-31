#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Library for getopt

void usage() {
    printf("Synopsis\n"
           "	Sorts given array based off specified sorting algorithm. \n"
           "Usage\n"
           "	./sorting [ahbiqr:n:p:H] [-ahbiq] [-r seed] [-n size] [-p elements] [-H]"
           "Options\n"
           "	-a	    	Use all sorting algorithms.\n"
           "	-h	    	Use heap sort.\n"
           "	-b	    	Use batcher sort.\n"
           "	-i	    	Use insert sort.\n"
           "	-q	    	Uses quick sort.\n"
           "	-r seed         Set random seed to seed, default seed is 13371453.\n"
           "	-n size		Sets array size to size, defualt size is 100.\n"
           "	-p elements	Sets the number of printed array elements to elements, "
           "default is 100.\n"
           "	-H		Prints program usage.\n");
}

int main(int argc, char **argv) {
    //argc = argument count, number of commands invoked
    //argv = argument vector, pointer to array string of arguments

    int opt = 0;
    int h = 0;
    int b = 1;
    int i = 2;
    int q = 3;

    //Setting default values
    int seed = 13371453;
    int size = 100;
    int elements = 100;

    //Set containing the sorting algorithms the user wants
    Set sorting_algorithms = empty_set();

    //Takes command line inputs
    while ((opt = getopt(argc, argv, "ahbiqr:n:p:H")) != -1) {
        switch (opt) {
        case 'a':
            //Adding all sorts to the set
            sorting_algorithms = insert_set(h, sorting_algorithms);
            sorting_algorithms = insert_set(b, sorting_algorithms);
            sorting_algorithms = insert_set(i, sorting_algorithms);
            sorting_algorithms = insert_set(q, sorting_algorithms);
            break;
        case 'h':
            //Adding just heap sort to the set
            sorting_algorithms = insert_set(h, sorting_algorithms);
            break;
        case 'b':
            //Adding just batcher sort to the set
            sorting_algorithms = insert_set(b, sorting_algorithms);
            break;
        case 'i':
            //Adding just insert sort to the set
            sorting_algorithms = insert_set(i, sorting_algorithms);
            break;
        case 'q':
            //Adding just quicksort to the set
            sorting_algorithms = insert_set(q, sorting_algorithms);
            break;
        case 'r':
            //Setting new seed, random seed
            seed = atoi(optarg);
            break;
        case 'n':
            //Setting new size, size of array
            size = atoi(optarg);
            break;
        case 'p':
            //Setting new elements, times to print
            elements = atoi(optarg);
            break;
        case 'H': usage(); break;
        default: usage(); break;
        }
    }

    if (elements > size) {
        elements = size;
    }

    //Creates stats struct to be used for counting stats when sorting
    Stats sort_stats;
    reset(&sort_stats);

    //Checks to see if i was a chosen option and insertion sorts if it was
    if (member_set(i, sorting_algorithms)) {
        srand(seed);
        uint32_t *array = (uint32_t *) malloc(size * 4);
        for (int i = 0; i < size; i++) {
            array[i] = rand() & 0x3fffffff;
        }

        insertion_sort(&sort_stats, array, size);
        printf("Insertion Sort, %d ", size);
        printf("elements, %" PRIu64, sort_stats.moves);
        printf(" moves,  %" PRIu64, sort_stats.compares);
        printf(" compares\n");

        int row_size = 0;
        for (int i = 0; i < elements; i += 1) {
            row_size += 1;
            printf("%13" PRIu32, array[i]);
            if (row_size == 5) {
                printf("\n");
                row_size = 0;
            }
        }
        reset(&sort_stats);
        free(array);
    }

    //Checks to see if b was a chosen option and batcher sorts if it was
    if (member_set(b, sorting_algorithms)) {
        srand(seed);
        uint32_t *array = (uint32_t *) malloc(size * 4);
        for (int i = 0; i < size; i++) {
            array[i] = rand() & 0x3fffffff;
        }

        batcher_sort(&sort_stats, array, size);
        printf("Batcher Sort, %d ", size);
        printf("elements, %" PRIu64, sort_stats.moves);
        printf(" moves, %" PRIu64, sort_stats.compares);
        printf(" compares\n");

        int row_size = 0;
        for (int i = 0; i < elements; i += 1) {
            row_size += 1;
            printf("%13" PRIu32, array[i]);
            if (row_size == 5) {
                printf("\n");
                row_size = 0;
            }
        }
        free(array);
        reset(&sort_stats);
    }

    //Checks to see if h was a chosen option and heap sorts if it was
    if (member_set(h, sorting_algorithms)) {

        srand(seed);
        uint32_t *array = (uint32_t *) malloc(size * 4);
        for (int i = 0; i < size; i++) {
            array[i] = rand() & 0x3fffffff;
        }

        heap_sort(&sort_stats, array, size);
        printf("Heap Sort, %d ", size);
        printf("elements, %" PRIu64, sort_stats.moves);
        printf(" moves, %" PRIu64, sort_stats.compares);
        printf(" compares\n");

        int row_size = 0;
        for (int i = 0; i < elements; i += 1) {
            row_size += 1;
            printf("%13" PRIu32, array[i]);
            if (row_size == 5) {
                printf("\n");
                row_size = 0;
            }
        }
        free(array);
        reset(&sort_stats);
    }

    //Checks to see if q was a chosen option and quick sorts if it was
    if (member_set(q, sorting_algorithms)) {

        srand(seed);
        uint32_t *array = (uint32_t *) malloc(size * 4);
        for (int i = 0; i < size; i++) {
            array[i] = rand() & 0x3fffffff;
        }

        quick_sort(&sort_stats, array, size);
        printf("Quick Sort, %d ", size);
        printf("elements, %" PRIu64, sort_stats.moves);
        printf(" moves, %" PRIu64, sort_stats.compares);
        printf(" compares\n");

        int row_size = 0;
        for (int i = 0; i < elements; i += 1) {
            row_size += 1;
            printf("%13" PRIu32, array[i]);
            if (row_size == 5) {
                printf("\n");
                row_size = 0;
            }
        }
        free(array);
        reset(&sort_stats);
    }

    return 0;
}
