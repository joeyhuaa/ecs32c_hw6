#include "store.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Store* initStore(const char* storeName, const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (!file) return NULL;

    // read the first line first to get n, then create the store
    char line[MAX_LINE_LEN];
    fgets(line, MAX_LINE_LEN, file);
    int n = atoi(line);
    struct Store* s = malloc(sizeof(struct Store));
    s->storeName = malloc(sizeof(char) * strlen(storeName));
    strcpy(s->storeName, storeName);
    s->items = malloc(sizeof(struct Item) * n);
    s->numItems = n;

    // n - number of items
    // item1 price num_in_stock
    // item2 price num_in_stock
    for (int i=0; i<n; i++) {
        fgets(line, MAX_LINE_LEN, file);
        char* name = malloc(sizeof(char) * strlen(name)); 
        strcpy(name, strtok(line, " "));
        char* price = strtok(NULL, " ");
        char* numInStock = strtok(NULL, " ");

        struct Item item = {name, atoi(price), atoi(numInStock)};   
        s->items[i] = item;
    }

    fclose(file);
    return s;
}

struct Item* getCheapestItem(struct Store* store)
{
    // iterate through items, use conditional
    struct Item* cheapest = malloc(sizeof(struct Item));
    for (int i=0; i<store->numItems; i++) {
        if (i == 0) {
            *cheapest = store->items[i];
        } else {
            if (store->items[i].price < cheapest->price) *cheapest = store->items[i];
        }
    }
    return cheapest;
}

void deallocateStore(struct Store* store)
{
    free(store->storeName);
    for (int i=0; i<store->numItems; i++) free(store->items[i].name);
    free(store->items);
    free(store);
}

void printStoreItems(const struct Store* store)
{
    printf("%s has the following items:\n", store->storeName);
    for (int i=0; i<store->numItems; i++) {
        struct Item item = store->items[i];
        printf("Item #%d: %s, Price: %d, Number in stock: %d\n", i+1, item.name, item.price, item.numInStock);
    }
}

int main() {
    // 1-1: this prints correctly?? why is it just '\n' when submitted to autograder?
    struct Store* store1 = NULL;
    struct Store* store2 = NULL;
    store1 = initStore("Safeway", "items.txt");            
    store2 = initStore("Safeway", "items2.txt");            
    printStoreItems(store1);            
    printStoreItems(store2);            
    struct Item* cheapest = getCheapestItem(store1);            
    printf ("Cheapest item: %s %d %d\n", cheapest->name, cheapest->price, cheapest->numInStock);
    deallocateStore(store1);
    deallocateStore(store2);
    free(cheapest);
}
