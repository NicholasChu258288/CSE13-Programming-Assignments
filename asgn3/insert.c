#include "insert.h"

#include "stats.h"

#include <stdint.h>
#include <stdio.h>

void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {

    for (uint32_t k = 1; k < n; k += 1) {
        //Creates temperary values to use when inserting.
        uint32_t tmp_index = k;
        uint32_t tmp_element = move(stats, A[k]);

        //Goes over elements and compares them with previous values.
        //Moves any values greater than it behind it.
        while (tmp_index > 0 == 1 && cmp(stats, tmp_element, A[tmp_index - 1]) == -1) {
            A[tmp_index] = move(stats, A[tmp_index - 1]);
            tmp_index -= 1;
        }

        A[tmp_index] = move(stats, tmp_element);
    }
    return;
}
