#ifndef STORE_H
#define STORE_H
#define MAX_LINE_LEN 1024

struct Item
{
    char* name;
    int price;
    int numInStock;
};

struct Store
{
    char* storeName;
    struct Item* items;
    int numItems;
};

struct Store* initStore(const char* storeName, const char* fileName);
struct Item* getCheapestItem(struct Store* store);
void deallocateStore(struct Store* store);
void printStoreItems(const struct Store* store);

#endif // STORE_H
