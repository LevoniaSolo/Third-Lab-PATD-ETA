#include <iostream>
#include <functional>
#include <string>
#include <ctime>
#include "Queue.hpp"
#include "Stack.hpp"
#include "Deque.hpp"
#include "Person.hpp"

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
            std::cout << temp.Get(i).toString() << " | ";
        }
    } else if (auto* queue = dynamic_cast<const Queue<T>*>(seq)) {
        Queue<T> copy(*queue);
        int length = copy.GetLength();
        for (int i = 0; i < length; i++) {
            std::cout << copy.Dequeue().toString() << " | ";
        }
    } else if (auto* deque = dynamic_cast<const Deque<T>*>(seq)) {
        Deque<T> copy(*deque);
        int length = copy.GetLength();
        for (int i = 0; i < length; i++) {
            std::cout << copy.PopFront().toString() << " | ";
        }
    }
    std::cout << std::endl;
}

// Функции для SplitIntoTwo, Map, Where, Reduce
bool hasLongId(const Person& p) {
    return p.GetID().length() > 5; // Пример: ID длиннее 5 символов
}

Person toUpperFirstName(const Person& p) {
    std::string firstName = p.GetFirstName();
    for (char& c : firstName) {
        c = std::toupper(c);
    }
    return Person(p.GetID(), firstName, p.GetMiddleName(), p.GetLastName(), p.GetBirthDate());
}

bool lastNameIsSmith(const Person& p) {
    return p.GetLastName() == "Smith";
}

Person concatIds(const Person& acc, const Person& p) {
    return Person(p.GetID() + ";" + acc.GetID(), acc.GetFirstName(), acc.GetMiddleName(), acc.GetLastName(), acc.GetBirthDate());
}

// Тесты для Queue
void testQueue() {
    std::cout << "=== Testing Queue ===" << std::endl;

    // Создаём тестовые объекты Person
    Person p1("ID001", "John", "A", "Smith", time(nullptr));
    Person p2("ID00234", "Jane", "B", "Doe", time(nullptr));
    Person p3("ID003", "Alice", "C", "Smith", time(nullptr));
    Person p4("ID00456", "Bob", "D", "Brown", time(nullptr));
    Person p5("ID005", "Eve", "E", "Davis", time(nullptr));
    Person p6("ID00678", "Tom", "F", "Wilson", time(nullptr));

    // Тест Enqueue и GetFirst
    Queue<Person> q1;
    q1.Enqueue(p1);
    q1.Enqueue(p2);
    std::cout << "Enqueue p1, p2; GetFirst: " << (q1.GetFirst() == p1 ? "PASS" : "FAIL") << std::endl;

    // Тест Dequeue
    Person dequeued = q1.Dequeue();
    std::cout << "Dequeue: " << dequeued.toString() << "; GetFirst: " << (q1.GetFirst() == p2 ? "PASS" : "FAIL") << std::endl;

    // Тест GetLength
    std::cout << "GetLength: " << (q1.GetLength() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест AppendSequence
    Queue<Person> q2;
    q2.Enqueue(p3);
    q2.Enqueue(p4);
    q1.AppendSequence(&q2);
    std::cout << "AppendSequence [p3, p4]: ";
    printSequence(&q1, "Result: "); // Ожидаем [p2, p3, p4]

    // Тест SplitIntoTwo
    Queue<Person> q3;
    q3.Enqueue(p1);
    q3.Enqueue(p2);
    q3.Enqueue(p3);
    q3.Enqueue(p4);
    auto [trueQ, falseQ] = q3.SplitIntoTwo(hasLongId);
    std::cout << "SplitIntoTwo by hasLongId on [p1, p2, p3, p4]: " << std::endl;
    printSequence(trueQ, "True: ");  // Ожидаем [p2, p4] (ID00234, ID00456)
    printSequence(falseQ, "False: "); // Ожидаем [p1, p3] (ID001, ID003)
    delete trueQ;
    delete falseQ;

    // Тест Concat
    Queue<Person> q4;
    q4.Enqueue(p5);
    q4.Enqueue(p6);
    Sequence<Person>* concatQ = q3.Concat(&q4);
    std::cout << "Concat [p1, p2, p3, p4] and [p5, p6]: ";
    printSequence(concatQ, "Result: "); // Ожидаем [p1, p2, p3, p4, p5, p6]
    delete concatQ;

    // Тест GetSubsequence
    Sequence<Person>* subQ = q3.GetSubsequence(1, 3);
    std::cout << "GetSubsequence(1, 3) on [p1, p2, p3, p4]: ";
    printSequence(subQ, "Result: "); // Ожидаем [p2, p3]
    delete subQ;

    // Тест Map
    Sequence<Person>* mappedQ = q3.Map(toUpperFirstName);
    std::cout << "Map toUpperFirstName on [p1, p2, p3, p4]: ";
    printSequence(mappedQ, "Result: "); // Ожидаем [JOHN Smith, JANE Doe, ALICE Smith, BOB Brown]
    delete mappedQ;

    // Тест Where
    Sequence<Person>* filteredQ = q3.Where(lastNameIsSmith);
    std::cout << "Where lastNameIsSmith on [p1, p2, p3, p4]: ";
    printSequence(filteredQ, "Result: "); // Ожидаем [p1, p3]
    delete filteredQ;

    // Тест Reduce
    Person concatIdsResult = q3.Reduce(concatIds, Person("INIT", "", "", "", time(nullptr)));
    std::cout << "Reduce concatIds on [p1, p2, p3, p4]: " << (concatIdsResult.GetID() == "INIT;ID001;ID00234;ID003;ID00456" ? "PASS" : "FAIL") << std::endl;

    // Тест ContainsSubsequence
    Queue<Person> q5;
    q5.Enqueue(p2);
    q5.Enqueue(p3);
    bool containsQ = q3.ContainsSubsequence(&q5);
    std::cout << "ContainsSubsequence [p2, p3] in [p1, p2, p3, p4]: " << (containsQ ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

// Тесты для Stack
void testStack() {
    std::cout << "=== Testing Stack ===" << std::endl;

    // Создаём тестовые объекты Person
    Person p1("ID001", "John", "A", "Smith", time(nullptr));
    Person p2("ID00234", "Jane", "B", "Doe", time(nullptr));
    Person p3("ID003", "Alice", "C", "Smith", time(nullptr));
    Person p4("ID00456", "Bob", "D", "Brown", time(nullptr));
    Person p5("ID005", "Eve", "E", "Davis", time(nullptr));
    Person p6("ID00678", "Tom", "F", "Wilson", time(nullptr));

    // Тест Push и Top
    Stack<Person> s1;
    s1.Push(p1);
    s1.Push(p2);
    std::cout << "Push p1, p2; Top: " << (s1.Top() == p2 ? "PASS" : "FAIL") << std::endl;

    // Тест Pop
    Person popped = s1.Pop();
    std::cout << "Pop: " << popped.toString() << "; Top: " << (s1.Top() == p1 ? "PASS" : "FAIL") << std::endl;

    // Тест GetLength
    std::cout << "GetLength: " << (s1.GetLength() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест SplitIntoTwo
    Stack<Person> s2;
    s2.Push(p1);
    s2.Push(p2);
    s2.Push(p3);
    s2.Push(p4);
    auto [trueS, falseS] = s2.SplitIntoTwo(hasLongId);
    std::cout << "SplitIntoTwo by hasLongId on [p1, p2, p3, p4]: " << std::endl;
    printSequence(trueS, "True: ");  // Ожидаем [p2, p4]
    printSequence(falseS, "False: "); // Ожидаем [p1, p3]
    delete trueS;
    delete falseS;

    // Тест Concat
    Stack<Person> s3;
    s3.Push(p5);
    s3.Push(p6);
    Sequence<Person>* concatS = s2.Concat(&s3);
    std::cout << "Concat [p1, p2, p3, p4] and [p5, p6]: ";
    printSequence(concatS, "Result: "); // Ожидаем [p1, p2, p3, p4, p5, p6]
    delete concatS;

    // Тест GetSubsequence
    Sequence<Person>* subS = s2.GetSubsequence(1, 3);
    std::cout << "GetSubsequence(1, 3) on [p1, p2, p3, p4]: ";
    printSequence(subS, "Result: "); // Ожидаем [p2, p3]
    delete subS;

    // Тест Map
    Sequence<Person>* mappedS = s2.Map(toUpperFirstName);
    std::cout << "Map toUpperFirstName on [p1, p2, p3, p4]: ";
    printSequence(mappedS, "Result: "); // Ожидаем [JOHN Smith, JANE Doe, ALICE Smith, BOB Brown]
    delete mappedS;

    // Тест Where
    Sequence<Person>* filteredS = s2.Where(lastNameIsSmith);
    std::cout << "Where lastNameIsSmith on [p1, p2, p3, p4]: ";
    printSequence(filteredS, "Result: "); // Ожидаем [p1, p3]
    delete filteredS;

    // Тест Reduce
    Person concatIdsResult = s2.Reduce(concatIds, Person("INIT", "", "", "", time(nullptr)));
    // std::cout << concatIdsResult.GetID() << std::endl;
    std::cout << "Reduce concatIds on [p1, p2, p3, p4]: " << (concatIdsResult.GetID() == "INIT;ID001;ID00234;ID003;ID00456" ? "PASS" : "FAIL") << std::endl;

    // Тест ContainsSubsequence
    Stack<Person> s4;
    s4.Push(p2);
    s4.Push(p3);
    bool containsS = s2.ContainsSubsequence(&s4);
    std::cout << "ContainsSubsequence [p2, p3] in [p1, p2, p3, p4]: " << (containsS ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

// Тесты для Deque
void testDeque() {
    std::cout << "=== Testing Deque ===" << std::endl;

    // Создаём тестовые объекты Person
    Person p1("ID001", "John", "A", "Smith", time(nullptr));
    Person p2("ID00234", "Jane", "B", "Doe", time(nullptr));
    Person p3("ID003", "Alice", "C", "Smith", time(nullptr));
    Person p4("ID00456", "Bob", "D", "Brown", time(nullptr));
    Person p5("ID005", "Eve", "E", "Davis", time(nullptr));
    Person p6("ID00678", "Tom", "F", "Wilson", time(nullptr));

    // Тест PushFront и GetFirst
    Deque<Person> d1;
    d1.PushFront(p1);
    d1.PushFront(p2);
    std::cout << "PushFront p1, p2; GetFirst: " << (d1.GetFirst() == p2 ? "PASS" : "FAIL") << std::endl;

    // Тест PushBack и GetLast
    d1.PushBack(p3);
    std::cout << "PushBack p3; GetLast: " << (d1.GetLast() == p3 ? "PASS" : "FAIL") << std::endl;

    // Тест PopFront
    Person poppedFront = d1.PopFront();
    std::cout << "PopFront: " << poppedFront.toString() << "; GetFirst: " << (d1.GetFirst() == p1 ? "PASS" : "FAIL") << std::endl;

    // Тест PopBack
    Person poppedBack = d1.PopBack();
    std::cout << "PopBack: " << poppedBack.toString() << "; GetLast: " << (d1.GetLast() == p1 ? "PASS" : "FAIL") << std::endl;

    // Тест GetLength
    std::cout << "GetLength: " << (d1.GetLength() == 1 ? "PASS" : "FAIL") << std::endl;

    // Тест Sort
    Deque<Person> d2;
    d2.PushBack(p3);
    d2.PushBack(p1);
    d2.PushBack(p2);
    // std::cout << (p1.GetID() < p2.GetID()) << std::endl;
    // std::cout << (p1 < p3) << std::endl;
    // std::cout << (p2 < p3) << std::endl;
    // std::cout << (p1.GetID() < p5.GetID()) << std::endl;
    // std::cout << (p2.GetID() < p5.GetID()) << std::endl;
    // std::cout << (p6.GetID() < p5.GetID()) << std::endl;
    Sequence<Person>* sorted = d2.Sort([](const Person& a, const Person& b) { return a < b; });
    std::cout << "Sort [p3, p1, p2]: ";
    printSequence(sorted, "Result: "); // Ожидаем [p1, p3, p2] (ID001, ID003, ID00234)
    delete sorted;

    // Тест Merge
    Deque<Person> d3;
    Deque<Person> d4;
    d3.PushBack(p1);
    d3.PushBack(p3);
    d4.PushBack(p2);
    d4.PushBack(p4);
    Sequence<Person>* merged = d3.Merge(&d4, [](const Person& a, const Person& b) { return a < b; });
    std::cout << "Merge [p1, p3] and [p2, p4]: ";
    printSequence(merged, "Result: "); // Ожидаем [p1, p3, p2, p4] (ID001, ID003, ID00234, ID00456)
    delete merged;

    // Тест Concat
    Sequence<Person>* concatD = d3.Concat(&d4);
    std::cout << "Concat [p1, p3] and [p2, p4]: ";
    printSequence(concatD, "Result: "); // Ожидаем [p1, p3, p2, p4]
    delete concatD;

    // Тест GetSubsequence
    Sequence<Person>* subD = d3.GetSubsequence(0, 2);
    std::cout << "GetSubsequence(0, 2) on [p1, p3]: ";
    printSequence(subD, "Result: "); // Ожидаем [p1, p3]
    delete subD;

    // Тест Map
    Sequence<Person>* mappedD = d3.Map(toUpperFirstName);
    std::cout << "Map toUpperFirstName on [p1, p3]: ";
    printSequence(mappedD, "Result: "); // Ожидаем [JOHN Smith, ALICE Smith]
    delete mappedD;

    // Тест Where
    Sequence<Person>* filteredD = d3.Where(lastNameIsSmith);
    std::cout << "Where lastNameIsSmith on [p1, p3]: ";
    printSequence(filteredD, "Result: "); // Ожидаем [p1, p3]
    delete filteredD;

    // Тест Reduce
    Person concatIdsResult = d3.Reduce(concatIds, Person("INIT", "", "", "", time(nullptr)));
    std::cout << "Reduce concatIds on [p1, p3]: " << (concatIdsResult.GetID() == "INIT;ID001;ID003" ? "PASS" : "FAIL") << std::endl;

    // Тест ContainsSubsequence
    Deque<Person> d5;
    d5.PushBack(p1);
    d5.PushBack(p3);
    bool containsD = d3.ContainsSubsequence(&d5);
    std::cout << "ContainsSubsequence [p1, p3] in [p1, p3]: " << (containsD ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

int main() {
    testQueue();
    testStack();
    testDeque();
    return 0;
}