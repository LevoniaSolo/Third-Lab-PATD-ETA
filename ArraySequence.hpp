#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP
#include "Sequence.hpp"
#include "DynamicArray.hpp"

template<typename T>
class ArraySequence: public Sequence<T>{
public:
    ArraySequence(const T* items, int count) {
        arraySeq = new DynamicArray<T>(items, count);
    }

    ArraySequence() : ArraySequence<T>(nullptr, 0) {}

    ArraySequence(int size) {
        arraySeq = new DynamicArray<T>(size);
    }

    ArraySequence(const ArraySequence<T>& other) {
        arraySeq = new DynamicArray<T>(*(other.arraySeq));
    }

    ArraySequence(const DynamicArray<T>& other) {
        arraySeq = new DynamicArray<T>(other);
    }

    ~ArraySequence() {
        delete arraySeq;
    }

    T GetFirst() const override{
        return arraySeq->Get(0);
    }

    T GetLast() const override{
        return arraySeq->Get(arraySeq->GetSize()-1);
    }

    T Get(int index) const override{
        return arraySeq->Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) override{
        DynamicArray<T>* subArray = arraySeq->GetSubArray(startIndex, endIndex);
        ArraySequence<T>* tmp = new ArraySequence<T>(*subArray);
        delete subArray;
        return tmp;
        // return new ArraySequence<T>(*arraySeq->GetSubArray(startIndex, endIndex));
    }

    ArraySequence<T>* ArrayGetSubsequence(int startIndex, int endIndex) {
        return dynamic_cast<ArraySequence<T>*>(GetSubsequence(startIndex, endIndex));
    }

    int GetLength() const override{
        return arraySeq->GetSize();
    }

    // Sequence<T>* Append(T item) override{
    //     Sequence<T>* tmp = Instance();
    //     Instance()->arraySeq->Append(item);
    //     return tmp;
    // }
    Sequence<T>* Append(T item) override {
        ArraySequence<T>* tmp = Instance();
        tmp->arraySeq->Append(item);
        return tmp;
    }

    ArraySequence<T>* ArrayAppend(T item){
        return dynamic_cast<ArraySequence<T>*>(Instance()->Append(item));
    }

    // Sequence<T>* Prepend(T item) override{
    //     Sequence<T>* tmp = Instance();
    //     Instance()->arraySeq->Prepend(item);
    //     return tmp;
    // }
    Sequence<T>* Prepend(T item) override {
        ArraySequence<T>* tmp = Instance();
        tmp->arraySeq->Prepend(item);
        return tmp;
    }

    ArraySequence<T>* ArrayPrepend(T item){
        return dynamic_cast<ArraySequence<T>*>(Instance()->Prepend(item));
    }

    // Sequence<T>*InsertAt(T item, int index){
    //     ArraySequence<T>* ar = ArraySequence(Instance());
    //     Sequence<T>* tmp = ar;
    //     ar->arraySeq = Instance()->arraySeq->InsertAt(item, index);
    //     return tmp;
    // }
    Sequence<T>* InsertAt(T item, int index) override {
        ArraySequence<T>* tmp = Instance();
        tmp->arraySeq->InsertAt(item, index);
        return tmp;
    }

    ArraySequence<T>* ArrayInsertAt(T item, int index){
        return dynamic_cast<ArraySequence<T>*>(Instance()->InsertAt(item, index));
    }

    Sequence<T>* Concat(Sequence<T>* other) override {
        ArraySequence<T>* tmp = Instance();
        ArraySequence<T>* otherArray = dynamic_cast<ArraySequence<T>*>(other);
        if (otherArray) {
            DynamicArray<T>* concatArray = tmp->arraySeq->Concat(otherArray->arraySeq);
            delete tmp->arraySeq;
            tmp->arraySeq = concatArray;
        } else {
            for (int i = 0; i < other->GetLength(); i++) {
                tmp->arraySeq->Append(other->Get(i));
            }
        }
        return tmp;
    }

    ArraySequence<T>* ArrayConcat(ArraySequence <T> *other){
        return dynamic_cast<ArraySequence<T>*>(Instance()->Concat(other));
    }    

    virtual ArraySequence<T>* Instance(){
        return this;
    }

    Sequence<T>* Map(std::function<T(T)> f) override {
        ArraySequence<T>* tmp = Instance();
        DynamicArray<T>* mappedArray = tmp->arraySeq->Map(f);
        delete tmp->arraySeq;
        tmp->arraySeq = mappedArray;
        return tmp;
    }

    Sequence<T>* Where(std::function<bool(T)> predicate) override {
        ArraySequence<T>* tmp = Instance();
        DynamicArray<T>* filteredArray = tmp->arraySeq->Where(predicate);
        delete tmp->arraySeq;
        tmp->arraySeq = filteredArray;
        return tmp;
    }

    T Reduce(std::function<T(T, T)> f, T initial) override {
        return arraySeq->Reduce(f, initial);
    }

    bool ContainsSubsequence(Sequence<T>* sub) override {
        ArraySequence<T>* subArray = dynamic_cast<ArraySequence<T>*>(sub);
        if (subArray) {
            return arraySeq->ContainsSubsequence(subArray->arraySeq);
        }
        DynamicArray<T>* temp = new DynamicArray<T>(sub->GetLength());
        for (int i = 0; i < sub->GetLength(); i++) {
            temp->Set(sub->Get(i), i);
        }
        bool result = arraySeq->ContainsSubsequence(temp);
        delete temp;
        return result;
    }
    
protected:
    DynamicArray<T>* arraySeq;

};

#endif