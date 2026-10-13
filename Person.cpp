// Person.cpp
#include "Person.h"
#include "LibraryException.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>

Person::Person() : id(0), name(""), phone("") {}

Person::Person(int id, const std::string& name, const std::string& phone) : id(0) {
    if (id <= 0) throw LibraryException("ID must be a positive number.");
    this->id = id;
    setName(name);
    setPhone(phone);
}

Person::~Person() {}

void Person::setName(const std::string& n) {
    if (n.empty()) throw LibraryException("Name cannot be empty.");
    name = n;
}

void Person::setPhone(const std::string& p) {
    if (!Utils::isValidPhone(p)) throw LibraryException("Phone must contain exactly 10 digits.");
    phone = p;
}

void Person::printCore() const {
    std::cout << std::left << std::setw(7) << id
              << std::setw(24) << Utils::fit(name, 22)
              << std::setw(13) << phone;
}

void Person::display() const {
    printCore();
    std::cout << "\n";
}
