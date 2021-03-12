#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct HashTable {
    unsigned* table; // the actual hash table
    unsigned* assignments; // 0 if that slot is empty, 1 if assigned
    unsigned size;
};

int hash(unsigned key, struct HashTable* table) {
    return key % table->size;
}

// hash: key % tableSize
// quadratic probing
// 1^2, 2^2, 3^2, ...

/**
 * unsigned element maps to int index position in array table
 */
struct HashTable* htCreate(unsigned tableSize) {
    if (tableSize == 0) return NULL;

    // prime check
    if (tableSize % 2 == 0 && tableSize != 2) return NULL;
    for (int i=3; i<tableSize; i++) {
        if (tableSize % i == 0) return NULL;
    }

    // create ht
    struct HashTable* ht = malloc(sizeof(struct HashTable));
    ht->table = malloc(sizeof(unsigned) * tableSize);
    ht->assignments = malloc(sizeof(unsigned) * tableSize);
    ht->size = tableSize;
    return ht;
}

/**
 * Deallocates all dynamically allocated memory associated with @ht.
 * Returns 0 if @ht is NULL; returns 1 otherwise.
 */
unsigned htDestroy(struct HashTable* ht) {
    if (!ht) return 0;
    free(ht->table);
    free(ht->assignments);
    free(ht);
    return 1;
}

/**
 * Returns number of buckets/slots currently in the hash table.
 * (This is NOT the number of elements.)
 * Returns 0 if given a null pointer.
 */
unsigned htGetTableSize(const struct HashTable* ht) {
    if (!ht) return 0;
    return ht->size;
}

/**
 * Returns the number of elements (i.e. key-value pairs) in the hash table.
 * Returns 0 if given a null pointer.
 */
unsigned htGetNumElements(const struct HashTable* ht) {
    if (!ht) return 0;

    unsigned num = 0;
    for (int i=0; i<ht->size; i++) {
        if (ht->assignments[i] == 1) num++;
    }
    return num;
}

/**
 * Inserts the key-value pair mapping @key to @val into the hash table.
 *
 * Returns 0 if:
 *
 * - @ht is a null pointer.
 *
 * - A key-value pair with key @key already exists in the table.
 *
 * Returns 1 if operation is successful.
 */
unsigned htInsert(struct HashTable* ht, unsigned key, int val) {
    // store a dict key->val at index hash(key)
    
    if (!ht) return 0;

    // check if @key already exists in table
    for (int i=0; i<ht->size; i++) {
        if (ht->table[i] == hash(key, ht) && ht->assignments[i] == 1) return 0;
    }

    // insert
    // WHY PASS VAL AS A PARAMETER WHEN HASH(KEY) WILL GIVE YOU VAL????
    unsigned i = 0;
    while (1) {
        // if table[val] is assigned, i++ and try again with val + i^2
    }
}

int main() {
    struct HashTable* t1 = htCreate(10);
    struct HashTable* t2 = htCreate(17);
    assert(t1 == NULL);
    assert(t2->size == 17);
    for (int i=0; i<t2->size; i++) {
        printf("%d: %d\n", i, t2->assignments[i]);
    }
}
