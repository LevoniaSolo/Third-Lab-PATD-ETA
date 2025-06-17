#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <ctime>

using PersonID = std::string;

class Person {
private:
    PersonID id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    time_t birthDate;

public:
    // Конструктор по умолчанию
    Person() : id("UNKNOWN"), firstName(""), middleName(""), lastName(""), birthDate(0) {}

    // Основной конструктор
    Person(PersonID id, std::string firstName, std::string middleName, std::string lastName, time_t birthDate)
        : id(id), firstName(firstName), middleName(middleName), lastName(lastName), birthDate(birthDate) {}

    PersonID GetID() const { return id; }
    std::string GetFirstName() const { return firstName; }
    std::string GetMiddleName() const { return middleName; }
    std::string GetLastName() const { return lastName; }
    time_t GetBirthDate() const { return birthDate; }

    bool operator<(const Person& other) const {
        return std::stoi(id.substr(2)) < std::stoi(other.id.substr(2));
    }

    bool operator==(const Person& other) const {
        return id == other.id &&
               firstName == other.firstName &&
               middleName == other.middleName &&
               lastName == other.lastName &&
               birthDate == other.birthDate;
    }

    bool operator!=(const Person& other) const {
        return !(*this == other); // Используем operator== для простоты
    }

    std::string toString() const {
        return id + " " + firstName + " " + middleName + " " + lastName;
    }
};

#endif