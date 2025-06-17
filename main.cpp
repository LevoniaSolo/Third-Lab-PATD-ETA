#include <iostream>
#include <functional>
#include "Queue.hpp"
#include "Stack.hpp"
#include "Deque.hpp"

// Вспомогательная функция для вывода последовательности
template<typename T>
void printSequence(const Sequence<T>* seq, const std::string& prefix) {
    std::cout << prefix;
    if (!seq || seq->GetLength() == 0) {
        std::cout << "empty" << std::endl;
        return;
    }
    if (auto* stack = dynamic_cast<const Stack<T>*>(seq)) {
        DynamicArray<T> temp;
        Stack<T> copy(*stack);
        int length = copy.GetLength();
        for (int i = 0; i < length; i++) {
            temp.Prepend(copy.Pop()); // Сохраняем в обратном порядке
        }
        for (int i = 0; i < length; i++) {
            std::cout << temp.Get(i) << " ";
        }
    } else if (auto* queue = dynamic_cast<const Queue<T>*>(seq)) {
        Queue<T> copy(*queue);
        int length = copy.GetLength();
        for (int i = 0; i < length; i++) {
            std::cout << copy.Dequeue() << " ";
        }
    } else if (auto* deque = dynamic_cast<const Deque<T>*>(seq)) {
        Deque<T> copy(*deque);
        int length = copy.GetLength();
        for (int i = 0; i < length; i++) {
            std::cout << copy.PopFront() << " ";
        }
    }
    std::cout << std::endl;
}

// Вспомогательная функция для SplitIntoTwo
bool isEven(int x) {
    return x % 2 == 0;
}

// Функции для Map, Reduce, Where
int square(int x) { return x * x; }
bool greaterThanTwo(int x) { return x > 2; }
int sum(int a, int b) { return a + b; }

// Тесты для Queue
void testQueue() {
    std::cout << "=== Testing Queue ===" << std::endl;

    // Тест Enqueue и GetFirst
    Queue<int> q1;
    q1.Enqueue(1);
    q1.Enqueue(2);
    std::cout << "Enqueue 1, 2; GetFirst: " << (q1.GetFirst() == 1 ? "PASS" : "FAIL") << std::endl;
    // std::cout << "Enqueue 1, 2; GetFirst: " << std::endl;

    // Тест Dequeue
    int dequeued = q1.Dequeue();
    std::cout << "Dequeue: " << dequeued << "; GetFirst: " << (q1.GetFirst() == 2 ? "PASS" : "FAIL") << std::endl;

    // Тест GetLength
    std::cout << "GetLength: " << (q1.GetLength() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест AppendSequence
    Queue<int> q2;
    q2.Enqueue(3);
    q2.Enqueue(4);
    q1.AppendSequence(&q2);
    
    std::cout << "AppendSequence [3, 4]: ";
    std::cout <<"smth"<< std::endl;
    printSequence(&q1, "Result: "); // Ожидаем [2, 3, 4]

    // Тест SplitIntoTwo
    Queue<int> q3;
    q3.Enqueue(1);
    q3.Enqueue(2);
    q3.Enqueue(3);
    q3.Enqueue(4);
    auto [trueQ, falseQ] = q3.SplitIntoTwo(isEven);
    std::cout << "SplitIntoTwo by isEven on [1, 2, 3, 4]: " << std::endl;
    printSequence(trueQ, "True: ");  // Ожидаем [2, 4]
    printSequence(falseQ, "False: "); // Ожидаем [1, 3]
    delete trueQ;
    delete falseQ;

    // Тест Concat
    Queue<int> q4;
    q4.Enqueue(5);
    q4.Enqueue(6);
    Sequence<int>* concatQ = q3.Concat(&q4);
    std::cout << "Concat [1, 2, 3, 4] and [5, 6]: ";
    printSequence(concatQ, "Result: "); // Ожидаем [1, 2, 3, 4, 5, 6]
    delete concatQ;

    // Тест GetSubsequence
    Sequence<int>* subQ = q3.GetSubsequence(1, 3);
    std::cout << "GetSubsequence(1, 3) on [1, 2, 3, 4]: ";
    printSequence(subQ, "Result: "); // Ожидаем [2, 3]
    delete subQ;

    // Тест Map
    Sequence<int>* mappedQ = q3.Map(square);
    std::cout << "Map square on [1, 2, 3, 4]: ";
    printSequence(mappedQ, "Result: "); // Ожидаем [1, 4, 9, 16]
    delete mappedQ;

    // Тест Where
    Sequence<int>* filteredQ = q3.Where(greaterThanTwo);
    std::cout << "Where >2 on [1, 2, 3, 4]: ";
    printSequence(filteredQ, "Result: "); // Ожидаем [3, 4]
    delete filteredQ;

    // Тест Reduce
    int sumQ = q3.Reduce(sum, 0);
    std::cout << "Reduce sum on [1, 2, 3, 4]: " << (sumQ == 10 ? "PASS" : "FAIL") << std::endl;

    // Тест ContainsSubsequence
    Queue<int> q5;
    q5.Enqueue(2);
    q5.Enqueue(3);
    bool containsQ = q3.ContainsSubsequence(&q5);
    std::cout << "ContainsSubsequence [2, 3] in [1, 2, 3, 4]: " << (containsQ ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

// Тесты для Stack
void testStack() {
    std::cout << "=== Testing Stack ===" << std::endl;

    // Тест Push и Top
    Stack<int> s1;
    s1.Push(1);
    s1.Push(2);
    std::cout << "Push 1, 2; Top: " << (s1.Top() == 2 ? "PASS" : "FAIL") << std::endl;
    Sequence<int>* c = dynamic_cast<Sequence<int>*>(&s1);
    printSequence(c, "Result: ");

    // Тест Pop
    int popped = s1.Pop();
    std::cout << "Pop: " << popped << "; Top: " << (s1.Top() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест GetLength
    std::cout << "GetLength: " << (s1.GetLength() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест SplitIntoTwo
    Stack<int> s2;
    s2.Push(1);
    s2.Push(2);
    s2.Push(3);
    s2.Push(4);
    auto [trueS, falseS] = s2.SplitIntoTwo(isEven);
    std::cout << "SplitIntoTwo by isEven on [1, 2, 3, 4]: " << std::endl;
    printSequence(trueS, "True: ");  // Ожидаем [2, 4]
    printSequence(falseS, "False: "); // Ожидаем [1, 3]
    delete trueS;
    delete falseS;

    // Тест Concat
    Stack<int> s3;
    s3.Push(5);
    s3.Push(6);
    Sequence<int>* concatS = s2.Concat(&s3);
    std::cout << "Concat [1, 2, 3, 4] and [5, 6]: ";
    printSequence(concatS, "Result: "); // Ожидаем [1, 2, 3, 4, 5, 6]
    delete concatS;

    // Тест GetSubsequence
    Sequence<int>* subS = s2.GetSubsequence(1, 3);
    std::cout << "GetSubsequence(1, 3) on [1, 2, 3, 4]: ";
    printSequence(subS, "Result: "); // Ожидаем [2, 3]
    delete subS;

    // Тест Map
    Sequence<int>* mappedS = s2.Map(square);
    std::cout << "Map square on [1, 2, 3, 4]: ";
    printSequence(mappedS, "Result: "); // Ожидаем [1, 4, 9, 16]
    delete mappedS;

    // Тест Where
    Sequence<int>* filteredS = s2.Where(greaterThanTwo);
    std::cout << "Where >2 on [1, 2, 3, 4]: ";
    printSequence(filteredS, "Result: "); // Ожидаем [3, 4]
    delete filteredS;

    // Тест Reduce
    int sumS = s2.Reduce(sum, 0);
    std::cout << "Reduce sum on [1, 2, 3, 4]: " << (sumS == 10 ? "PASS" : "FAIL") << std::endl;

    // Тест ContainsSubsequence
    Stack<int> s4;
    s4.Push(2);
    s4.Push(3);
    bool containsS = s2.ContainsSubsequence(&s4);
    std::cout << "ContainsSubsequence [2, 3] in [1, 2, 3, 4]: " << (containsS ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

// Тесты для Deque
void testDeque() {
    std::cout << "=== Testing Deque ===" << std::endl;

    // Тест PushFront и GetFirst
    Deque<int> d1;
    d1.PushFront(1);
    d1.PushFront(2);
    std::cout << "PushFront 1, 2; GetFirst: " << (d1.GetFirst() == 2 ? "PASS" : "FAIL") << std::endl;

    // Тест PushBack и GetLast
    d1.PushBack(3);
    std::cout << "PushBack 3; GetLast: " << (d1.GetLast() == 3 ? "PASS" : "FAIL") << std::endl;

    // Тест PopFront
    int poppedFront = d1.PopFront();
    std::cout << "PopFront: " << poppedFront << "; GetFirst: " << (d1.GetFirst() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест PopBack
    int poppedBack = d1.PopBack();
    std::cout << "PopBack: " << poppedBack << "; GetLast: " << (d1.GetLast() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест GetLength
    std::cout << "GetLength: " << (d1.GetLength() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест Sort
    Deque<int> d2;
    d2.PushBack(3);
    d2.PushBack(1);
    d2.PushBack(2);
    // Sequence<int>* sor = dynamic_cast<Sequence<int>*>(&d2);
    // printSequence(sor, "Result: ");
    Sequence<int>* sorted = d2.Sort([](int a, int b) { return a < b; });
    std::cout << "Sort [3, 1, 2]: ";
    printSequence(sorted, "Result: "); // Ожидаем [1, 2, 3]
    delete sorted;

    // Тест Merge
    Deque<int> d3, d4;
    d3.PushBack(1);
    d3.PushBack(3);
    d4.PushBack(2);
    d4.PushBack(4);
    Sequence<int>* merged = d3.Merge(&d4, [](int a, int b) { return a < b; });
    std::cout << "Merge [1, 3] and [2, 4]: ";
    printSequence(merged, "Result: "); // Ожидаем [1, 2, 3, 4]
    delete merged;

    // Тест Concat
    Sequence<int>* concatD = d3.Concat(&d4);
    std::cout << "Concat [1, 3] and [2, 4]: ";
    printSequence(concatD, "Result: "); // Ожидаем [1, 3, 2, 4]
    delete concatD;

    // Тест GetSubsequence
    Sequence<int>* subD = d3.GetSubsequence(0, 2);
    std::cout << "GetSubsequence(0, 2) on [1, 3]: ";
    printSequence(subD, "Result: "); // Ожидаем [1, 3]
    delete subD;

    // Тест Map
    Sequence<int>* mappedD = d3.Map(square);
    std::cout << "Map square on [1, 3]: ";
    printSequence(mappedD, "Result: "); // Ожидаем [1, 9]
    delete mappedD;

    // Тест Where
    Sequence<int>* filteredD = d3.Where(greaterThanTwo);
    std::cout << "Where >2 on [1, 3]: ";
    printSequence(filteredD, "Result: "); // Ожидаем [3]
    delete filteredD;

    // Тест Reduce
    int sumD = d3.Reduce(sum, 0);
    std::cout << "Reduce sum on [1, 3]: " << (sumD == 4 ? "PASS" : "FAIL") << std::endl;

    // Тест ContainsSubsequence
    Deque<int> d5;
    d5.PushBack(1);
    d5.PushBack(3);
    bool containsD = d3.ContainsSubsequence(&d5);
    std::cout << "ContainsSubsequence [1, 3] in [1, 3]: " << (containsD ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

int main() {
    testQueue();
    testStack();
    testDeque();
    return 0;
}