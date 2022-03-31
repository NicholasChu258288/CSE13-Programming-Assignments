#include "text.h"
#include "ht.h"
#include "bf.h"
#include "salts.h"
#include "speck.h"
#include "parser.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <regex.h>
#include <ctype.h>

#define WORD "[A-Za-z](['-]?[A-Za-z])*" // Regular expression  for scanning words from files.

uint32_t noiselimit = 100; // Number of noise words to filter out, default is 100.

struct Text {
    HashTable *ht;
    BloomFilter *bf;
    uint32_t word_count;
};

Text *text_create(FILE *infile, Text *noise) {
    // Creates Text
    Text *t = (Text *) malloc(sizeof(Text));
    t->ht = ht_create(pow(2, 19)); // Size of 2^19
    //Checks if hash table is properly created
    if (t->ht == NULL) {
        printf("Failed to create text\n");
        free(t->ht);
        free(t);
        return NULL;
    }
    t->bf = bf_create(pow(2, 21)); // Size of 2^21
    //Checks if bloom filter was properly created
    if (t->bf == NULL) {
        printf("Failed to create text\n");
        ht_delete(&(t->ht));
        free(t->bf);
        free(t);
        return NULL;
    }
    t->word_count = 0;

    //Using infile to fill up hash tables and bloom filters
    regex_t re;
    //Compiles regular expression
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        return NULL;
    }

    char *word = NULL;
    uint32_t i = 0; //Just used to count number of words scanned in from noise
    char *w;

    while ((word = next_word(infile, &re)) != NULL && i < noiselimit) {
        w = strdup(word);

        //Changes parseed words into lowercase
        int wi = 0;
        while (w[wi]) {
            w[wi] = tolower(w[wi]);
            wi += 1;
        }

        //When text being created is the noise text
        if (noise == NULL) {
            bf_insert(t->bf, w);
            ht_insert(t->ht, w);
            t->word_count = t->word_count + 1;
            i += 1;
        }
        //When there is existing noise text
        if (noise != NULL) {
            if (!(bf_probe(noise->bf, w))) {
                bf_insert(t->bf, w);
                ht_insert(t->ht, w);
                t->word_count = t->word_count + 1;
            }
        }
        free(w);
    }
    regfree(&re);
    return t;
}

void text_delete(Text **text) {
    bf_delete(&((*text)->bf));
    ht_delete(&((*text)->ht));
    free(*text);
    *text = NULL;
    return;
}

double text_dist(Text *text1, Text *text2, Metric metric) {
    //Fill in later
    HashTableIterator *hti = hti_create(text1->ht);
    HashTableIterator *hti1 = hti_create(text2->ht);

    Node *n = NULL;
    double dist_sum = 0; // Dist between the two texts
    double dist1 = 0; // Dist for text1
    double dist2 = 0; // Dist for text2

    //Goes through unique words from text
    while ((n = ht_iter(hti))) {
        dist1 = text_frequency(text1, n->word);
        dist2 = text_frequency(text2, n->word);
        //Will do seperate things depending on metric
        if (metric == EUCLIDEAN) {
            dist_sum += pow((dist1 - dist2), 2);
        }
        if (metric == MANHATTAN) {
            dist_sum += fabs(dist1 - dist2);
        }
        if (metric == COSINE) {
            dist_sum += dist1 * dist2;
        }
    }

    //Goes through unique words from text2
    //that were not in text1
    while ((n = ht_iter(hti1))) {
        //If word was not in first text file
        if (ht_lookup(text1->ht, n->word) == NULL) {
            dist1 = text_frequency(text1, n->word); //Can basically just assume is 0
            dist2 = text_frequency(text2, n->word);
            if (metric == EUCLIDEAN) {
                dist_sum += pow((dist1 - dist2), 2);
            }
            if (metric == MANHATTAN) {
                dist_sum += fabs(dist1 - dist2);
            }
            if (metric == COSINE) {
                dist_sum += dist1 * dist2;
            }
        }
    }

    //Do final things to dist_sum depending on metric
    if (metric == EUCLIDEAN) {
        dist_sum = sqrt(dist_sum);
    }
    if (metric == MANHATTAN) {
        dist_sum = fabs(dist_sum);
    }
    if (metric == COSINE) {
        dist_sum = 1 - dist_sum;
    }

    hti_delete(&hti);
    hti_delete(&hti1);
    return dist_sum;
}

double text_frequency(Text *text, char *word) {
    //First checks if word is in bloom filter
    if (bf_probe(text->bf, word)) {
        //If word was in bloom filter then get the count
        Node *n = ht_lookup(text->ht, word);
        if (n != NULL) {
            return (double) n->count / (double) text->word_count;
        }
    }
    return (double) 0;
}

bool text_contains(Text *text, char *word) {
    //First checks if the word is in bloom filter
    if (bf_probe(text->bf, word)) {
        //If word is in bloom filter then check if
        //the word was in the hash table
        if (ht_lookup(text->ht, word)) {
            return true;
        }
    }
    return false;
}

void text_print(Text *text) {
    printf("Word count: %d\n", text->word_count);
    ht_print(text->ht);
}
