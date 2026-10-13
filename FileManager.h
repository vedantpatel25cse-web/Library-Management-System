// FileManager.h
// Handles all reading and writing of the data files (text files, '|' separated).
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "Book.h"
#include "Member.h"
#include "Transaction.h"

class FileManager {
private:
    std::string booksFile;
    std::string membersFile;
    std::string transactionsFile;

    static bool fileExists(const std::string& name);

public:
    FileManager(const std::string& booksFile = "books.txt",
                const std::string& membersFile = "members.txt",
                const std::string& transactionsFile = "transactions.txt");

    // ---- save: write the first 'count' objects of the array to disk ----
    void saveBooks(const Book* books, int count) const;
    void saveMembers(const Member* members, int count) const;
    void saveTransactions(const Transaction* txns, int count) const;

    // ---- load: allocate a NEW dynamic array (new[]), delete[] the old one,
    //      and hand the new array back through the reference parameters.
    //      Returns false (array untouched) if the file does not exist. ----
    bool loadBooks(Book*& books, int& count, int& capacity) const;
    bool loadMembers(Member*& members, int& count, int& capacity) const;
    bool loadTransactions(Transaction*& txns, int& count, int& capacity) const;

    std::string getBooksFile() const        { return booksFile; }
    std::string getMembersFile() const      { return membersFile; }
    std::string getTransactionsFile() const { return transactionsFile; }
};

#endif
