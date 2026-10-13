// Library.h
// The "system class": owns the dynamic arrays of Book, Member and Transaction
// objects and implements every menu operation.
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Book.h"
#include "Member.h"
#include "Person.h"
#include "Transaction.h"
#include "FileManager.h"

class Library {
private:
    Book*        books;          // dynamic array of books
    int          bookCount;
    int          bookCapacity;

    Member*      members;        // dynamic array of members
    int          memberCount;
    int          memberCapacity;

    Transaction* transactions;   // dynamic array of transactions
    int          txnCount;
    int          txnCapacity;
    int          nextTransactionId;

    FileManager  fileMgr;

    // The object owns raw memory, so copying is forbidden (avoids double delete[]).
    Library(const Library&);
    Library& operator=(const Library&);

    int  findBookIndex(int bookId) const;
    int  findMemberIndex(int memberId) const;
    int  findOpenTransaction(int bookId) const;
    int  pickEntity(const std::string& heading, bool withTransactions) const;
    void printPerson(const Person& p) const;            // polymorphic call p.display()

    // sub-operations used by the menu handlers
    void addBook();
    void addMember();
    void updateBook();
    void updateMember();
    void deleteBook();
    void deleteMember();
    void searchMember();

public:
    explicit Library(int initialCapacity = 10);
    ~Library();

    // ---- one function per main-menu option ----
    void addRecord();          // 1
    void displayRecords();     // 2
    void searchRecord();       // 3
    void updateRecord();       // 4
    void deleteRecord();       // 5
    void transactionMenu();    // 6  (issue / return)
    void report() const;       // 7
    void saveLoadMenu();       // 8

    // ---- core operations named in the blueprint ----
    void searchBook();
    void issueBook();
    void returnBook();

    // ---- persistence ----
    void saveAll() const;
    void loadAll(bool quiet = false);
};

#endif
