#include "bf.h"
#include "salts.h"
#include "speck.h"
#include "bv.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    bf->filter = bv_create(size);
    return bf;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
    return;
}

void bf_delete(BloomFilter **bf) {
    bv_delete(&((*bf)->filter));
    free(*bf);
    *bf = NULL;
    return;
}

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *word) {
    //Gets indecies of word
    uint32_t primary_index = hash(bf->primary, word) % bf_size(bf);
    uint32_t secondary_index = hash(bf->secondary, word) % bf_size(bf);
    uint32_t tertiary_index = hash(bf->tertiary, word) % bf_size(bf);
    //Sets bits in bit vector (filter)
    bv_set_bit(bf->filter, primary_index);
    bv_set_bit(bf->filter, secondary_index);
    bv_set_bit(bf->filter, tertiary_index);
    return;
}

bool bf_probe(BloomFilter *bf, char *word) {
    //Get indecies of word
    uint32_t p = hash(bf->primary, word) % bf_size(bf);
    uint32_t s = hash(bf->secondary, word) % bf_size(bf);
    uint32_t t = hash(bf->tertiary, word) % bf_size(bf);
    //Confirms that bits at index are set
    if (bv_get_bit(bf->filter, p) && bv_get_bit(bf->filter, s) && bv_get_bit(bf->filter, t)) {
        //If index at all positions are correct
        return true;
    }
    return false;
}
