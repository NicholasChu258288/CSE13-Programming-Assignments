#include "stack.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) (malloc(sizeof(Stack)));
    s->top = 0;
    s->capacity = capacity;
    s->items = (Node **) calloc(capacity, sizeof(Node));
    return s;
}

void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
}

bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    //Pushes node to stack when there is still
    //space in the stack
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top = s->top + 1;
    return true;
}

bool stack_pop(Stack *s, Node **n) {
    //Pops node from stack when stack is not
    //empty
    if (stack_empty(s)) {
        return false;
    }
    s->top = s->top - 1;
    **n = *(s->items[s->top]);
    return true;
}

void stack_print(Stack *s) {
    printf("Stack: [");
    for (uint32_t i = 0; i < s->top; i += 1) {
        printf("(%c", s->items[i]->symbol);
        printf(", %" PRIu64 ")", s->items[i]->frequency);
        if ((i + 1) < s->top) {
            printf(", ");
        }
    }
    printf("]\n");
}
