#include "node.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) (malloc(sizeof(Node)));
    n->left = NULL;
    n->right = NULL;
    n->symbol = symbol;
    n->frequency = frequency;
    return n;
}

void node_delete(Node **n) {
    if (*n) {
        if ((*n)->left) {
            node_delete((&(*n)->left));
        }
        if ((*n)->right) {
            node_delete(&((*n)->right));
        }
        free(*n);
        *n = NULL;
    }
}

Node *node_join(Node *left, Node *right) {
    //Get combined frequenct of left and right node
    uint64_t frequency = left->frequency + right->frequency;
    //Creates a new node using $ for symbol, combined frequency of the
    //left and right node for freqeuency, and sets left and write of
    //the new node to the ones mentioned in the parameters
    Node *n = node_create('$', frequency);
    n->left = left;
    n->right = right;
    return n;
}

void node_print(Node *n) {
    printf("Node: %c\n", n->symbol);
    printf("Frequency: %" PRIu64 "\n", n->frequency);
    if (n->left) {
        printf("Left node symbol: %c\n", n->left->symbol);
    }
    if (n->right) {
        printf("Right node symbol: %c\n", n->right->symbol);
    }
}
