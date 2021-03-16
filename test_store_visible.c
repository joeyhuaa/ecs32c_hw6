#include "store.h"
#include <stdio.h>
#include <stdlib.h>

#define STORE_FILE_DIR "autograder_store_files/"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You did not pass a test case number as command-line argument to this executable.\n");
        return 1;
    }
    int caseNum = atoi(argv[1]);
    struct Store* store1 = NULL;
    struct Store* store2 = NULL;
    struct Item* cheapest = NULL;
    switch (caseNum)
    {
        case 1:
            store1 = initStore("Safeway", STORE_FILE_DIR"items1.txt");
            store2 = initStore("Safeway", STORE_FILE_DIR"items1.txt");
            printStoreItems(store1);
            printStoreItems(store2);
            cheapest = getCheapestItem(store1);
            printf ("Cheapest item: %s %d %d\n", cheapest->name,
                cheapest->price, cheapest->numInStock);
            break;
        case 2:
            store1 = initStore("Safeway", STORE_FILE_DIR"items2.txt");
            store2 = initStore("Safeway", STORE_FILE_DIR"items1.txt");
            printStoreItems(store1);
            printStoreItems(store2);
            cheapest = getCheapestItem(store1);
            printf("Cheapest item: %s %d %d\n", cheapest->name,
                cheapest->price, cheapest->numInStock);
            break;
        default:
            fprintf(stderr, "Invalid case number.\n");
            return 2;
    }
    return 0;
}
