// ConsoleApplication_Hash.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "HashTable.h"

using namespace std;

// Хеш-функция, возвращающая значения от 0 до 10
unsigned long HashF10(int key)
{
    return key % 10; // метод деления на 10 (возвращается остаток от деления)
}
// todo другая хеш-функция из презентации

int main()
{
    setlocale(LC_ALL, "rus");
    HashTable<int> h1(10, HashF10); // 10 блоков

    h1.Insert(2);
    h1.Insert(12);
    h1.Insert(3);
    cout << "ok";
    int f = h1.Find(2);
   // cout << "\n" << f;
    f = h1.Find(12);
    //cout << "\n" << f;
    f = h1.Find(3);
    //cout << "\n" << f << "\n";

    h1.PrintTable();

    h1.Delete(2);
    f = h1.Find(12);
    cout << "\n" << f;

    h1.PrintTable();

    h1.Insert(12);
    h1.PrintTable();


    HashTableIterator<int> it1(&h1);

    
    /*for (auto it1 : h1) {
        cout << it1;
    }*/
}

