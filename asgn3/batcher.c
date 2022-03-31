#include "batcher.h"

#include "stats.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

//Compares values at x and y index and swaps them if value at x is larger than value at y.
void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        swap(stats, &A[x], &A[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //Special case when array has no elements to prevent problems
    if (n == 0) {
        return;
    }

    //This is to find which indicies are being compared
    uint32_t length = n;

    //Using log2() function from math.h to find bit length
    uint32_t bit_length = log2(n) + 1;

    //Gets gap to use for comparing
    uint32_t gap1 = 1 << (bit_length - 1);

    while (gap1 > 0) {

        uint32_t gap2 = 1 << (bit_length - 1);
        uint32_t r = 0;
        uint32_t gap_dist = gap1;

        while (gap_dist > 0) {

            for (uint32_t i = 0; i < length - gap_dist; i += 1) {

                //Partitioning the array and sorting k
                //Checking if i and gap_dist are 0, and as in the bitwise operation
                if ((i & gap_dist) == r) {
                    //Compares the two elements seperated by the gap in the array
                    //Swaps when element in i position is larger than i+gap_dist
                    comparator(stats, A, i, i + gap_dist);
                }
            }

            gap_dist = gap2 - gap1;

            //Shifts by 1 bit, << is times 2 and >> is dividing
            gap2 >>= 1;

            r = gap1;
        }

        gap1 >>= 1;
    }
}
