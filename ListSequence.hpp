#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP
#include "Sequence.hpp"
#include "LinkedList.hpp"

template<typename T>
class ListSequence : public Sequence<T> {
public:
    ListSequence(const T* items, int count) {
        listSeq = new LinkedList<T>(items, count);
    }

    ListSequence() : ListSequence(nullptr, 0) {}

    ListSequence(const ListSequence<T>& other) {
        listSeq = new LinkedList<T>(*other.listSeq);
    }

    ListSequence(const LinkedList<T>& other) {
        listSeq = new LinkedList<T>(other);
    }

    ~ListSequence() {
        delete listSeq;
    }

    T GetFirst() const override {
        return listSeq->GetFirst();
    }

    T GetLast() const override {
        return listSeq->GetLast();
    }

    T Get(int index) const override {
        return listSeq->Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) override {
        LinkedList<T>* subList = listSeq->GetSubList(startIndex, endIndex);
        ListSequence<T>* result = new ListSequence<T>(*subList);
        delete subList;
        return result;
    }

    ListSequence<T>* ListGetSubsequence(int startIndex, int endIndex) {
        return dynamic_cast<ListSequence<T>*>(Instance()->GetSubsequence(startIndex, endIndex));
    }

    int GetLength() const override {
        return listSeq->GetLength();
    }

    Sequence<T>* Append(T item) override {
        ListSequence<T>* result = Instance();
        result->listSeq->Append(item);
        return result;
    }

    ListSequence<T>* ListAppend(T item) {
        return dynamic_cast<ListSequence<T>*>(Instance()->Append(item));
    }

    Sequence<T>* Prepend(T item) override {
        ListSequence<T>* result = Instance();
        result->listSeq->Prepend(item);
        return result;
    }

    ListSequence<T>* ListPrepend(T item) {
        return dynamic_cast<ListSequence<T>*>(Instance()->Prepend(item));
    }

    Sequence<T>* InsertAt(T item, int index) override {
        ListSequence<T>* result = Instance();
        result->listSeq->InsertAt(item, index);
        return result;
    }

    ListSequence<T>* ListInsertAt(T item, int index) {
        return dynamic_cast<ListSequence<T>*>(Instance()->InsertAt(item, index));
    }

    Sequence<T>* Concat(Sequence<T>* other) override {
        ListSequence<T>* result = Instance();
        ListSequence<T>* otherList = dynamic_cast<ListSequence<T>*>(other);
        if (otherList) {
            LinkedList<T>* concatList = result->listSeq->Concat(otherList->listSeq);
            delete result->listSeq;
            result->listSeq = concatList;
        } else {
            for (int i = 0; i < other->GetLength(); i++) {
                result->listSeq->Append(other->Get(i));
            }
        }
        return result;
    }

    ListSequence<T>* ListConcat(ListSequence<T>* other) {
        return dynamic_cast<ListSequence<T>*>(Instance()->Concat(other));
    }

    virtual ListSequence<T>* Instance() {
        return this;
    }

    Sequence<T>* Map(std::function<T(T)> f) override {
        ListSequence<T>* tmp = Instance();
        LinkedList<T>* mappedList = tmp->listSeq->Map(f);
        delete tmp->listSeq;
        tmp->listSeq = mappedList;
        return tmp;
    }

    template <typename U>
    Sequence<U>* MapTo(std::function<U(T)> f) override {
        ListSequence<U>* result = new ListSequence<U>();
        LinkedList<U>* mappedList = listSeq->MapTo(f);
        delete result->listSeq;
        result->listSeq = mappedList;
        return result;
    }

    Sequence<T>* Where(std::function<bool(T)> predicate) override {
        ListSequence<T>* tmp = Instance();
        LinkedList<T>* filteredList = tmp->listSeq->Where(predicate);
        delete tmp->listSeq;
        tmp->listSeq = filteredList;
        return tmp;
    }

    T Reduce(std::function<T(T, T)> f, T initial) override {
        return listSeq->Reduce(f, initial);
    }

    bool ContainsSubsequence(Sequence<T>* sub) override {
        ListSequence<T>* subList = dynamic_cast<ListSequence<T>*>(sub);
        if (subList) {
            return listSeq->ContainsSubsequence(subList->listSeq);
        }
        LinkedList<T>* temp = new LinkedList<T>();
        for (int i = 0; i < sub->GetLength(); i++) {
            temp->Append(sub->Get(i));
        }
        bool result = listSeq->ContainsSubsequence(temp);
        delete temp;
        return result;
    }

protected:
    LinkedList<T>* listSeq;
};
#endif