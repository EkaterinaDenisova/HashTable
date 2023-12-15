#pragma once
//author: �������� ���������

#include <vector>
#include <iostream>
#include <stdexcept> // ��� ����������
#include "LinkedList.h"
using namespace std;

template <class T>
class HashTableIterator;

// ����� ���-�������
template <class T>
class HashTable
{
private:
	// ����� ������; ������������ ������ �������
	int numBuckets;
	// ������ ��������� �������
	vector< LinkedList<T> > buckets;
	// ���-�������
	// todo ��������� ��� ��� ��������� �� �������
	unsigned long (*hf)(T key);
public:
	// ����������� � �����������, �����������
	// ������ ������� � ���-�������
	HashTable(int nbuckets, unsigned long hashf(T key));

	// ������ ��������� �������

	// ������� � ���-�������
	void Insert(const T& key);

	// ����� � ���-������� �� �����
	int Find(const T& key);

	// �������� �� ���-�������
	void Delete(const T& key);

	// ������� �������
	void ClearTable();

	// ����� ������� � �������
	void PrintTable();
	// ���������� ������ � �������
	//void Update(const T& key);


	friend class HashTableIterator<T>;


	HashTableIterator<T> begin() const {
		return HashTableIterator<T>(*this);
	}

	HashTableIterator<T> end() const {
		return HashTableIterator<T>(nullptr);
	}

};

// ����� ��������� ��� ���-�������
template<typename T>
class HashTableIterator : public AbsIterator<T> {
private:
	// ��������� �������, ���������� ������
	HashTable<T>* ht;

	int currentBucket; // ������ ���������� ������
	// �������� � ������ �� ������� currentBucket
	LinkedList::LinkedListIterator<T> currBucketPtr;

	// ������� ��� ���������� ������ ++
	// ������� � cb, ������ ��������� �������� ������ ��� ���������
	void SearchNextNode(int cb) {
		currentBucket = -1;
		// ���� ������ cb ������ ������� �������, ���������� �����
		if (!(cb > ht->numBuckets)) {

			// ����� ������ �������� ����, ������� � �������� ������ �� ����� �������,
			// � �������� ������� �������� ������
			for (int i = cb; i < ht->numBuckets; i++)
				if (!ht->buckets[i].ListEmpty())
				{
					// ����� ��� ��� ���������, ���������� currentBucket ������ i
					//� � currBucketPtr ��������� ����� ������ ��������� ������
					
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


	// �����������, �������������� ������� ����� � ����� HashTable
	// SearchNextNode �������������� ������ �������� ���� � �������
	HashTableIterator(HashTable<T>* hf) {
		this->ht = hf;

		//Iterator<T>(hf), HashTable(&hf)
		SearchNextNode(0);
	}

	// �������� ���������
	bool operator==(const AbsIterator<T>& o) const override {
		return (*currBucketPtr == dynamic_cast<const HashTableIterator<T>&>(o).*currBucketPtr)
			&& (this->currentBucket == dynamic_cast<const HashTableIterator<T>&>(o).*currentBucket);
	}

	bool operator!=(const AbsIterator<T>& o) const override {
		return !((currBucketPtr == dynamic_cast<const HashTableIterator<T>&>(o).currBucketPtr)
			&& (this->currentBucket == dynamic_cast<const HashTableIterator<T>&>(o).currentBucket));
	}

	// �������� ������� � ������
	T& operator*() const override {
		//���������� ������ �������� ����
		return *currBucketPtr;
	}

	// �������� �������� �� ��������� �������
	HashTableIterator& operator++() override {

		++currBucketPtr;
		if (currBucketPtr == ht->buckets[currentBucket].end()) {
			SearchNextNode(++currentBucket);
		}
		return *this;


		//���� ������� �� ����� nullptr
		//if (current != nullptr) {
			//��������� � ���������� ����
			//current = current->next;
		//}
		//���������� ������ �� ������� ������
		//return *this;
	}

	~HashTableIterator() {

	}

};

// ����������� � �����������, �����������
// ������ ������� � ���-�������
template <class T>
HashTable<T>::HashTable(int nbuckets, unsigned long hashf(T key)) {
	if (nbuckets <= 0) {
		throw invalid_argument("������������ ������ ���-�������");
	}
	else {
		this->numBuckets = nbuckets;
		this->hf = hashf;
		this->buckets.resize(nbuckets);
	}
}

// ���������� �������� � ���-�������
template <class T>
void HashTable<T>::Insert(const T& key)
{
	// hashval � ������ ����� (���������� ������)
	int hashval = hf(key) % numBuckets;
	// lst � ��������� ��� buckets[hashval].
	// �������� �������� ��� ��������
	LinkedList<T>& lst = buckets[hashval];

	bool is_in_hash = false; // ���������, ���� �� ������� � ���-�������
	//����� �� ���� ����������

	for (auto it2 : lst) {
		if (it2 == key) {
			is_in_hash = true;
		}
	}

	// ���� ��������� ���
	if (!is_in_hash) {
		lst.addNode(key);
	}

	/*for (lst.begin(); !1st.EndOfList(); Ist.NextO)
		// ���� ���� ������, �������� ������ � �����
		if (Ist.DataO == key)
		{
			1st.Data() = key;
			current = &lst.Data();
			return;
		}
	// ������, ��������������� ����� �����, �� �������, �������� ������� � ������
	1st.InsertRear(key);
	current - &lst.Data();
	size++;*/
}

// ����� � ���-�������
// ���������� ������ �������� � ������ �� ������� �������, ���� 
// -1, ���� � ������� ��� ������ ��������
template <class T>
int HashTable<T>::Find(const T& key)
{
	// ��������� �������� ���-������� � ���������� lst
	// �� ������ ���������������� ���������� ������
	int hashval = hf(key) % numBuckets;
	LinkedList<T>& lst = buckets[hashval];


	// ������������� ���� ���������� ������ � ������� key
	// ������� ������
	return lst.searchNodeInd(key);
	
}

// �������� �������� �� ���-�������
// ���� �������� ���, �� ������ �� ����������
template <class T>
void HashTable<T>::Delete(const T& key)
{
	// ��������� �������� ���-������� � ���������� lst
	// �� ������ ���������������� ���������� ������
	int hashval = hf(key) % numBuckets;
	LinkedList<T>& lst = buckets[hashval];

	// ������������� ���� ���������� ������ � ������� key
	// ���� ������, �� ������� ��� �� ������
	lst.removeNode(key);

}

// ������� �������
template <class T>
void HashTable<T>::ClearTable() {
	LinkedList<T>& lst;
	for (int i = 0; i < numBuckets; i++) {
		lst = buckets[i];
		lst.ClearList();
	}
}

// ����� ������� � �������
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