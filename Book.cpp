// Book.cpp
#include "Book.h"
#include "LibraryException.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>

Book::Book() : bookId(0), title(""), author(""), category(""), available(true) {}

Book::Book(int bookId, const std::string& title, const std::string& author,
           const std::string& category, bool available)
    : bookId(0), available(available) {
    if (bookId <= 0) throw LibraryException("Book ID must be a positive number.");
    this->bookId = bookId;
    setTitle(title);
    setAuthor(author);
    setCategory(category);
}

void Book::setTitle(const std::string& t) {
    if (t.empty()) throw LibraryException("Book title cannot be empty.");
    title = t;
}

void Book::setAuthor(const std::string& a) {
    if (a.empty()) throw LibraryException("Author name cannot be empty.");
    author = a;
}

void Book::setCategory(const std::string& c) {
    if (c.empty()) throw LibraryException("Category cannot be empty.");
    category = c;
}

void Book::printHeader() {
    std::cout << std::left << std::setw(7) << "ID" << std::setw(32) << "Title"
              << std::setw(24) << "Author" << std::setw(16) << "Category" << "Status\n"
              << std::string(87, '-') << "\n";
}

void Book::display() const {
    std::cout << std::left << std::setw(7) << bookId
              << std::setw(32) << Utils::fit(title, 30)
              << std::setw(24) << Utils::fit(author, 22)
              << std::setw(16) << Utils::fit(category, 14)
              << (available ? "Available" : "Issued") << "\n";
}

void Book::issue() {
    if (!available) throw LibraryException("Book is already issued.");
    available = false;
}

void Book::returnBook() {
    if (available) throw LibraryException("Book was not issued.");
    available = true;
}
