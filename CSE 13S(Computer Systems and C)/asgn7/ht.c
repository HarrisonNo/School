#include "ht.h"
#include "salts.h"
#include "speck.h"
#include <inttypes.h>
#include <stdio.h> //needed for print
#include <stdlib.h> //needed for NULL

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

HashTable *ht_create(uint32_t size) {
    HashTable *HT = (HashTable *) malloc(1 * sizeof(HashTable));
    HT->size = size;
    HT->salt[0] = SALT_HASHTABLE_LO;
    HT->salt[1] = SALT_HASHTABLE_HI;
    HT->trees = malloc(size * sizeof(Node *));
    for (uint32_t i = 0; i < size; i++) {
        HT->trees[i] = bst_create(); //simply points each pointer to NULL
    }
    return HT;
}

void ht_delete(HashTable **ht) {
    if (ht == NULL) {
        return;
    }
    for (uint32_t i = 0; i < (*ht)->size; i++) {
        if ((*ht)->trees[i] != NULL) {
            bst_delete(&((*ht)->trees[i]));
            (*ht)->trees[i] = NULL;
        }
    }
    free((*ht)->trees);
    (*ht)->trees = NULL;
    free(*ht);
    *ht = NULL;
    ht = NULL;
    return;
}

uint32_t ht_size(HashTable *ht) {
    if (ht == NULL) {
        return 0;
    }
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups++;
    uint32_t HashValue = hash(&(ht->salt[0]), oldspeak) % ht->size;
    return bst_find(ht->trees[HashValue], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups++;
    if (oldspeak == NULL) {
        return;
    }
    uint32_t HashValue = hash(&(ht->salt[0]), oldspeak) % ht->size;
    Node *N = bst_insert(ht->trees[HashValue], oldspeak, newspeak);
    if (ht->trees[HashValue] == NULL) //set new node to front if NULL
    {
        ht->trees[HashValue] = N;
    }
    return;
}

uint32_t ht_count(HashTable *ht) {
    uint32_t Count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            Count++;
        }
    }
    return Count;
}

double ht_avg_bst_size(HashTable *ht) {
    double TotalSize = 0, AverageSize = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        TotalSize += bst_size(ht->trees[i]);
    }
    AverageSize = (TotalSize / (ht_count(ht)));
    return AverageSize;
}

double ht_avg_bst_height(HashTable *ht) {
    double TotalHeight = 0, AverageHeight = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        TotalHeight += bst_height(ht->trees[i]);
    }
    AverageHeight = (TotalHeight / (ht_count(ht)));
    return AverageHeight;
}

void ht_print(HashTable *ht) {
    printf("Salt low: %" PRIu64 "\n", ht->salt[0]);
    printf("Salt high: %" PRIu64 "\n", ht->salt[1]);
    printf("Hashtable size:%" PRIu32 "\n", ht->size);
    for (uint32_t i = 0; i < ht->size; i++) {
        bst_print(ht->trees[i]);
    }
    return;
}
