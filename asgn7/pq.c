#include "pq.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Node for priority queue
typedef struct pqNode {
    char *author;
    double dist;
} pqNode;

//Priority queue
struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    pqNode **queue;
};

//Functions for priority queue nodes (pqN)
pqNode *pqNode_create(char *author, double dist) {
    pqNode *pqN = malloc(sizeof(pqNode));
    pqN->author = strdup(author);
    pqN->dist = dist;
    return pqN;
}

void pqNode_print(pqNode *pqN) {
    printf("Author: %s\n", pqN->author);
    printf("Distance: %lf\n", pqN->dist);
}

void pqNode_delete(pqNode **pqN) {
    free((*pqN)->author);
    free(*pqN);
    *pqN = NULL;
    return;
}

//Helper functions for heap
void swap(pqNode *x, pqNode *y) {
    pqNode tmp = *x;
    *x = *y;
    *y = tmp;
    return;
}

uint32_t min_child(PriorityQueue *q, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && q->queue[right - 1]->dist < q->queue[left - 1]->dist) {
        return right;
    }
    return left;
}

void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t parent = first;
    uint32_t child = min_child(q, parent, last);
    while (parent <= last / 2 && found == false) {
        if (q->queue[parent - 1]->dist > q->queue[child - 1]->dist) {
            swap(q->queue[parent - 1], q->queue[child - 1]);
            parent = child;
            child = min_child(q, parent, last);
        } else {
            found = true;
        }
    }
    return;
}

void build_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    for (uint32_t parent = last / 2; parent > first - 1; parent -= 1) {
        fix_heap(q, parent, last);
    }
}

//Functions for priority queue
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = malloc(sizeof(PriorityQueue));
    q->head = 1;
    q->tail = 1;
    q->capacity = capacity;
    q->queue = (pqNode **) calloc(capacity, sizeof(pqNode));
    return q;
}

void pq_print(PriorityQueue *q) {
    printf("[");
    for (uint32_t i = q->tail - 1; i < q->head - 1; i += 1) {
        printf("(%s", q->queue[i]->author);
        printf(", %lf)", q->queue[i]->dist);
        if ((i + 1) < q->head - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    return;
}

void pq_delete(PriorityQueue **q) {
    for (uint32_t i = 0; i < (*q)->capacity; i += 1) {
        if (((*q)->queue[i]) != NULL) {
            pqNode_delete(&((*q)->queue[i]));
        }
    }
    free((*q)->queue);
    free(*q);
    *q = NULL;
    return;
}

bool pq_empty(PriorityQueue *q) {
    if (q->head == q->tail) {
        return true;
    } else {
        return false;
    }
}

bool pq_full(PriorityQueue *q) {
    //Perhaps change
    if (q->head - 1 == q->capacity) {
        return true;
    } else {
        return false;
    }
}

uint32_t pq_size(PriorityQueue *q) {
    return q->head;
}

bool enqueue(PriorityQueue *q, char *author, double dist) {
    if (pq_full(q)) {
        return false;
    } else {
        pqNode *n = pqNode_create(author, dist);

        //Delete this maybe
        if (q->queue[q->head - 1]) {
            pqNode_delete(&(q->queue[q->head - 1]));
        }

        q->queue[q->head - 1] = n;
        build_heap(q, q->tail, q->head);
        q->head = q->head + 1;
        return true;
    }
}

bool dequeue(PriorityQueue *q, char **author, double *dist) {
    if (pq_empty(q)) {
        return false;
    } else {
        *author = q->queue[q->tail - 1]->author;
        *dist = q->queue[q->tail - 1]->dist;
        swap(q->queue[q->tail - 1], q->queue[q->head - 2]);
        q->head = q->head - 1;
        build_heap(q, q->tail, q->head - 1);
        return true;
    }
    return false;
}
