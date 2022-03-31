#include "pq.h"
#include "node.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

struct PriorityQueue {
    uint32_t head; //Holds index for head of priority queue
    uint32_t tail; //Holds index for tail of priority queue
    uint32_t capacity;
    Node **queue;
};

//Helper functions for min heap which will be used for implementing the priority queue
void swap(Node *x, Node *y) {
    //Swaps nodes x and y
    Node tmp = *x;
    *x = *y;
    *y = tmp;
}

uint32_t min_child(PriorityQueue *q, uint32_t first, uint32_t last) {
    //Finds node with smallest frequency
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && q->queue[right - 1]->frequency < q->queue[left - 1]->frequency) {
        return right;
    }
    return left;
}

void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t parent = first;
    uint32_t child = min_child(q, parent, last);
    while (parent <= last / 2 && found == false) {
        if (q->queue[parent - 1]->frequency > q->queue[child - 1]->frequency) {
            swap(q->queue[parent - 1], q->queue[child - 1]);
            parent = child;
            child = min_child(q, parent, last);
        } else {
            found = true;
        }
    }
}

void build_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    for (uint32_t parent = last / 2; parent > first - 1; parent -= 1) {
        fix_heap(q, parent, last);
    }
}

//Functions for priority queue
PriorityQueue *pq_create(uint32_t capacity) {
    //Creates and returns a new priority queue of specified capacity
    PriorityQueue *q = (PriorityQueue *) (malloc(sizeof(PriorityQueue)));
    //"Starting" at index 1 because using heap to simulate base indexing of 1
    q->head = 1;
    q->tail = 1;
    q->capacity = capacity;
    q->queue = (Node **) calloc(capacity, sizeof(Node));
    return q;
}

void pq_delete(PriorityQueue **q) {
    free((*q)->queue);
    free(*q);
    *q = NULL;
}

bool pq_empty(PriorityQueue *q) {
    if (q->head == q->tail) {
        return true;
    } else {
        return false;
    }
}

bool pq_full(PriorityQueue *q) {
    if (q->head == q->capacity) {
        return true;
    } else {
        return false;
    }
}

uint32_t pq_size(PriorityQueue *q) {
    return q->head;
}

bool enqueue(PriorityQueue *q, Node *n) {
    //Adds new node to priority queue, then rebuilds the heap
    if (pq_full(q)) {
        return false;
    } else {
        q->queue[q->head - 1] = n;
        build_heap(q, q->tail, q->head);
        q->head = q->head + 1;
        return true;
    }
}

bool dequeue(PriorityQueue *q, Node **n) {
    //Pops element from queue and rebuilds head
    if (pq_empty(q)) {
        return false;
    } else {
        *n = q->queue[q->head - 2];
        //Swaps tail and head node, decrements head, and rebuilds heap
        //This is a work around for the limited capacity
        swap(q->queue[q->tail - 1], q->queue[q->head - 2]);
        q->head = q->head - 1;
        build_heap(q, q->tail, q->head - 1);
        return true;
    }
    return false;
}

void pq_print(PriorityQueue *q) {
    printf("[");
    for (uint32_t i = q->tail - 1; i < q->head - 1; i += 1) {
        printf("(%c", q->queue[i]->symbol);
        printf(",%" PRIu64 ")", q->queue[i]->frequency);
        if ((i + 1) < q->head - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
