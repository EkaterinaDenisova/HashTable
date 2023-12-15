#pragma once
//author: Денисова Екатерина

#include <vector>
#include <iostream>
#include <stdexcept> // для исключений
#include "LinkedList.h"
using namespace std;

template <class T>
class HashTableIterator;

// Класс хеш-таблицы
template <class T>
class HashTable
{
private:
	// число блоков; представляет размер таблицы
	int numBuckets;
	// массив связанных списков
	vector< LinkedList<T> > buckets;
	// хеш-функция
	// todo отдельный тип для указателя на функцию
	unsigned long (*hf)(T key);
public:
	// конструктор с параметрами, включающими
	// размер таблицы и хеш-функцию
	HashTable(int nbuckets, unsigned long hashf(T key));

	// методы обработки списков

	// вставка в хеш-таблицу
	void Insert(const T& key);

	// поиск в хеш-таблице по ключу
	int Find(const T& key);

	// удаление из хеш-таблицы
	void Delete(const T& key);

	// очистка таблицы
	void ClearTable();

	// вывод таблицы в консоль
	void PrintTable();
	// обновление данных в таблице
	//void Update(const T& key);


	friend class HashTableIterator<T>;


	HashTableIterator<T> begin() const {
		return HashTableIterator<T>(*this);
	}

	HashTableIterator<T> end() const {
		return HashTableIterator<T>(nullptr);
	}

};

// класс итератора для хеш-таблицы
template<typename T>
class HashTableIterator : public AbsIterator<T> {
private:
	// указатель таблицы, подлежащей обходу
	HashTable<T>* ht;

	int currentBucket; // индекс связанного списка
	// итератор в списке по индексу currentBucket
	LinkedList::LinkedListIterator<T> currBucketPtr;

	// утилита для реализации метода ++
	// начиная с cb, искать следующий непустой список для просмотра
	void SearchNextNode(int cb) {
		currentBucket = -1;
		// если индекс cb больше размера таблицы, прекратить поиск
		if (!(cb > ht->numBuckets)) {

			// иначе искать непустой блок, начиная с текущего списка до конца таблицы,
			// и обновить частные элементы данных
			for (int i = cb; i < ht->numBuckets; i++)
				if (!ht->buckets[i].ListEmpty())
				{
					// перед тем как вернуться, установить currentBucket равным i
					//и в currBucketPtr поместить адрес нового непустого списка
					
					//currBucketPtr(ht->buckets[i].begin());
					currBucketPtr = ht->buckets[i].begin();
					//currBucketPtr->Reset();
					currentBucket = i;
					break;
					//return;
				}
		}
	}

public:


	// конструктор, инициализирует базовый класс и класс HashTable
	// SearchNextNode идентифицирует первый непустой блок в таблице
	HashTableIterator(HashTable<T>* hf) {
		this->ht = hf;

		//Iterator<T>(hf), HashTable(&hf)
		SearchNextNode(0);
	}

	// оператор сравнения
	bool operator==(const AbsIterator<T>& o) const override {
		return (*currBucketPtr == dynamic_cast<const HashTableIterator<T>&>(o).*currBucketPtr)
			&& (this->currentBucket == dynamic_cast<const HashTableIterator<T>&>(o).*currentBucket);
	}

	bool operator!=(const AbsIterator<T>& o) const override {
		return !((currBucketPtr == dynamic_cast<const HashTableIterator<T>&>(o).currBucketPtr)
			&& (this->currentBucket == dynamic_cast<const HashTableIterator<T>&>(o).currentBucket));
	}

	// оператор доступа к данным
	T& operator*() const override {
		//возвращает данные текущего узла
		return *currBucketPtr;
	}

	// оператор перехода на следующий элемент
	HashTableIterator& operator++() override {

		++currBucketPtr;
		if (currBucketPtr == ht->buckets[currentBucket].end()) {
			SearchNextNode(++currentBucket);
		}
		return *this;


		//если текущий не равен nullptr
		//if (current != nullptr) {
			//переходим к следующему узлу
			//current = current->next;
		//}
		//возвращаем ссылку на текущий объект
		//return *this;
	}

	~HashTableIterator() {

	}

};

// конструктор с параметрами, включающими
// размер таблицы и хеш-функцию
template <class T>
HashTable<T>::HashTable(int nbuckets, unsigned long hashf(T key)) {
	if (nbuckets <= 0) {
		throw invalid_argument("Недопустимый размер хеш-таблицы");
	}
	else {
		this->numBuckets = nbuckets;
		this->hf = hashf;
		this->buckets.resize(nbuckets);
	}
}

// добавление элемента в хеш-таблицу
template <class T>
void HashTable<T>::Insert(const T& key)
{
	// hashval — индекс блока (связанного списка)
	int hashval = hf(key) % numBuckets;
	// lst — псевдоним для buckets[hashval].
	// помогает обойтись без индексов
	LinkedList<T>& lst = buckets[hashval];

	bool is_in_hash = false; // проверяем, есть ли элемент в хеш-таблице
	//чтобы не было дубликатов

	for (auto it2 : lst) {
		if (it2 == key) {
			is_in_hash = true;
		}
	}

	// если дубликата нет
	if (!is_in_hash) {
		lst.addNode(key);
	}

	/*for (lst.begin(); !1st.EndOfList(); Ist.NextO)
		// если ключ совпал, обновить данные и выйти
		if (Ist.DataO == key)
		{
			1st.Data() = key;
			current = &lst.Data();
			return;
		}
	// данные, соответствующие этому ключу, не найдены, вставить элемент в список
	1st.InsertRear(key);
	current - &lst.Data();
	size++;*/
}

// поиск в хеш-таблице
// возвращает индекс элемента в списке по индексу таблицы, либо 
// -1, если в таблице нет такого значения
template <class T>
int HashTable<T>::Find(const T& key)
{
	// вычислить значение хеш-функции и установить lst
	// на начало соответствующего связанного списка
	int hashval = hf(key) % numBuckets;
	LinkedList<T>& lst = buckets[hashval];


	// просматривать узлы связанного списка в поисках key
	// вернуть индекс
	return lst.searchNodeInd(key);
	
}

// удаление элемента из хеш-таблицы
// если элемента нет, то ничего не происходит
template <class T>
void HashTable<T>::Delete(const T& key)
{
	// вычислить значение хеш-функции и установить lst
	// на начало соответствующего связанного списка
	int hashval = hf(key) % numBuckets;
	LinkedList<T>& lst = buckets[hashval];

	// просматривать узлы связанного списка в поисках key
	// если найден, то удалить его из списка
	lst.removeNode(key);

}

// очистка таблицы
template <class T>
void HashTable<T>::ClearTable() {
	LinkedList<T>& lst;
	for (int i = 0; i < numBuckets; i++) {
		lst = buckets[i];
		lst.ClearList();
	}
}

// вывод таблицы в консоль
template <class T>
void HashTable<T>::PrintTable() {
	//LinkedList<T>& lst;
	for (int i = 0; i < numBuckets; i++) {
		cout << i << ") ";
		buckets[i].printList();
		//lst.printList();
	}
	cout << endl;
}