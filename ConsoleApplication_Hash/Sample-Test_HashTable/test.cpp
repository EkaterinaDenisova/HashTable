#include "pch.h"
#include "../ConsoleApplication_Hash/HashTable.h"

unsigned long HashF10(int key)
{
	return key % 10; // метод деления на 10 (возвращается остаток от деления)
}

unsigned long HashFstr(unsigned char* str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}


// проверка вставки и поиска
TEST(TestHashTable, TestInsertFind) {

	HashTable<int> h1(10, HashF10); // 10 блоков

	h1.Insert(2);
	h1.Insert(12);
	h1.Insert(3);
	h1.Insert(33);
	h1.Insert(24);
	h1.Insert(85);

	EXPECT_EQ(h1.Find(2), 0);
	EXPECT_EQ(h1.Find(12), 1);
	EXPECT_EQ(h1.Find(3), 0);
	EXPECT_EQ(h1.Find(33), 1);
	EXPECT_EQ(h1.Find(24), 0);
	EXPECT_EQ(h1.Find(85), 0);

	EXPECT_EQ(h1.Find(43), -1);
	EXPECT_EQ(h1.Find(154), -1);
	EXPECT_EQ(h1.Find(95), -1);
	
}


// тестирование удаления
TEST(TestHashTable, TestDelete) {

	HashTable<int> h1(10, HashF10); // 10 блоков

	h1.Insert(2);
	h1.Insert(12);
	h1.Insert(3);
	h1.Insert(33);
	h1.Insert(24);
	h1.Insert(85);

	h1.Delete(24);
	EXPECT_EQ(h1.Find(24), -1);

	h1.Delete(85);
	EXPECT_EQ(h1.Find(85), -1);

	// удаление несуществующего значения
	h1.Delete(112);
	EXPECT_EQ(h1.Find(112), -1);

	

}

// тестирование количества элементов в хеш-таблице
TEST(TestHashTable, TestNum) {

	HashTable<int> h1(10, HashF10); // 10 блоков

	EXPECT_EQ(0, h1.NumberOfNodes());

	h1.Insert(2);
	h1.Insert(12);
	h1.Insert(3);
	h1.Insert(33);
	h1.Insert(24);
	h1.Insert(85);

	EXPECT_EQ(6, h1.NumberOfNodes());

	h1.Delete(85);
	EXPECT_EQ(5, h1.NumberOfNodes());

	h1.Delete(85);
	EXPECT_EQ(5, h1.NumberOfNodes());

	h1.Insert(3);
	EXPECT_EQ(5, h1.NumberOfNodes());

}
