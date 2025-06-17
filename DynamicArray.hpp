#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP
#include <stdlib.h>
#include <iostream>
#include <cassert>

template<typename T>
class DynamicArray {
public:
    //Конструкторы
    
    DynamicArray(): data(nullptr), size(0) {}

    DynamicArray(int newSize) : data(nullptr), size(0) {
        if (newSize < 0) {
            throw std::invalid_argument("Size cannot be negative");
        }
        if (newSize > 0) {
            data = new T[newSize]();
            this->size = newSize;
        }
    }

    DynamicArray(const T* items, int count) : data(nullptr), size(0) {
        if (count < 0) {
            throw std::invalid_argument("Count cannot be negative");
        }
        if (count > 0 && items == nullptr) {
            throw std::invalid_argument("Items cannot be null for non-zero count");
        }
        if (count > 0) {
            data = new T[count];
            size = count;
            for (int i = 0; i < count; i++) {
                data[i] = items[i];
            }
        }
    }
    // Обычный конструктор копирования
    DynamicArray(const DynamicArray<T>& other) {
        data = new T[other.size];
        size = other.size;
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // // Конструктор копирования по мув ссылке
    // DynamicArray(DynamicArray<T>&& other) {
    //     data = other.data;
    //     size = other.size;

    //     other.data = nullptr;
    //     other.size = 0;
    // }

    // Обычный оператор присвоения
    DynamicArray& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        if (data) {
            delete []data;
        }

        data = new T[other.size];
        size = other.size;
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }

        return *this;
    }

    // // Оператор присвоения по мув ссылке
    // DynamicArray& operator=(DynamicArray<T>&& other) {
    //     if (this == &other) {
    //         return *this;
    //     }
    //     if (data) {
    //         delete []data;
    //     }

    //     data = other.data;
    //     size = other.size;

    //     other.data = nullptr;
    //     other.size = 0;

    //     return *this;
    // }

    ~DynamicArray() {
        delete[] data;
    }

    //Декомпозиция

    T Get(int index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    int GetSize() const{
        return size;
    }

    DynamicArray<T>* GetSubArray(int startIndex, int endIndex) const {
        if (!data || size <= 0 || startIndex == endIndex){ 
            return new DynamicArray<T>();
        }
        if (startIndex < 0 || startIndex >= size || endIndex > size || startIndex > endIndex){
            throw std::out_of_range("Index out of range");
        }
        T* array = nullptr;
        if (endIndex - startIndex > 0) {
            array = new T[endIndex - startIndex];
        }
        for (int i = startIndex; i < endIndex; i++) {
            array[i - startIndex] = data[i];
        }
        DynamicArray<T>* subArray = new DynamicArray<T>(array, endIndex - startIndex);
        delete[] array;
        return subArray;
    }

    //Операции

    void Set(const T& object, int index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = object;
    }
    
    // void Set(T&& object, int index) {
    //     if (index >= size) {
    //         throw std::out_of_range("Index out of range");
    //     }
    //     data[index] = std::move(object);
    // }

    void Resize(int new_size) {
        if (new_size == size) {
            return;
        }
        if (new_size == 0) {
            delete[] data;
            data = nullptr;
            size = 0;
            return;
        }
        T* new_data = new T[new_size]();
        if (!data) {
            data = new_data;
            size = new_size;
            return;
        }

        int copy_count = std::min(size, new_size);
        for (int i = 0; i < copy_count; i++) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        size = new_size;
    }

    void Append(const T& object) {
        Resize(size + 1);
        data[size - 1] = object;
    }

    void Prepend(const T& object) {
        Resize(size + 1);
        for (int i = size - 1; i > 0; i--) {
            data[i] = data[i - 1];
        }
        data[0] = object;
    }
    
    void InsertAt(const T& value, int index) {
        if (index < 0 || index > size){
            throw std::out_of_range("Index out of range");
        }
        Resize(size + 1); 
        for (int i = size - 1; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;
    }

    DynamicArray<T>* Concat(const DynamicArray<T>* other) const {
        DynamicArray<T>* result = new DynamicArray<T>(*this);
        if (!other || !other->data || other->size <= 0){
            return result;
        }
        result->Resize(result->size + other->size);
        for (int i = 0; i < other->size; i++) {
            result->Set(other->data[i], size + i);
        }
        return result;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // map-reduce
    DynamicArray<T>* Map(std::function<T(T)> f) const {
        DynamicArray<T>* result = new DynamicArray<T>(size);
        if (!data) return result;
        for (int i = 0; i < size; i++) {
            result->data[i] = f(data[i]);
        }
        return result;
    }

    template <typename U>
    DynamicArray<U>* MapTo(std::function<U(T)> f) const {
        DynamicArray<U>* result = new DynamicArray<U>(size);
        if (!data) return result;
        for (int i = 0; i < size; i++) {
            result->data[i] = f(data[i]);
        }
        return result;
    }

    DynamicArray<T>* Where(std::function<bool(T)> f) const {
        DynamicArray<T>* result = new DynamicArray<T>();
        if (!data) return result;
        for (int i = 0; i < size; i++) {
            if (f(data[i])) {
                result->Append(data[i]);
            }
        }
        return result;
    }

    T Reduce(std::function<T(T, T)> f, const T& c) const {
        if (!data || size <= 0) return c;
        T answer = f(data[0], c);
        for (int i = 1; i < size; i++) {
            answer = f(data[i], answer);
        }
        return answer;
    }

    bool ContainsSubsequence(const DynamicArray<T>* sub) const {
        if (!sub || !sub->data || sub->size > size) return false;
        for (int i = 0; i <= size - sub->size; i++) {
            bool match = true;
            for (int j = 0; j < sub->size; j++) {
                if (data[i + j] != sub->data[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }

    //Всякие методы
    T RemoveLast() {
        if (size == 0) {
            throw std::out_of_range("Array is empty");
        }
        T value = data[size - 1];
        Resize(size - 1);
        return value;
    }


    // Пример реализации итератора
    class Iterator {
    public:
        Iterator(const T* iterData, int iterSize, int index = 0)
            : data(iterData), size(iterSize), currentIndex(index) {}
        T operator*() const { return data[currentIndex]; }
        Iterator& operator++() {
            currentIndex++;
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return currentIndex != other.currentIndex;
        }
    private:
        const T* data;
        int size;
        int currentIndex;
    };

    Iterator begin() const { return Iterator(data, size, 0); }
    Iterator end() const { return Iterator(data, size, size); }

private:
    T* data;
    int size;
};

#endif