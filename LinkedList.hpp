#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP
#include <stdlib.h>
#include <iostream>

template<typename T>
class LinkedList {
    class Node;

public:
    //Конструкторы    
    LinkedList(const T* items, int count) : head(nullptr), tail(nullptr), size(0) {
        if (count < 0) {
            throw std::invalid_argument("Count cannot be negative");
        }
        if (items == nullptr || count == 0) {
            return;
        }

        Node* last = nullptr;
        for (int i = 0; i < count; i++) {
            Node* newNode = new Node;
            newNode->data = items[i];
            newNode->next = nullptr;
            
            if (last == nullptr) {
                head = newNode;
            } else {
                last->next = newNode;
            }
            
            last = newNode;
            size++;
        }
        tail = last;
    }

    LinkedList(): head(nullptr), tail(nullptr), size(0) {}

    // Конструктор копирования
    LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), size(0) {
        Node* current = list.head;
        Node* last = nullptr;
        
        while (current != nullptr) {
            Node* newNode = new Node(current->data);
            // newNode->data = current->data;
            // newNode->next = nullptr;
            
            if (last == nullptr) {
                head = newNode;
            } else {
                last->next = newNode;
            }
            
            last = newNode;
            current = current->next;
            size++;
        }
        tail = last;
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    
    //Декомпозиция
    T GetFirst() const{
        if (head == nullptr) {
            throw std::out_of_range("List is empty");
        }
        return head->data;
    }

    T Get(int index) const{
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for(int i = 0; i<index; i++){
            current = current->next;
        }
        return current->data;
    }

    T GetLast() const {
        if (size == 0 || tail == nullptr) {
            throw std::out_of_range("List is empty");
        }
        return tail->data;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const{
        if (!head || size <= 0 || startIndex == endIndex){ 
            return new LinkedList<T>();
        }
        if (startIndex < 0 || endIndex > size || startIndex > endIndex) {
            throw std::out_of_range("Invalid index range");
        }
        LinkedList<T>* list = new LinkedList<T>();

        Node* current = head;
        Node* last = nullptr;
        
        for (int i = 0; i<size; i++) {
            if(i >= startIndex && i < endIndex){
                Node* newNode = new Node(current->data);
                // newNode->data = current->data;
                // newNode->next = nullptr;
                
                if (last == nullptr) {
                    list->head = newNode;
                } 
                else {
                    last->next = newNode;
                }
                
                last = newNode;
                list->size++;
            }
            current = current->next;
        }
        return list;
    }

    int GetLength() const{
        return size;
    }

    //Операции
    void Append(T item){
        Node* newelem = new Node(item);
        // newelem->data = item;
        // newelem->next = nullptr;
        if (head == nullptr){
            head = newelem;
        }
        else{
            tail->next = newelem;
        }
        tail = newelem;
        size++;
    }

    void Prepend(T item) {
        Node* newelem = new Node;
        newelem->data = item;
        newelem->next = head;
        head = newelem;
        if (size == 0) {
            tail = newelem;
        }
        size++;
    }

    void InsertAt(T item, int index) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == size) {
            Append(item);
            return;
        }
        Node* current = head;
        for (int i = 1; i < index; i++) {
            current = current->next;
        }
        Node* newelem = new Node;
        newelem->data = item;
        newelem->next = current->next;
        current->next = newelem;
        size++;
    }

    LinkedList<T>* Concat(const LinkedList<T> *other) const{
        LinkedList<T>* listNew = new LinkedList<T>(*this);
        if (other == nullptr || other->size == 0) {
            return listNew;
        }
        Node* current = other->head;
        for(int i = 0; i<other->size; i++){
            listNew->Append(current->data);
            current = current->next;
        }
        return listNew;
    }

    //map-reduce
    
    LinkedList<T>* Map(std::function<T(T)> f) const {
        LinkedList<T>* result = new LinkedList<T>();
        Node* current = head;
        while (current != nullptr) {
            result->Append(f(current->data));
            current = current->next;
        }
        return result;
    }
    template <typename U>
    LinkedList<U>* MapTo(std::function<U(T)> f) const {
        LinkedList<U>* result = new LinkedList<U>();
        Node* current = head;
        while (current != nullptr) {
            result->Append(f(current->data));
            current = current->next;
        }
        return result;
    }

    LinkedList<T>* Where(std::function<bool(T)> f) const {
        LinkedList<T>* result = new LinkedList<T>();
        Node* current = head;
        while (current != nullptr) {
            if (f(current->data)) {
                result->Append(current->data);
            }
            current = current->next;
        }
        return result;
    }

    T Reduce(std::function<T(T, T)> f, const T& c) const {
        if (size == 0 || head == nullptr) return c;
        T answer = c;
        Node* current = head;
        while (current != nullptr) {
            answer = f(answer, current->data);
            current = current->next;
        }
        return answer;
    }

    bool ContainsSubsequence(const LinkedList<T>* sub) const {
        if (!sub || sub->size > size) return false;
        Node* current = head;
        for (int i = 0; i <= size - sub->size; i++) {
            bool match = true;
            Node* subCurrent = sub->head;
            Node* temp = current;
            for (int j = 0; j < sub->size; j++) {
                if (!temp || temp->data != subCurrent->data) {
                    match = false;
                    break;
                }
                temp = temp->next;
                subCurrent = subCurrent->next;
            }
            if (match) return true;
            current = current->next;
        }
        return false;
    }

    //ВСЯКИЕ ФУНКЦИИ

    T RemoveFirst() {
        if (head == nullptr) throw std::out_of_range("List is empty");
        Node* oldHead = head;
        T value = oldHead->data;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        size--;
        delete oldHead;
        return value;
    }

    void AppendList(LinkedList<T>* other) {
        if (other == nullptr || other->size == 0) return;
        if (size == 0) {
            head = other->head;
            tail = other->tail;
        } else {
            tail->next = other->head;
            tail = other->tail;
        }
        size += other->size;
        // Prevent other from deleting nodes
        other->head = nullptr;
        other->tail = nullptr;
        other->size = 0;
    }

    //Итераторы
    class Iterator {
        public:
            Iterator(Node* node) : current(node) {}
            const T& operator*() const {
                if (!current) {
                    throw std::out_of_range("Iterator out of range");
                }
                return current->data;
            }
            Iterator& operator++() {
                if (current) {
                    current = current->next;
                }
                return *this;
            }
            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }
        private:
            Node* current;
    };
        
    Iterator begin() const {
        return Iterator(head);
    }
        
    Iterator end() const {
        return Iterator(nullptr);
    }
    


private:
    class Node {
        public:
            T data;
            Node* next;
            Node(T value) : data(value), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;
};

#endif