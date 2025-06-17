#ifndef STACK_HPP
#define STACK_HPP
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <functional>
#include <utility>

template<typename T>
class Stack : public Sequence<T> {
public:
    Stack() : array(new DynamicArray<T>()) {}
    Stack(const T* items, int count) : array(new DynamicArray<T>(items, count)) {}
    Stack(const Stack<T>& other) : array(new DynamicArray<T>(*other.array)) {}
    ~Stack() {
        delete array;
    }

    void Push(T item) {
        array->Append(item);
    }

    T Pop() {
        return array->RemoveLast();
    }

    T Top() const {
        return array->Get(array->GetSize() - 1);
    }

    int GetLength() const override {
        return array->GetSize();
    }

    Stack<T>* Concat(Sequence<T>* other) const override {
        Stack<T>* tmp = new Stack<T>(*this);
        Stack<T>* otherStack = dynamic_cast<Stack<T>*>(other);
        if (otherStack) {
            DynamicArray<T>* concatArray = tmp->array->Concat(otherStack->array);
            // DynamicArray<T>* concatArray = otherStack->array->Concat(tmp->array);

            delete tmp->array;
            tmp->array = concatArray;
        } else {
            for (int i = 0; i < other->GetLength(); i++) {
                tmp->array->Append(other->Get(i));
            }
        }
        return tmp;
    }

    Stack<T>* GetSubsequence(int startIndex, int endIndex) const override {
        DynamicArray<T>* subArray = array->GetSubArray(startIndex, endIndex);
        Stack<T>* tmp = new Stack<T>();
        delete tmp->array;
        tmp->array = subArray;
        return tmp;
    }

    Stack<T>* Map(std::function<T(T)> f) const override {
        Stack<T>* tmp = new Stack<T>(*this);
        DynamicArray<T>* mappedArray = tmp->array->Map(f);
        delete tmp->array;
        tmp->array = mappedArray;
        return tmp;
    }

    Stack<T>* Where(std::function<bool(T)> predicate) const override {
        Stack<T>* tmp = new Stack<T>(*this);
        DynamicArray<T>* filteredArray = tmp->array->Where(predicate);
        delete tmp->array;
        tmp->array = filteredArray;
        return tmp;
    }

    T Reduce(std::function<T(T, T)> f, T initial) const override {
        return array->Reduce(f, initial);
    }

    bool ContainsSubsequence(Sequence<T>* sub) const override {
        Stack<T>* subStack = dynamic_cast<Stack<T>*>(sub);
        if (subStack) {
            // result = true;
            return array->ContainsSubsequence(subStack->array);
        }
        DynamicArray<T>* temp = new DynamicArray<T>();
        for (int i = 0; i < sub->GetLength(); i++) {
            temp->Append(sub->Get(i));
        }
        bool result = array->ContainsSubsequence(temp);
        delete temp;
        return result;
    }

    std::pair<Stack<T>*, Stack<T>*> SplitIntoTwo(std::function<bool(T)> predicate) {
        Stack<T>* trueStack = new Stack<T>();
        Stack<T>* falseStack = new Stack<T>();
        for (int i = 0; i < array->GetSize(); i++) {
            T item = array->Get(i);
            if (predicate(item)) {
                trueStack->Push(item);
            } else {
                falseStack->Push(item);
            }
        }
        return std::make_pair(trueStack, falseStack);
    }

protected:
    DynamicArray<T>* array;

    virtual Stack<T>* Instance() {
        return this;
    }

private:
    // Unsupported Sequence<T> methods
    T Get(int index) const override {
        throw std::logic_error("Get by index is not supported for Stack");
    }

    Sequence<T>* Append(T item) override {
        throw std::logic_error("Append is not supported for Stack; use Push");
    }

    Sequence<T>* Prepend(T item) override {
        throw std::logic_error("Prepend is not supported for Stack");
    }

    Sequence<T>* InsertAt(T item, int index) override {
        throw std::logic_error("InsertAt is not supported for Stack");
    }

    T GetFirst() const override {
        throw std::logic_error("GetFirst is not supported for Stack; use Top");
    }

    T GetLast() const override {
        throw std::logic_error("GetLast is not supported for Stack; use Top");
    }
};

#endif