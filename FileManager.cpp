// FileManager.cpp
#include "FileManager.h"
#include "LibraryException.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace {

std::vector<std::string> split(const std::string& line, char sep) {
    std::vector<std::string> parts;
    std::string item;
    std::istringstream in(line);
    while (std::getline(in, item, sep)) parts.push_back(item);
    if (!line.empty() && line[line.size() - 1] == sep) parts.push_back("");
    return parts;
}

int toInt(const std::string& s) {
    std::size_t used = 0;
    int v = std::stoi(s, &used);          // throws std::exception on bad data
    if (used != s.size()) throw LibraryException("Bad number in data file: " + s);
    return v;
}

int newCapacity(std::size_t n) { return n < 10 ? 10 : static_cast<int>(n) * 2; }

// Copy a vector into a freshly allocated dynamic array and replace 'arr'.
template <typename T>
void adopt(const std::vector<T>& items, T*& arr, int& count, int& capacity) {
    int cap = newCapacity(items.size());
    T* fresh = new T[cap];
    for (std::size_t i = 0; i < items.size(); ++i) fresh[i] = items[i];
    delete[] arr;                          // release the old array
    arr = fresh;
    count = static_cast<int>(items.size());
    capacity = cap;
}

void reportSkipped(const std::string& file, int skipped) {
    if (skipped > 0)
        std::cout << "  Warning: " << skipped << " damaged line(s) skipped in " << file << ".\n";
}

}  // namespace

FileManager::FileManager(const std::string& b, const std::string& m, const std::string& t)
    : booksFile(b), membersFile(m), transactionsFile(t) {}

bool FileManager::fileExists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

// ------------------------------------------------------------------ save
void FileManager::saveBooks(const Book* books, int count) const {
    std::ofstream out(booksFile.c_str());
    if (!out) throw LibraryException("Cannot open file for writing: " + booksFile);
    for (int i = 0; i < count; ++i)
        out << books[i].getBookId() << '|' << books[i].getTitle() << '|' << books[i].getAuthor() << '|'
            << books[i].getCategory() << '|' << (books[i].isAvailable() ? 1 : 0) << '\n';
    if (!out) throw LibraryException("Error while writing: " + booksFile);
}

void FileManager::saveMembers(const Member* members, int count) const {
    std::ofstream out(membersFile.c_str());
    if (!out) throw LibraryException("Cannot open file for writing: " + membersFile);
    for (int i = 0; i < count; ++i)
        out << members[i].getMemberId() << '|' << members[i].getName() << '|' << members[i].getPhone() << '|'
            << members[i].getEmail() << '|' << members[i].getBorrowedCount() << '\n';
    if (!out) throw LibraryException("Error while writing: " + membersFile);
}

void FileManager::saveTransactions(const Transaction* txns, int count) const {
    std::ofstream out(transactionsFile.c_str());
    if (!out) throw LibraryException("Cannot open file for writing: " + transactionsFile);
    for (int i = 0; i < count; ++i)
        out << txns[i].getTransactionId() << '|' << txns[i].getBookId() << '|' << txns[i].getMemberId() << '|'
            << txns[i].getIssueDate() << '|' << (txns[i].isReturned() ? txns[i].getReturnDate() : "-") << '\n';
    if (!out) throw LibraryException("Error while writing: " + transactionsFile);
}

// ------------------------------------------------------------------ load
bool FileManager::loadBooks(Book*& books, int& count, int& capacity) const {
    if (!fileExists(booksFile)) return false;
    std::ifstream in(booksFile.c_str());
    std::vector<Book> items;
    std::string line;
    int skipped = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            std::vector<std::string> f = split(line, '|');
            if (f.size() != 5) throw LibraryException("wrong field count");
            items.push_back(Book(toInt(f[0]), f[1], f[2], f[3], toInt(f[4]) != 0));
        } catch (const std::exception&) { ++skipped; }
    }
    reportSkipped(booksFile, skipped);
    adopt(items, books, count, capacity);
    return true;
}

bool FileManager::loadMembers(Member*& members, int& count, int& capacity) const {
    if (!fileExists(membersFile)) return false;
    std::ifstream in(membersFile.c_str());
    std::vector<Member> items;
    std::string line;
    int skipped = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            std::vector<std::string> f = split(line, '|');
            if (f.size() != 5) throw LibraryException("wrong field count");
            items.push_back(Member(toInt(f[0]), f[1], f[2], f[3], toInt(f[4])));
        } catch (const std::exception&) { ++skipped; }
    }
    reportSkipped(membersFile, skipped);
    adopt(items, members, count, capacity);
    return true;
}

bool FileManager::loadTransactions(Transaction*& txns, int& count, int& capacity) const {
    if (!fileExists(transactionsFile)) return false;
    std::ifstream in(transactionsFile.c_str());
    std::vector<Transaction> items;
    std::string line;
    int skipped = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            std::vector<std::string> f = split(line, '|');
            if (f.size() != 5) throw LibraryException("wrong field count");
            items.push_back(Transaction(toInt(f[0]), toInt(f[1]), toInt(f[2]), f[3], f[4] == "-" ? "" : f[4]));
        } catch (const std::exception&) { ++skipped; }
    }
    reportSkipped(transactionsFile, skipped);
    adopt(items, txns, count, capacity);
    return true;
}
