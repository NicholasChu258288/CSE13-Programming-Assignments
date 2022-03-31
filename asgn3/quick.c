#include "quick.h"

#include "stats.h"

#include <stdint.h>
#include <stdio.h>

//Will be used to divide the array for quicksort, partitions sorted and unsorted
uint32_t partition(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {

    uint32_t i = low - 1;

    // Check partition when testing to see if conditionals are correct
    for (uint32_t j = low; j < high; j += 1) {
        if (cmp(stats, A[j - 1], A[high - 1]) == -1) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[high - 1]);
    return i + 1;
}

//Recursively sorts using partitions
void quick_sorter(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {
    if (low < high) {
        uint32_t pivot = partition(stats, A, low, high);
        quick_sorter(stats, A, low, pivot - 1);
        quick_sorter(stats, A, pivot + 1, high);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
