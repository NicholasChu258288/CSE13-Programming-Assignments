#include "code.h"
#include "defines.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

Code code_init(void) {
    Code c;
    c.top = 0;
    memset(c.bits, 0, sizeof(c.bits));
    return c;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    } else {
        return false;
    }
}

bool code_full(Code *c) {
    if (c->top == ALPHABET) {
        return true;
    } else {
        return false;
    }
}

bool code_set_bit(Code *c, uint32_t i) {
    //Set bit in c's items at index i to 1
    if (i < ALPHABET) {
        c->bits[i / 8] |= (0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}

bool code_clr_bit(Code *c, uint32_t i) {
    //Set bit in c's items at index i to 0
    if (i < ALPHABET) {
        c->bits[i / 8] &= ~(0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}

bool code_get_bit(Code *c, uint32_t i) {
    //Gets bit from index of from items and returns true or false
    //when 1 or 0 respectively
    if (i <= c->top) {
        if (c->bits[i / 8] & (0x1 << i % 8)) {
            return true;
        }
        return false;
    }
    return false;
}

bool code_push_bit(Code *c, uint8_t i) {
    //Pushes bit i to c's items
    if (!(code_full(c)) && i == 0) {
        if (code_clr_bit(c, c->top)) {
            c->top += 1;
            return true;
        }
    }
    if (!(code_full(c)) && i == 1) {
        if (code_set_bit(c, c->top)) {
            c->top += 1;
            return true;
        }
    }
    return false;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    //Pops bit i from c's items
    uint32_t b;
    if (!code_empty(c)) {
        if (code_get_bit(c, c->top - 1)) {
            b = 1;
            *bit = b;
            c->top -= 1;
            return true;
        } else {
            b = 0;
            *bit = b;
            c->top -= 1;
            return true;
        }
    }
    return false;
}

void code_print(Code *c) {
    for (uint8_t i = 0; i < c->top; i += 1) {
        if (code_get_bit(c, i)) {
            printf("%d", 1);
        } else {
            printf("%d", 0);
        }
    }
    printf("\n");
}
