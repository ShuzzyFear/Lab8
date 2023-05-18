#ifndef FUNCS_H_
#define FUNCS_H_
struct Ht_item {
    std::string key;
    int ip;
    int port;
};

struct LinkedList {
    Ht_item* item;
    LinkedList* next;
};

struct HashTable {

    Ht_item** items;
    LinkedList** overflow_buckets;
    int size;
    int count;
};
HashTable* create_table(int size);
void AddInfo(HashTable* table);
void print_table(HashTable* table);
std::string ht_search(HashTable* table, std::string key);
void ht_insert(HashTable* table, std::string key);
void ht_delete(HashTable* table, std::string key);
#endif

