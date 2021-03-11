#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    unsigned* table; // unsigned -> int
};

// quadratic probing
// 1-3-5-7-9-...

struct Table* htCreate(unsigned tableSize) {
    if (tableSize == 0) return NULL;

    // prime check
    if (tableSize % 2 == 0 && tableSize != 2) return NULL;
    for (int i=3; i<tableSize; i++) {
        if (tableSize % i == 0) return NULL;
    }

    // create ht
    struct HashTable* ht = malloc(sizeof(struct HashTable));
    ht->table = malloc(sizeof(unsigned) * tableSize);
    return ht;
}