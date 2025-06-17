#ifndef DEQUE_HPP
#define DEQUE_HPP
#include "Sequence.hpp"
#include "DoublyLinkedList.hpp"
#include <functional>
#include <utility>

template<typename T>
class Deque : public Sequence<T> {
public:
    Deque() : list(new DoublyLinkedList<T>()) {}
    Deque(const T* items, int count) : list(new DoublyLinkedList<T>(items, count)) {}
    Deque(const Deque<T>& other) : list(new DoublyLinkedList<T>(*other.list)) {}
    ~Deque() {
        delete list;
    }

    void PushFront(T item) {
        list->Prepend(item);
    }

    void PushBack(T item) {
        list->Append(item);
    }

    T PopFront() {
        return list->RemoveFirst();
    }

    T PopBack() {
        return list->RemoveLast();
    }

    T GetFirst() const override {
        return list->GetFirst();
    }

    T GetLast() const override {
        return list->GetLast();
    }

    int GetLength() const override {
        return list->GetLength();
    }

    Deque<T>* Sort(std::function<bool(T, T)> comparator) {
        Deque<T>* tmp = new Deque<T>(*this);
        tmp->list->Sort(comparator);
        return tmp;
    }

    Deque<T>* Concat(Sequence<T>* other) const override {
        Deque<T>* tmp = new Deque<T>(*this);
        Deque<T>* otherDeque = dynamic_cast<Deque<T>*>(other);
        if (otherDeque) {
            DoublyLinkedList<T>* concatList = tmp->list->Concat(otherDeque->list);
            delete tmp->list;
            tmp->list = concatList;
        } else {
            for (int i = 0; i < other->GetLength(); i++) {
                tmp->list->Append(other->Get(i));
            }
        }
        return tmp;
    }

    Deque<T>* GetSubsequence(int startIndex, int endIndex) const override {
        DoublyLinkedList<T>* subList = list->GetSubList(startIndex, endIndex);
        Deque<T>* tmp = new Deque<T>();
        delete tmp->list;
        tmp->list = subList;
        return tmp;
    }

    Deque<T>* Map(std::function<T(T)> f) const override {
        Deque<T>* tmp = new Deque<T>(*this);
        DoublyLinkedList<T>* mappedList = tmp->list->Map(f);
        delete tmp->list;
        tmp->list = mappedList;
        return tmp;
    }

    Deque<T>* Where(std::function<bool(T)> predicate) const override {
        Deque<T>* tmp = new Deque<T>(*this);
        DoublyLinkedList<T>* filteredList = tmp->list->Where(predicate);
        delete tmp->list;
        tmp->list = filteredList;
        return tmp;
    }

    T Reduce(std::function<T(T, T)> f, T initial) const override {
        return list->Reduce(f, initial);
    }

    bool ContainsSubsequence(Sequence<T>* sub) const override {
        Deque<T>* subDeque = dynamic_cast<Deque<T>*>(sub);
        if (subDeque) {
            return list->ContainsSubsequence(subDeque->list);
        }
        DoublyLinkedList<T>* temp = new DoublyLinkedList<T>();
        for (int i = 0; i < sub->GetLength(); i++) {
            temp->Append(sub->Get(i));
        }
        bool result = list->ContainsSubsequence(temp);
        delete temp;
        return result;
    }

    Deque<T>* Merge(Deque<T>* other, std::function<bool(T, T)> comparator) {
        Deque<T>* tmp = new Deque<T>(*this);
        DoublyLinkedList<T>* left = new DoublyLinkedList<T>(*tmp->list);
        DoublyLinkedList<T>* right = new DoublyLinkedList<T>(*other->list);
        tmp->list->Merge(left, right, comparator);
        delete left;
        delete right;
        return tmp;
    }

protected:
    DoublyLinkedList<T>* list;

    virtual Deque<T>* Instance() {
        return this;
    }

private:
    // Unsupported Sequence<T> methods
    T Get(int index) const override {
        throw std::logic_error("Get by index is not supported for Deque");
    }

    Sequence<T>* Append(T item) override {
        throw std::logic_error("Append is not supported for Deque; use PushBack");
    }

    Sequence<T>* Prepend(T item) override {
        throw std::logic_error("Prepend is not supported for Deque; use PushFront");
    }

    Sequence<T>* InsertAt(T item, int index) override {
        throw std::logic_error("InsertAt is not supported for Deque");
    }
};

#endif