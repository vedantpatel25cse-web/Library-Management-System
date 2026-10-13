// Member.cpp
#include "Member.h"
#include "LibraryException.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>

Member::Member() : Person(), email(""), borrowedCount(0) {}

Member::Member(int memberId, const std::string& name, const std::string& phone,
               const std::string& email, int borrowedCount)
    : Person(memberId, name, phone), borrowedCount(0) {
    setEmail(email);
    if (borrowedCount < 0 || borrowedCount > MAX_BORROW)
        throw LibraryException("Invalid borrowed-book count for member.");
    this->borrowedCount = borrowedCount;
}

void Member::setEmail(const std::string& e) {
    if (!Utils::isValidEmail(e)) throw LibraryException("Invalid email address.");
    email = e;
}

void Member::printHeader() {
    std::cout << std::left << std::setw(7) << "ID" << std::setw(24) << "Name"
              << std::setw(13) << "Phone" << std::setw(32) << "Email" << "Borrowed\n"
              << std::string(83, '-') << "\n";
}

void Member::display() const {
    printCore();                                   // reuse the base-class part of the row
    std::cout << std::setw(32) << Utils::fit(email, 30)
              << borrowedCount << "/" << MAX_BORROW << "\n";
}

void Member::borrowBook() {
    if (!canBorrow())
        throw LibraryException("Borrow limit reached (maximum " + std::to_string(MAX_BORROW) + " books).");
    ++borrowedCount;
}

void Member::returnBook() {
    if (borrowedCount <= 0) throw LibraryException("This member has no borrowed books.");
    --borrowedCount;
}
