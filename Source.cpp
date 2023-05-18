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


int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));
    HashTable* ht = create_table(CAPACITY);
    char ch;
    std::ifstream in("names.txt", std::ios::binary);
    while (true)
    {
        int choose;
        printf("Выберите действие:\n1. Ввести данные из базы данных\n2. Вывести данные из таблицы\n3. Найти данные\n4. Добавить свою строку\n 5. Удалить строку\n6. Выход\n");
        std::cin >> choose;
        if (choose == 1)
        {
            AddInfo(ht);
        }
        if (choose == 2)
        {
            print_table(ht);
        }
        if (choose == 3)
        {
            std::string searchkey;
            std::cout << "Введите искомое имя:\n";
            std::cin >> searchkey;
            ht_search(ht, searchkey);
        }
        if (choose == 4)
        {
            std::string searchkey;
            std::cout << "Введите имя:\n";
            std::cin >> searchkey;
            ht_insert(ht, searchkey);
        }
        if (choose == 5)
        {
            std::string searchkey;
            std::cout << "Введите имя для удаления:\n";
            std::cin >> searchkey;
            ht_delete(ht, searchkey);
        }
        if (choose == 6)
        {
            return 0;
        }
    }
}