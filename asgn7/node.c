#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

Node *node_create(char *word) {
    Node *n = malloc(sizeof(Node));
    n->word = strdup(word);
    n->count = 1;
    return n;
}

void node_delete(Node **n) {
    free((*n)->word);
    free(*n);
    *n = NULL;
    return;
}

void node_print(Node *n) {
    printf("Word: %s\n", n->word);
    printf("Count: %" PRIu32 "\n", n->count);
    return;
}
