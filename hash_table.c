#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Element { // this will get stored in hash table, key->val
    unsigned key;
    int val;
};

struct HashTable {
    struct Element** table; // the actual hash table
    unsigned* assignments; // 0 if that slot is empty, 1 if assigned
    unsigned size;
};

int hash(unsigned key, struct HashTable* table) {
    return key % table->size;
}

int nextPrime(x) {
    // return lowest prime > x
    // algo, naive, poly time

    // try = x + 1
    // check all nums from 2 => try - 1, see if any are divisible
    // if so, recurse with try+1
    // else return try

    // check
    int try = x + 1;
    int i = 2;
    if (try == 2) return try;
    for (; i<try-1; i++) {
        if (try % i == 0) {
            // printf("%d mod %d = 0\n", try, i);
            break;
        }
    }
    
    if (i == try-1) return try; 
    else return nextPrime(try);
}

struct HashTable* rehash(struct HashTable* ht) {
    // 1. make new table to size nextPrime(currsize)
    struct HashTable* new_ht = htCreate(nextPrime(ht->size));
    // printf("next prime = %d\n", nextPrime(ht->size));

    // 2. make new assignments to size nextPrime(currsize), and reset all to 0

    // 3. htdestroy ht

    return new_ht;
}







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
    ht->table = malloc(sizeof(struct Element) * tableSize);
    for (int i=0; i<tableSize; i++) ht->table[i] = malloc(sizeof(struct Element));
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

    // now have to free each slot in table bc its pointer to struct Element
    for (int i=0; i<ht->size; i++) {
        free(ht->table[i]);
    }
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
    if (!ht) return 0;

    // check for rehash here, before insert
    // if (numElems + 1) / tablesize >= 0.5, rehash (create a new table and hash everything again)
    float loadfactor = (float) (htGetNumElements(ht) + 1) / ht->size;
    if (loadfactor >= 0.5) {
        rehash(ht);
    }

    // get the hashval
    int hashval = hash(key, ht);

    // check if @key already exists in table somewhere
    for (int i=0; i<ht->size; i++) {
        // seg fault here
        if (ht->assignments[i] == 1) {
            if (ht->table[i]->key == key) return 0;
        }
    }

    // try 1st insert
    if (ht->assignments[hashval] == 0) { 
        ht->table[hashval] = malloc(sizeof(struct Element));
        ht->table[hashval]->key = key;
        ht->table[hashval]->val = val;
        ht->assignments[hashval] = 1; // flip 0 -> 1
        return 1;
    // if fails, then q probe
    } else {
        // try index hashval + i^2
        // plug it on the first 0
        // how to handle loops?
        unsigned i = 1;
        while (1) {
            // get the index to try
            int index = hashval + i * i;
            // try insert
            if (ht->assignments[index] == 0) {
                ht->table[index]->key = key;
                ht->table[index]->val = val;
                ht->assignments[index] = 1; // flip 0 -> 1
                return 1;
            }
            // increment i, try again
            i++;

            // figure out how to handle loops?
            // rehashing?
        }
    }
}

int main() {
    struct HashTable* t2 = htCreate(11);
    htInsert(t2, 25, 200);
    htInsert(t2, 28, 99);
    htInsert(t2, 34, 999);
    htInsert(t2, 17, 17);
    htInsert(t2, 0, 17000);
    htInsert(t2, 51, 17123);
    // htInsert(t2, 50, 17123);
    for (int i=0; i<t2->size; i++) {
        printf("%d: %d -> %d\n", i, t2->table[i]->key, t2->table[i]->val);
    }
    printf("numElems: %d\n", htGetNumElements(t2));

    // test rehash
}
