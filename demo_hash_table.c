#include "hash_table.h"

#include <stdio.h>

int main()
{
    struct HashTable* h1 = htCreate(7);
    htInsert(h1, 16, 400);
    htInsert(h1, 23, 500);
    int val = -1;
    printf("%u\n", htGetValue(h1, 16, &val));  // 1
    printf("%d\n", val);  // 400
    htGetValue(h1, 23, &val);
    printf("%d\n", val);  // 500
    unsigned index = 1000;
    printf("%u\n", htGetIndex(h1, 16, &index));  // 1
    printf("%u\n", index);  // 2
    htGetIndex(h1, 23, &index);
    printf("%u\n", index);  // 3
    htUpdate(h1, 23, 600);
    printf("%u\n", htGetNumElements(h1));  // 2
    printf("%u\n", htGetTableSize(h1));  // 7
    htInsert(h1, 40, 400);
    htPrint(h1);
    htDeleteAllByValue(h1, 400);
    printf("---\n");
    htPrint(h1);
    htDestroy(h1);
    h1 = NULL;
    printf("==========\n");
    struct HashTable* h2 = htCreate(3);
    htInsert(h2, 31, 1000);
    htPrint(h2);
    htInsert(h2, 2, 2000);  // triggers rehash
    printf("size: %u\n", htGetTableSize(h2));  // 7
    htPrint(h2);
}
