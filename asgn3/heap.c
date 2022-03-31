#include "heap.h"

#include "stats.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

//Finds max child of a parent value in the heap.
uint32_t max_child(Stats *stats, uint32_t *A, int first_index, int last_index) {
    // Saves child indexes
    int left_child_index = 2 * first_index;
    int right_child_index = 2 * first_index + 1;

    //Checks if there exists a right and left child and compares if there are.
    if (right_child_index <= last_index
        && cmp(stats, A[right_child_index - 1], A[left_child_index - 1]) == 1) {
        return right_child_index;
    }
    return left_child_index;
}

void fix_heap(Stats *stats, uint32_t *A, int first_index, int last_index) {
    // fixed is to end loop when heap has been fixed.
    // Using 0 and 1 for true and false.
    int fixed = 0;
    int fix_parent_index = first_index;

    // Uses max_child function to find max child.
    uint32_t greatest_child_index = max_child(stats, A, fix_parent_index, last_index);

    //Switches greatest child with parent when child is larger than parent
    while (fix_parent_index <= floor(last_index / 2) && fixed == 0) {
        if (cmp(stats, A[fix_parent_index - 1], A[greatest_child_index - 1]) == -1) {
            swap(stats, &A[fix_parent_index - 1], &A[greatest_child_index - 1]);
            fix_parent_index = greatest_child_index;
            greatest_child_index = max_child(stats, A, fix_parent_index, last_index);
        } else {
            fixed = 1;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, int first_index, int last_index) {
    //Fixes and issues and builds the heap.
    for (int parent_index = floor(last_index / 2); parent_index > first_index - 1;
         parent_index -= 1) {
        fix_heap(stats, A, parent_index, last_index);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //Technically still working with base index 0, but this is to pretend using base 1.
    int first_index = 1;
    int last_index = n;
    build_heap(stats, A, first_index, last_index);

    //Begins to heap sorting using the heap.
    for (int leaf = last_index; leaf > first_index; leaf -= 1) {
        swap(stats, &A[first_index - 1], &A[leaf - 1]);
        fix_heap(stats, A, first_index, leaf - 1);
    }
}
