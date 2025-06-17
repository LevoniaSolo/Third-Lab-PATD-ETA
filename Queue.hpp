#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "Sequence.hpp"
#include "LinkedList.hpp"
#include <functional>
#include <utility>

template<typename T>
class Queue : public Sequence<T> {
public:
    Queue() : list(new LinkedList<T>()) {}
    Queue(const T* items, int count) : list(new LinkedList<T>(items, count)) {}
    Queue(const Queue<T>& other) : list(new LinkedList<T>(*other.list)) {}
    ~Queue() {
        delete list;
    }

    void Enqueue(T item) {
        list->Append(item);
    }

    T Dequeue() {
        return list->RemoveFirst();
    }

    T GetFirst() const override {
        return list->GetFirst();
    }

    int GetLength() const override {
        return list->GetLength();
    }

    void AppendSequence(Queue<T>* other) {
        if (other != nullptr) {
            list->AppendList(other->list);
            other->list = new LinkedList<T>();
        }
    }

    Queue<T>* Concat(Sequence<T>* other) const override {
        Queue<T>* tmp = new Queue<T>(*this);
        Queue<T>* otherQueue = dynamic_cast<Queue<T>*>(other);
        if (otherQueue) {
            LinkedList<T>* concatList = tmp->list->Concat(otherQueue->list);
            delete tmp->list;
            tmp->list = concatList;
        } else {
            for (int i = 0; i < other->GetLength(); i++) {
                tmp->list->Append(other->Get(i));
            }
        }
        return tmp;
    }

    Queue<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);
        Queue<T>* tmp = new Queue<T>();
        delete tmp->list;
        tmp->list = subList;
        return tmp;
    }

    Queue<T>* Map(std::function<T(T)> f) const override {
        Queue<T>* tmp = new Queue<T>(*this);
        LinkedList<T>* mappedList = tmp->list->Map(f);
        delete tmp->list;
        tmp->list = mappedList;
        return tmp;
    }

    Queue<T>* Where(std::function<bool(T)> predicate) const override {
        Queue<T>* tmp = new Queue<T>(*this);
        LinkedList<T>* filteredList = tmp->list->Where(predicate);
        delete tmp->list;
        tmp->list = filteredList;
        return tmp;
    }

    T Reduce(std::function<T(T, T)> f, T initial) const override {
        return list->Reduce(f, initial);
    }

    bool ContainsSubsequence(Sequence<T>* sub) const override {
        Queue<T>* subQueue = dynamic_cast<Queue<T>*>(sub);
        if (subQueue) {
            return list->ContainsSubsequence(subQueue->list);
        }
        LinkedList<T>* temp = new LinkedList<T>();
        for (int i = 0; i < sub->GetLength(); i++) {
            temp->Append(sub->Get(i));
        }
        bool result = list->ContainsSubsequence(temp);
        delete temp;
        return result;
    }

    std::pair<Queue<T>*, Queue<T>*> SplitIntoTwo(std::function<bool(T)> predicate) {
        Queue<T>* trueQueue = new Queue<T>();
        Queue<T>* falseQueue = new Queue<T>();
        for (int i = 0; i < list->GetLength(); i++) {
            T item = list->Get(i);
            if (predicate(item)) {
                trueQueue->Enqueue(item);
            } else {
                falseQueue->Enqueue(item);
            }
        }
        return std::make_pair(trueQueue, falseQueue);
    }

protected:
    LinkedList<T>* list;

    virtual Queue<T>* Instance() {
        return this;
    }

private:

    T Get(int index) const override {
        throw std::logic_error("Get by index is not supported for Queue");
    }

    Sequence<T>* Append(T item) override {
        throw std::logic_error("Append is not supported for Queue; use Enqueue");
    }

    Sequence<T>* Prepend(T item) override {
        throw std::logic_error("Prepend is not supported for Queue");
    }

    Sequence<T>* InsertAt(T item, int index) override {
        throw std::logic_error("InsertAt is not supported for Queue");
    }

    T GetLast() const override {
        throw std::logic_error("GetLast is not supported for Queue");
    }
};

#endif