#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

static void reportIfNull(const struct HashTable* ht)
{
    if (ht)
        printf("Not NULL.\n");
    else
        printf("Is NULL.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You did not pass a test case number as command-line argument to this executable.\n");
        return 1;
    }
    int caseNum = atoi(argv[1]);
    struct HashTable* h1 = NULL, *h2 = NULL;
    unsigned index = 10000;
    int val = -10000;
    switch (caseNum)
    {
        case 1:
            h1 = htCreate(0);
            reportIfNull(h1);
            h2 = htCreate(7);
            h1 = htCreate(23);
            reportIfNull(h2);
            reportIfNull(h1);
            break;
        case 3:
            h1 = htCreate(7);
            printf("%d\n", htInsert(NULL, 16, 400));
            printf("%d\n", htInsert(h1, 16, 400));
            htInsert(h1, 23, 500);
            printf("%d\n", htInsert(h1, 23, 600));
            htPrint(h1);
            break;
        case 4:
            h1 = htCreate(17);
            htInsert(h1, 18, 0);
            htInsert(h1, 0, 42);
            htInsert(h1, 17, 50);
            htPrint(h1);
            break;
        case 5:
            h1 = htCreate(3);
            htInsert(h1, 82, -900);
            htInsert(h1, 0, -50);  // triggers rehash
            htPrint(h1);
            htInsert(h1, 8, 0);
            printf("=====\n");
            htPrint(h1);
            break;
        case 10:
            printf("%d\n", htGetIndex(NULL, 15, &index));
            h1 = htCreate(7);
            htInsert(h1, 44, 500);
            htInsert(h1, 16, 500);
            printf("%d\n", htInsert(h1, 23, 500));
            printf("%d\n", htGetIndex(h1, 16, &index));
            printf("Index: %u\n", index);
            printf("%d\n", htGetIndex(h1, 44, &index));
            printf("Index: %u\n", index);
            printf("%d\n", htGetIndex(h1, 23, &index));
            printf("Index: %u\n", index);
            break;
        case 12:
            h1 = htCreate(19);
            printf("%d\n", htGetTableSize(h1));
            htInsert(h1, 36, 59);
            htInsert(h1, 37, 34);
            htInsert(h1, 55, -18);
            htInsert(h1, 1000, -17);
            htInsert(h1, 20, 17);
            htInsert(h1, 19, 20);
            htInsert(h1, 14, 400);
            htPrint(h1);
            printf("%d\n", htInsert(h1, 55, 599));
            printf("%d\n", htInsert(h1, 74, 673));
            printf("%d\n", htInsert(h1, 207, 473));
            printf("%d\n", htInsert(h1, 77, 73));
            printf("%d\n", htGetTableSize(h1));
            printf("%d\n", htGetIndex(h1, 207, &index));
            printf("Index: %u\n", index);
            printf("%d\n", htGetIndex(h1, 77, &index));
            printf("Index: %u\n", index);
            htPrint(h1);
            break;
        case 18:
            h1 = htCreate(7);
            htInsert(h1, 0, 0);
            htUpdate(h1, 0, 7);
            htPrint(h1);
            htUpdate(h1, 0, 17);
            printf("%d\n", htGetNumElements(h1));
            htGetValue(h1, 0, &val);
            printf("%d\n", val);
            break;
        case 21:
            h1 = htCreate(3);
            htInsert(h1, 19, -50);
            printf("%d\n", htDelete(h1, -50));
            printf("%d\n", htDelete(h1, 19));
            printf("%d\n", htGetNumElements(h1));
            htPrint(h1);
            htInsert(h1, 22, 80);
            printf("%d\n", htGetIndex(h1, 22, &index));
            printf("Index: %u\n", index);
            break;
        case 24:
            h1 = htCreate(2027);
            for (unsigned i = 300; i < 600; ++i)
            {
                htInsert(h1, i * 5, i - 9);
                htUpdate(h1, i * 5, 20);
            }
            htUpdate(h1, 1500, 18);
            printf("%d\n", htGetNumElements(h1));
            printf("%d\n", htGetIndex(h1, 1505, &index));
            printf("Index: %u\n", index);
            printf("%d\n", htGetNumElements(h1));
            htDeleteAllByValue(h1, 20);
            printf("%d\n", htGetIndex(h1, 1505, &index));
            printf("%d\n", htGetNumElements(h1));
            break;
        case 27:
            // Memory leak case; uses case 12.
            h1 = htCreate(19);
            printf("%d\n", htGetTableSize(h1));
            htInsert(h1, 36, 59);
            htInsert(h1, 37, 34);
            htInsert(h1, 55, -18);
            htInsert(h1, 1000, -17);
            htInsert(h1, 20, 17);
            htInsert(h1, 19, 20);
            htInsert(h1, 14, 400);
            htPrint(h1);
            printf("%d\n", htInsert(h1, 55, 599));
            printf("%d\n", htInsert(h1, 74, 673));
            printf("%d\n", htInsert(h1, 207, 473));
            printf("%d\n", htInsert(h1, 77, 73));
            printf("%d\n", htGetTableSize(h1));
            printf("%d\n", htGetIndex(h1, 207, &index));
            printf("Index: %u\n", index);
            printf("%d\n", htGetIndex(h1, 77, &index));
            printf("Index: %u\n", index);
            htPrint(h1);
            htDestroy(h1);
            break;
        default:
            fprintf(stderr, "Invalid case number.\n");
            return 2;
    }
    return 0;
}
