#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <string>
#include <functional>

namespace Functions {
// Вспомогательные функции для тестирования
inline int inc(int x, int n) { return x + n; }

// Создание тестовых функций
inline std::function<int(int)> makeIncFunction(int n) {
    return [n](int x) { return inc(x, n); };
}
} // namespace Functions

class FunctionPtr {
private:
    std::string id; // Идентификатор, например, "FID001"
    std::string name; // Имя функции, например, "inc1"
    std::function<int(int)> func; // Функция, которую можно вызывать

public:
    // Конструктор по умолчанию
    FunctionPtr() : id("FID000"), name("default"), func([](int x) { return x; }) {}

    // Основной конструктор
    FunctionPtr(const std::string& id, const std::string& name, std::function<int(int)> f)
        : id(id), name(name), func(f) {}

    // Получение идентификатора
    std::string GetID() const { return id; }

    // Получение имени
    std::string GetName() const { return name; }

    // Вызов функции
    int operator()(int x) const { return func(x); }

    // Сравнение по числовому значению ID (аналог Person::operator<)
    bool operator<(const FunctionPtr& other) const {
        return std::stoi(id.substr(3)) < std::stoi(other.id.substr(3));
    }

    // Равенство (по id, name и поведению функции)
    bool operator==(const FunctionPtr& other) const {
        return id == other.id && name == other.name;
    }

    // Неравенство
    bool operator!=(const FunctionPtr& other) const {
        return !(*this == other);
    }

    // Строковое представление
    std::string toString() const {
        return id + " " + name;
    }
};

#endif