#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP
#include <functional>
#include <stdexcept>

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(T value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    DoublyLinkedList(const T* items, int count) : head(nullptr), tail(nullptr), size(0) {
        if (count < 0) throw std::invalid_argument("Count cannot be negative");
        if (items == nullptr || count == 0) return;
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        Node* current = other.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }

    ~DoublyLinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void Append(T item) {
        Node* node = new Node(item);
        if (!head) {
            head = tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        size++;
    }

    void Prepend(T item) {
        Node* node = new Node(item);
        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        size++;
    }

    T RemoveFirst() {
        if (!head) throw std::out_of_range("List is empty");
        Node* oldHead = head;
        T value = oldHead->data;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        size--;
        delete oldHead;
        return value;
    }

    T RemoveLast() {
        if (!tail) throw std::out_of_range("List is empty");
        Node* oldTail = tail;
        T value = oldTail->data;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        size--;
        delete oldTail;
        return value;
    }

    T GetFirst() const {
        if (!head) throw std::out_of_range("List is empty");
        return head->data;
    }

    T GetLast() const {
        if (!tail) throw std::out_of_range("List is empty");
        return tail->data;
    }

    int GetLength() const {
        return size;
    }

    DoublyLinkedList* GetSubList(int startIndex, int endIndex) const {
        if (size <= 0 || startIndex == endIndex) return new DoublyLinkedList();
        if (startIndex < 0 || endIndex > size || startIndex > endIndex) {
            throw std::out_of_range("Invalid index range");
        }
        DoublyLinkedList* result = new DoublyLinkedList();
        Node* current = head;
        for (int i = 0; i < endIndex; i++) {
            if (i >= startIndex) {
                result->Append(current->data);
            }
            current = current->next;
        }
        return result;
    }

    DoublyLinkedList* Concat(const DoublyLinkedList* other) const {
        DoublyLinkedList* result = new DoublyLinkedList(*this);
        if (!other || other->size == 0) return result;
        Node* current = other->head;
        while (current) {
            result->Append(current->data);
            current = current->next;
        }
        return result;
    }

    DoublyLinkedList* Map(std::function<T(T)> f) const {
        DoublyLinkedList* result = new DoublyLinkedList();
        Node* current = head;
        while (current) {
            result->Append(f(current->data));
            current = current->next;
        }
        return result;
    }

    template <typename U>
    DoublyLinkedList<U>* MapTo(std::function<U(T)> f) const {
        DoublyLinkedList<U>* result = new DoublyLinkedList<U>();
        Node* current = head;
        while (current) {
            result->Append(f(current->data));
            current = current->next;
        }
        return result;
    }

    DoublyLinkedList* Where(std::function<bool(T)> predicate) const {
        DoublyLinkedList* result = new DoublyLinkedList();
        Node* current = head;
        while (current) {
            if (predicate(current->data)) {
                result->Append(current->data);
            }
            current = current->next;
        }
        return result;
    }

    T Reduce(std::function<T(T, T)> f, T initial) const {
        T result = initial;
        Node* current = head;
        while (current) {
            result = f(result, current->data);
            current = current->next;
        }
        return result;
    }

    bool ContainsSubsequence(const DoublyLinkedList* sub) const {
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

    void Sort(std::function<bool(T, T)> comparator) {
        if (size <= 1) return;
        Node* mid = head;
        for (int i = 0; i < size / 2; i++) {
            mid = mid->next;
        }
        DoublyLinkedList left, right;
        Node* current = head;
        while (current != mid) {
            left.Append(current->data);
            current = current->next;
        }
        while (current) {
            right.Append(current->data);
            current = current->next;
        }
        // std::cout << left.GetFirst() << left.GetLast() << right.GetFirst() << right.GetLast() << mid->data <<std::endl;
        left.Sort(comparator);
        right.Sort(comparator);
        // std::cout << left.GetFirst() << left.GetLast() << right.GetFirst() << right.GetLast() << mid->data <<std::endl; 

        Merge(&left, &right, comparator);
        // std::cout << GetFirst() << GetLast() <<std::endl; 
        // head = left.head;
        // tail = left.tail;
        // size = left.size;
        // left.head = nullptr;
        // left.tail = nullptr;
        // left.size = 0;
        // std::cout << GetFirst() << GetLast() << GetLength() << std::endl; 
    }

    void Merge(DoublyLinkedList* left, DoublyLinkedList* right, std::function<bool(T, T)> comparator) {
        DoublyLinkedList result;
        Node* leftCurrent = left->head;
        Node* rightCurrent = right->head;
        while (leftCurrent && rightCurrent) {
            if (comparator(leftCurrent->data, rightCurrent->data)) {
                result.Append(leftCurrent->data);
                leftCurrent = leftCurrent->next;
            } else {
                result.Append(rightCurrent->data);
                rightCurrent = rightCurrent->next;
            }
        }
        // std::cout << left->GetFirst() << left->GetLast() << right->GetFirst() << right->GetLast() <<std::endl; 
        while (leftCurrent) {
            result.Append(leftCurrent->data);
            leftCurrent = leftCurrent->next;
        }
        while (rightCurrent) {
            result.Append(rightCurrent->data);
            rightCurrent = rightCurrent->next;
        }
        head = result.head;
        tail = result.tail;
        size = result.size;
        result.head = nullptr;
        result.tail = nullptr;
        result.size = 0;
        // std::cout << "etc" << left->GetFirst() << left->GetLast() << right->GetFirst() << right->GetLast() <<std::endl; 

    }
};

#endif