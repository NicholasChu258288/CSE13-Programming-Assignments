#include "bv.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    BitVector *bv = malloc(sizeof(BitVector));
    bv->length = length;
    //Should return bv when the vector is initialized
    if ((bv->vector = calloc(length, sizeof(uint8_t))) != NULL) {
        return bv;
    }
    //Should return null when the vector is unable
    //to be initialized
    free(bv);
    return NULL;
}

void bv_print(BitVector *bv) {
    //Prints bytes
    printf("Bytes: [");
    for (uint32_t i = 0; i < bv->length; i += 1) {
        printf("%" PRIu8, bv->vector[i]);
        if (i + 1 < bv->length) {
            printf(", ");
        }
    }
    printf("]\n");

    //Prints starting at lsb. (Ex. 10000000 = 1)
    printf("Bits: [");
    for (uint32_t i = 1; (i - 1) / 8 < bv_length(bv); i += 1) {
        if (bv_get_bit(bv, i - 1)) {
            printf("1");
        } else {
            printf("0");
        }

        if (i % 8 == 0 && i / 8 < bv_length(bv)) {
            printf(" ");
        }
    }
    printf("]\n");
    return;
}

void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i / 8 < bv->length) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    }
    return false;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i / 8 < bv->length) {
        bv->vector[i / 8] &= ~(1 << i % 8);
        return true;
    }
    return false;
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i / 8 < bv->length) {
        if (bv->vector[i / 8] & (0x1 << i % 8)) {
            return true;
        }
    }
    return false;
}
