#include "huffman.h"
#include "pq.h"
#include "code.h"
#include "stack.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

#include <inttypes.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
    Node *n;
    PriorityQueue *q = pq_create(ALPHABET);
    //Reads histogram and creates nodes
    //for sybmols with more than 0 frequency
    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            n = node_create(i, hist[i]);
            enqueue(q, n);
        }
    }
    Node *left;
    Node *right;
    //Joins node until there is only one node left
    pq_print(q);

    while (pq_size(q) > 2) {
        dequeue(q, &left);
        dequeue(q, &right);
        n = node_join(left, right);
        enqueue(q, n);
    }
    dequeue(q, &n);
    return n;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    uint8_t *bit = 0;
    if (root) {
        if (!(root->left) && !(root->right)) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, bit);
        }
    }
}

void dump_tree(int outfile, Node *root) {
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (!(root->left) && !(root->right)) {
            write(outfile, "L", 1);
            write(outfile, &(root->symbol), 1);
        } else {
            write(outfile, "I", 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    uint8_t symbol;
    Node *n;
    Node *left = node_create('$', 0); //Place holder node when dequeuing
    Stack *s = stack_create(nbytes);
    for (int byte = 0; byte < nbytes; byte += 1) {
        if (tree_dump[byte] == 'L') {
            symbol = tree_dump[byte + 1];
            n = node_create(symbol, 0);
            stack_push(s, n);
        }
        if (tree_dump[byte] == 'I') {
            stack_pop(s, &n);
            stack_pop(s, &left);
            n = node_join(left, n);
            stack_push(s, n);
        }
    }
    stack_pop(s, &n);
    stack_delete(&s);
    return n;
}

void delete_tree(Node **root) {
    if (*root) {
        if ((*root)->left) {
            delete_tree(&(*root)->left);
        }
        if ((*root)->right) {
            delete_tree(&(*root)->right);
        }
        free(*root);
        *root = NULL;
    }
}
