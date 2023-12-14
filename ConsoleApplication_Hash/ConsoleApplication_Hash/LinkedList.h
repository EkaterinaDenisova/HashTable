#pragma once
//author: Денисова Екатерина

#include <iostream>
#include <vector>
#include <stdexcept> // для исключений
#include "AbsIterator.h"
#include <fstream>
// класс узла списка
template <typename T>
class Node {
public:
    T data;
    Node<T>* next;

    // конструктор с параметром
    Node(const T& data1) {
        this->data = data1;
        next = nullptr;
    }
};

// класс односвязного списка
template <typename T>
class LinkedList {
private:
    // указатель на начало списка
    Node<T>* head;

public:
    // конструктор без параметров
    LinkedList() {
        head = nullptr;
    }

    // деструктор
    // удаляются все узлы в списке
    ~LinkedList() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void ClearList() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // добавление узла в конец списка
    void addNode(const T& data);
    // Удаление узла по значению
    void removeNode(const T& data);
    // поиск узла по значению
    // true если узел найден, иначе false
    bool searchNode(T& data);
    // поиск индекса узла, если не найден, то -1
    int searchNodeInd(const T& data);
    // значение узла по индексу
    T& dataByInd(int ind);
    // вывод списка в консоль
    void printList();
    // вывод списка в файл
    void printListToFile(const std::string& filename);
    // количеств узлов в списке
    int ListSize();

    // проверка, пустой ли список 
    bool ListEmpty() {
        Node<T>* current = head;
        if (current == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    std::vector<T> ListToVec();


    // класс итератора для Linked List
    template<typename T>
    class LinkedListIterator : public AbsIterator<T> {
    private:
        Node<T>* current;  // Текущий узел

    public:
        // конструктор с параметром
        LinkedListIterator(Node<T>* x) {
            current = x;
        }

        // оператор сравнения
        bool operator==(const AbsIterator<T>& o) const override {
            return current == dynamic_cast<const LinkedListIterator<T>&>(o).current;
        }

        bool operator!=(const AbsIterator<T>& o) const override {
            return !(current == dynamic_cast<const LinkedListIterator<T>&>(o).current);
        }

        // оператор доступа к данным
        T& operator*() const override {
            //возвращает данные текущего узла
            return current->data;
        }

        // оператор перехода на следующий узел
        LinkedListIterator& operator++() override {
            //если текущий не равен nullptr
            if (current != nullptr) {
                //переходим к следующему узлу
                current = current->next;
            }
            //возвращаем ссылку на текущий объект
            return *this;
        }


    };


    LinkedListIterator<T> begin() const {
        return LinkedListIterator<T>(head);
    }

    LinkedListIterator<T> end() const {
        return LinkedListIterator<T>(nullptr);
    }

};


// добавление узла в список
template<typename T>
void LinkedList<T>::addNode(const T& data) {
    Node<T>* newNode = new Node<T>(data);

    // если список пустой, то создаём первый элемент
    if (head == nullptr) {
        head = newNode;
    }
    // находим последний узел списка и добавляем новый узел после него
    else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Удаление узла по значению
template<typename T>
void LinkedList<T>::removeNode(const T& data) {
    // если список не пустой
    if (head != nullptr) {

        // если нужно удалить узел из начала
        if (head->data == data) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;

        }
        // если удаляемый узел не в начале
        // ищем узел на удаление
        else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                // если нужный узел найден
                // удаляем его и меняем указатели
                if (current->next->data == data) {
                    Node<T>* temp = current->next;
                    current->next = current->next->next;
                    delete temp;
                    return;
                }
                current = current->next;
            }
        }
    }
}

// поиск узла
// true, если есть узел
// иначе false
template<typename T>
bool LinkedList<T>::searchNode(T& data) {
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == data) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// поиск индекса узла
// -1, если нет узел
template<typename T>
int LinkedList<T>::searchNodeInd(const T& data) {
    Node<T>* current = head;
    int ind = 0;
    while (current != nullptr) {
        if (current->data == data) {
            return ind;
        }
        current = current->next;
        ind++;
    }
    return -1;
}

// значение узла по индексу
template<typename T>
T& LinkedList<T>::dataByInd(int ind) {
    if (ind < 0) {
        throw std::invalid_argument("Недопустимое значение индекса");
    }
    else {
        int i = 0;
        //T res;

        Node<T>* current = head;

        while (i < ind) {
            if (current == nullptr || current->next == nullptr) {
                throw std::invalid_argument("Недопустимое значение индекса");
            }
            else {
                current = current->next;
                i++;
            }
        }

        return current->data;

    }
}

// вывод списка в консоль
template<typename T>
void LinkedList<T>::printList() {
    Node<T>* current = head;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// вывод списка в файл
template<typename T>
void LinkedList<T>::printListToFile(const std::string& filename) {
    std::ofstream fout(filename);
    if (!fout.is_open()) // если файл не был открыт
    {
        throw std::invalid_argument("Файл не может быть открыт");
    }

    Node<T>* current = head;
    while (current != nullptr) {
        fout << current->data << std::endl;
        current = current->next;
    }

    fout.close();
}

template<typename T>
int LinkedList<T>::ListSize() {
    int res = 0;
    Node<T>* current = head;
    while (current != nullptr) {
        res++;
        //std::cout << current->data << " ";
        current = current->next;
    }
    return res;
}

template<typename T>
std::vector<T> LinkedList<T>::ListToVec() {
    std::vector<T> t;
    Node<T>* current = this->head;
    while (current != nullptr) {
        t.push_back(current->data);
        
        current = current->next;
    }
    return t;

}


