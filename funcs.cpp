#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctime>
#include <windows.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <queue>
#include <sstream>
#include <string>
#include "funcs.h"

#define CAPACITY 50000 // Size of the Hash Table
unsigned long hash_function(std::string str) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

LinkedList* allocate_list() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    return list;
}

LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    if (!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }

    else if (list->next == NULL) {
        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList* temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }

    LinkedList* node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;

    return list;
}

Ht_item* linkedlist_remove(LinkedList* list) {
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedList* node = list->next;
    LinkedList* temp = list;
    temp->next = NULL;
    list = node;
    Ht_item* it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item);
    free(temp);
    return it;
}

void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item);
        free(temp);
    }
}

LinkedList** create_overflow_buckets(HashTable* table) {

    LinkedList** buckets = (LinkedList**)calloc(table->size, sizeof(LinkedList*));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

void free_overflow_buckets(HashTable* table) {
    LinkedList** buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}


Ht_item* create_item(std::string key1) {
    // Creates a pointer to a new hash table item
    Ht_item* item = new Ht_item[sizeof(Ht_item)];
    item->ip = (rand() * 10) % CAPACITY;
    item->port = rand();
    item->key = key1;
    return item;
}

HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)calloc(table->size, sizeof(Ht_item*));
    for (int i = 0; i < size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item* item) {
    free(item);
}

void free_table(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item) {
    LinkedList* head = table->overflow_buckets[index];

    if (head == NULL) {
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else {
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable* table, std::string key) {
    Ht_item* item = create_item(key);
    unsigned long index = hash_function(key);

    Ht_item* current_item = table->items[index];

    if (current_item == NULL) {
        if (table->count == table->size) {
            printf("Insert Error: Hash Table is full\n");
            free_item(item);
            return;
        }
        table->items[index] = item;
        table->count++;
    }

    else {
        if (current_item->key == key) {
            table->items[index]->ip = (rand() * 10) % CAPACITY;
            table->items[index]->port = rand();
            return;
        }

        else {
            handle_collision(table, index, item);
            return;
        }
    }
}

std::string ht_search(HashTable* table, std::string key) {
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];
    while (item != NULL) {
        if (item->key == key)
        {
            printf("IP: %d  port: %d\n\n", table->items[index]->ip, table->items[index]->port);
            return key;
        }
        if (head == NULL)
        {
            printf("%s не сущестует\n", key);
            return key;
        }

        item = head->item;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable* table, std::string key) {
    // Deletes an item from the table
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    if (item == NULL) {
        // Does not exist. Return
        return;
    }
    else {
        if (head == NULL && item->key == key) {
            // No collision chain. Remove the item
            // and set table index to NULL
            table->items[index] = NULL;
            //free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL) {
            // Collision Chain exists
            if (item->key == key) {
                // Remove this item and set the head of the list
                // as the new item

               // free_item(item);
                LinkedList* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key);
                //free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList* curr = head;
            LinkedList* prev = NULL;

            while (curr) {
                if (curr->item->key == key) {
                    if (prev == NULL) {
                        // First element of the chain. Remove the chain
                        //free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        //free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }

        }
    }
}

/*void print_search(HashTable* table, char* key) {
    char* val;
    if ((val = ht_search(table, key)) == NULL) {
        printf("%s не сущестует\n", key);
        return;
    }
    else {
        printf("Key:%s, Value:%s\n", key, val);
    }
}
*/
void print_table(HashTable* table) {
    printf("\n-------------------\n");
    for (int i = 0; i < table->size; i++) {
        if (table->items[i]) {
            //printf("Индекс:%d, Имя:%s, IP:%d, port:%d", i, table->items[i]->key, table->items[i]->ip, table->items[i]->port);
            std::cout << "Индекс:" << i << "Имя: " << table->items[i]->key << " IP: " << table->items[i]->ip << " port: " << table->items[i]->port << std::endl;
            if (table->overflow_buckets[i]) {
                printf(" => Overflow Bucket => ");
                LinkedList* head = table->overflow_buckets[i];
                while (head) {
                    printf("Имя:%s, IP:%d, port:%d", head->item->key, head->item->ip, head->item->port);
                    head = head->next;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------\n");
}

void AddInfo(HashTable* table)
{
    std::ifstream file("names.txt"); // создаем объект класса ifstream
    //file.open("names.txt");
    int number, position;
    char* temp;
    std::string key;
    temp = new char[50];
    printf("Сколько элементов?\n");
    std::cin >> number;
    printf("Начиная с:\n");
    std::cin >> position;
    for (int i = 0; i < position - 1; i++)
    {
        std::getline(file, key);
    }

    for (int i = 0; i < number; i++)
    {
        //file.get(key, 50, '\n');
        std::getline(file, key);
        ht_insert(table, key);
    }

    file.close();
}