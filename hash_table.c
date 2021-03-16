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

int hash(unsigned key, const struct HashTable* table) {
    return key % table->size;
}

int nextPrime(int x) {
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
    else {
        for (; i<try-1; i++) {
            if (try % i == 0) {
                // printf("%d mod %d = 0\n", try, i);
                break;
            }
        }
        if (i == try-1) {
            // printf("returning next prime = %d\n", try);
            return try;  
        }
        else return nextPrime(try);
    }
}

void rehash(struct HashTable* ht) {
    // set up
    int old_size = ht->size;
    int new_size = nextPrime(ht->size*2);
    struct Element** old_table = malloc(sizeof(struct Element) * old_size);
    for (int i=0; i<old_size; i++) {
        old_table[i] = malloc(sizeof(struct Element));
        old_table[i]->key = ht->table[i]->key;
        old_table[i]->val = ht->table[i]->val;
    }
    unsigned old_assignments[old_size];
    for (int i=0; i<old_size; i++) old_assignments[i] = ht->assignments[i];

    // 1. make a "new" ht
    ht->table = realloc(ht->table, sizeof(struct Element) * new_size);
    for (int i=old_size; i<new_size; i++) ht->table[i] = malloc(sizeof(struct Element));
    ht->assignments = realloc(ht->assignments, sizeof(unsigned) * new_size);
    for (int i=0; i<new_size; i++) ht->assignments[i] = 0; // reset all assignments to 0, since everything is getting rehashed
    ht->size = new_size;

    // 2. rehash all the keys - loop through old_table and rehash the indices in old_table at which old_assignments[i] = 1
    for (int i=0; i<old_size; i++) {
        if (old_assignments[i] == 1) {
            // printf("rehashing [%d]: %d -> %d\n", i, old_table[i]->key, old_table[i]->val);
            htInsert(ht, old_table[i]->key, old_table[i]->val); // rehash
        }
    }

    // 3. dealloc memory
    for (int i=0; i<old_size; i++) free(old_table[i]);
    free(old_table);
}

int arrIncludes(int* arr, int target, int size) {
    // return 0 or 1 depending on if target is in arr
    for (int i=0; i<size; i++) {
        if (arr[i] == target) return 1;
    }
    return 0;
}

int quadProbeIndex(int hashval, int baseIndex, int tableSize) {
    // get the index to probe via quad probing, takes into account wrap arounds
    int index = hashval + baseIndex * baseIndex;
    if (index >= tableSize) {
        // wrap around
        index -= tableSize;
        while (index >= tableSize) {
            index -= tableSize;
        }
        return index;

    } else return index;
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
    for (int i=0; i<ht->size; i++) free(ht->table[i]);
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
        ht->table[hashval]->key = key;
        ht->table[hashval]->val = val;
        ht->assignments[hashval] = 1; // flip 0 -> 1
        return 1;
    // collision -> q probe
    } else {
        unsigned i = 1;
        int probed[100]; // this arr will hold all probed slots, so we can keep track if any of them have alr been visited, at which point probing will stop
        for (int i=0; i<100; i++) probed[i] = -1;
        while (1) {
            // get index hashval + i^2
            // need to figure out how to wrap around 
            int index = quadProbeIndex(hashval, i, ht->size);
            
            // check if probed[] contains index
            if (arrIncludes(probed, index, 100)) return 0;

            // try insert
            if (ht->assignments[index] == 0) {
                ht->table[index]->key = key;
                ht->table[index]->val = val;
                ht->assignments[index] = 1; // flip 0 -> 1
                return 1;
            }

            // increment i, try again
            i++;

            // add index to probed[]
            probed[i] = index;
        }
    }
}

/**
 * Updates the key-value pair with key @key to be mapped to @val.
 *
 * Returns 0 if @ht is a null pointer or if @key is not found.
 * Returns 1 if operation is successful.
 */
unsigned htUpdate(struct HashTable* ht, unsigned key, int val) {
    if (!ht) return 0;
    int hashval = hash(key, ht);
    if (ht->table[hashval]->key == key && ht->assignments[hashval] == 1) {
        ht->table[hashval]->val = val;
        return 1;
    } else return 0;
}

/**
 * Finds the value corresponding to the given key and places it in the
 * variable referenced by @val.
 *
 * Returns 0 (and doesn't change the value referenced by @val) if
 * any of the following is true:
 * 
 * - @ht or @val is a null pointer.
 *
 * - @key is not found in the table.
 *
 * If the operation is successful, then returns 1.
 */
unsigned htGetValue(const struct HashTable* ht, unsigned key, int* val) {
    if (!ht || !val) return 0;
    int hashval = hash(key, ht);
    if (ht->table[hashval]->key == key && ht->assignments[hashval] == 1) {
        *val = ht->table[hashval]->val;
        return 1;
    } else return 0;
}

/**
 * Finds the index of the bucket/slot at which @key is and places this index
 * in the variable referenced by @index.
 * 
 * Returns 0 (and doesn't change the value referenced by @index) if
 * any of the following is true:
 * 
 * - @ht or @index is a null pointer.
 *
 * - @key is not found in the table.
 *
 * If the operation is successful, then returns 1.
 */
unsigned htGetIndex(const struct HashTable* ht, unsigned key, unsigned* index) {
    if (!ht || !index) return 0;
    int hashval = hash(key, ht);
    // printf("getIndex: hashval = %d, key = %d, ht[hashval]->key = %d\n", hashval, key, ht->table[hashval]->key);

    // key was inserted without collision
    if (ht->table[hashval]->key == key && ht->assignments[hashval] == 1) {
        *index = hashval;
        return 1;
    // search the rest of the table
    } else {        
        *index = hashval + 1;
        while (*index != hashval) {
            if (ht->table[*index]->key == key && ht->assignments[*index] == 1) return 1;
            else {
                if (*index == ht->size-1) *index = 0; // wrap around
                else *index+=1;
            }
        }

        // if we reach here, then *index = hashval and we searched entire table w/o finding key
        return 0;
    };
}

/**
 * Prints the key-value pair (or none if there isn't any) at each bucket/slot.
 *
 * Returns 0 if @ht is a null pointer; returns 1 otherwise.
 */
unsigned htPrint(const struct HashTable* ht) {
    if (!ht) return 0;
    for (int i=0; i<ht->size; i++) {
        if (ht->assignments[i] == 1) printf("%d: %d -> %d\n", i, ht->table[i]->key, ht->table[i]->val);
        else printf("%d: (empty)\n", i);
    }
    return 1;
}

/**
 * Deletes the key-value pair that has the given key.
 *
 * Returns 0 if @ht is a null pointer or if @key not found.
 * Returns 1 if operation is successful.
 */
unsigned htDelete(struct HashTable* ht, unsigned key) {
    if (!ht) return 0;
    int hashval = hash(key, ht);
    if (ht->table[hashval]->key == key && ht->assignments[hashval] == 1) {
        ht->assignments[hashval] = 0; // lazy deletion
        return 1;
    } else return 0;
}

/**
 * Deletes all key-value pairs that have the given value.
 *
 * Returns 0 if @ht is a null pointer.
 * Otherwise, returns number of values deleted.
 */
unsigned htDeleteAllByValue(struct HashTable* ht, int val) {
    if (!ht) return 0;
    int count = 0;
    for (int i=0; i<ht->size; i++) {
        if (ht->table[i]->val == val) {
            ht->assignments[i] = 0; // lazy deletion
            count++;
        }
    }
    return count;
}

int main() {
    // things to test
    // 1. infinite probing - when a key cannot be inserted
    // 2. htGetIndex - can it return index of key that was placed via probe and not direct hash?
    // 3. htGetValue - same as (2) but with values
    // 4. htUpdate - same as (2), must be able to find keys that were hashed post collision
}
