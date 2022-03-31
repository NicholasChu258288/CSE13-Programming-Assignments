#include "ht.h"
#include "salts.h"
#include "speck.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **slots;
};

struct HashTableIterator {
    HashTable *table;
    uint32_t slot;
};

//HashTable functions
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    ht->size = size;
    ht->slots = (Node **) calloc(size, sizeof(Node));
    ht->salt[0] = SALT_HASHTABLE_LO;
    ht->salt[1] = SALT_HASHTABLE_HI;
    return ht;
}

void ht_print(HashTable *ht) {
    printf("Printing hash table:\n");
    printf("Size: %" PRIu32 "\n", ht->size);
    printf("[");
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->slots[i] != NULL) {
            printf("(%s:", ht->slots[i]->word);
            printf("%" PRIu32 ")", ht->slots[i]->count);
        }
    }
    printf("]\n");
    return;
}

void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < ht_size(*ht); i += 1) {
        if ((*ht)->slots[i] != NULL) {
            node_delete(&((*ht)->slots[i]));
        }
    }
    free((*ht)->slots);
    free(*ht);
    *ht = NULL;
    return;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *word) {
    int index = hash(ht->salt, word) % ht_size(ht);
    Node *n = ht->slots[index];
    if (n == NULL) {
        return NULL;
    }
    return n;
}

Node *ht_insert(HashTable *ht, char *word) {
    //First checks if word is already in hash table
    Node *n = ht_lookup(ht, word);
    //If already existing increment count
    if (n != NULL) {
        n->count = n->count + 1;
        return n;
    }
    //If word is not already a node in the hash table
    //Find next availible spot and create and new
    //node and insert it into the hash table
    uint32_t count = 0;
    uint32_t index = hash(ht->salt, word) % ht_size(ht);
    while (count < ht_size(ht)) {
        if (ht->slots[index] == NULL) {
            n = node_create(word);
            ht->slots[index] = n;
            return n;
        }
        count += 1;
        index = (index + 1) % ht_size(ht);
    }
    return NULL;
}

//HashTableIterator functions
HashTableIterator *hti_create(HashTable *ht) {
    HashTableIterator *hti = (HashTableIterator *) malloc(sizeof(HashTableIterator));
    hti->table = ht;
    hti->slot = 0;
    return hti;
}

void hti_delete(HashTableIterator **hti) {
    //Only deletes iterator itself, not the hash table
    //Deleting the hash table must be done seperately
    free(*hti);
    *hti = NULL;
    return;
}

Node *ht_iter(HashTableIterator *hti) {
    Node *n = NULL;
    while (hti->slot < ht_size(hti->table)) {
        n = hti->table->slots[hti->slot];
        //Incrimenting
        hti->slot = hti->slot + 1;
        if (n != NULL) {
            return n;
        }
    }
    return NULL;
}
