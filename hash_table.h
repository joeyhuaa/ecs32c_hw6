#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**
 * Implementation of a hash table that stores key-value pairs mapping unsigned
 * integers to ints.
 * For its hash function, the table does key % tableSize.
 * The table uses quadratic probing for collision resolution.
 * The table does not support non-unique keys.
 *
 * The table rehashes whenever the load factor reaches 1/2. When an attempt
 * is made to insert an element that would cause a rehashing, the rehashing
 * should be done before the element is inserted. Rehashing is not mentioned
 * for the rest of this header file; it's assumed that rehashing is done
 * automatically by any function that would do an insertion.
 * (NOTE: Depending on your instructor for ECS 32B, you might not realize
 * that some define rehashing differently. See my ECS 32B lecture slides on
 * hash tables that I posted to Canvas for how I define rehashing.)
 * Upon a rehash, the table size (let's call it m) should be increased to
 * the lowest prime number that is greater than or equal to 2m.
 * Elements (i.e. key-value pairs) are transferred from the "old table" to the
 * "new/larger table" in the order in which they appear in the old table.
 */
struct HashTable;

/**
 * Creates a hash table with the given number of buckets/slots.
 * Returns a pointer to the created instance, or NULL if any of the following
 * are true:
 * - @tableSize is 0.
 * - @tableSize is not prime.
 */
struct HashTable* htCreate(unsigned tableSize);

/**
 * Deallocates all dynamically allocated memory associated with @ht.
 * Returns 0 if @ht is NULL; returns 1 otherwise.
 */
unsigned htDestroy(struct HashTable* ht);

/**
 * Returns number of buckets/slots currently in the hash table.
 * (This is NOT the number of elements.)
 * Returns 0 if given a null pointer.
 */
unsigned htGetTableSize(const struct HashTable* ht);

/**
 * Returns the number of elements (i.e. key-value pairs) in the hash table.
 * Returns 0 if given a null pointer.
 */
unsigned htGetNumElements(const struct HashTable* ht);

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
unsigned htInsert(struct HashTable* ht, unsigned key, int val);

/**
 * Updates the key-value pair with key @key to be mapped to @val.
 *
 * Returns 0 if @ht is a null pointer or if @key is not found.
 * Returns 1 if operation is successful.
 */
unsigned htUpdate(struct HashTable* ht, unsigned key, int val);

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
unsigned htGetValue(const struct HashTable* ht, unsigned key, int* val);

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
unsigned htGetIndex(const struct HashTable* ht, unsigned key, unsigned* index);

/**
 * Prints the key-value pair (or none if there isn't any) at each bucket/slot.
 *
 * Returns 0 if @ht is a null pointer; returns 1 otherwise.
 */
unsigned htPrint(const struct HashTable* ht);

/**
 * Deletes the key-value pair that has the given key.
 *
 * Returns 0 if @ht is a null pointer or if @key not found.
 * Returns 1 if operation is successful.
 */
unsigned htDelete(struct HashTable* ht, unsigned key);

/**
 * Deletes all key-value pairs that have the given value.
 *
 * Returns 0 if @ht is a null pointer.
 * Otherwise, returns number of values deleted.
 */
unsigned htDeleteAllByValue(struct HashTable* ht, int val);

#endif // HASH_TABLE_H
