// Library.cpp
#include "Library.h"
#include "LibraryException.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <map>

namespace {

// Doubles the capacity of a dynamic array and copies the old elements across.
template <typename T>
void growArray(T*& arr, int count, int& capacity) {
    int newCap = capacity * 2;
    T* bigger = new T[newCap];
    for (int i = 0; i < count; ++i) bigger[i] = arr[i];
    delete[] arr;
    arr = bigger;
    capacity = newCap;
}

}  // namespace

// ================================================================== lifecycle
Library::Library(int initialCapacity)
    : books(0), bookCount(0), bookCapacity(initialCapacity > 0 ? initialCapacity : 10),
      members(0), memberCount(0), memberCapacity(initialCapacity > 0 ? initialCapacity : 10),
      transactions(0), txnCount(0), txnCapacity(initialCapacity > 0 ? initialCapacity : 10),
      nextTransactionId(1), fileMgr() {
    books        = new Book[bookCapacity];
    members      = new Member[memberCapacity];
    transactions = new Transaction[txnCapacity];
}

Library::~Library() {
    delete[] books;
    delete[] members;
    delete[] transactions;
}

// ================================================================== helpers
int Library::findBookIndex(int bookId) const {
    for (int i = 0; i < bookCount; ++i)
        if (books[i].getBookId() == bookId) return i;
    return -1;
}

int Library::findMemberIndex(int memberId) const {
    for (int i = 0; i < memberCount; ++i)
        if (members[i].getMemberId() == memberId) return i;
    return -1;
}

int Library::findOpenTransaction(int bookId) const {
    for (int i = 0; i < txnCount; ++i)
        if (transactions[i].getBookId() == bookId && !transactions[i].isReturned()) return i;
    return -1;
}

int Library::pickEntity(const std::string& heading, bool withTransactions) const {
    std::cout << "\n--- " << heading << " ---\n"
              << "1. Books\n2. Members\n";
    if (withTransactions) std::cout << "3. Transactions\n";
    std::cout << "0. Back to main menu\n";
    return Utils::readInt("Choice: ", 0, withTransactions ? 3 : 2);
}

void Library::printPerson(const Person& p) const {
    p.display();      // virtual call: Member::display() runs when p is really a Member
}

// ================================================================== 1. ADD
void Library::addRecord() {
    int c = pickEntity("ADD RECORD", false);
    if (c == 1) addBook();
    else if (c == 2) addMember();
}

void Library::addBook() {
    std::cout << "\n== Add Book ==\n";
    int id = Utils::readInt("Book ID (1-999999): ", 1, 999999);
    if (findBookIndex(id) >= 0) throw LibraryException("A book with ID " + std::to_string(id) + " already exists.");
    std::string title    = Utils::readText("Title: ");
    std::string author   = Utils::readText("Author: ");
    std::string category = Utils::readText("Category: ");

    Book b(id, title, author, category, true);
    if (bookCount == bookCapacity) growArray(books, bookCount, bookCapacity);
    books[bookCount++] = b;
    std::cout << "Book added successfully.\n";
}

void Library::addMember() {
    std::cout << "\n== Add Member ==\n";
    int id = Utils::readInt("Member ID (1-999999): ", 1, 999999);
    if (findMemberIndex(id) >= 0) throw LibraryException("A member with ID " + std::to_string(id) + " already exists.");
    std::string name  = Utils::readText("Name: ");
    std::string phone = Utils::readPhone("Phone (10 digits): ");
    std::string email = Utils::readEmail("Email: ");

    Member m(id, name, phone, email);
    if (memberCount == memberCapacity) growArray(members, memberCount, memberCapacity);
    members[memberCount++] = m;
    std::cout << "Member added successfully.\n";
}

// ================================================================== 2. DISPLAY
void Library::displayRecords() {
    int c = pickEntity("DISPLAY RECORDS", true);
    if (c == 0) return;
    std::cout << "\n";
    if (c == 1) {
        if (bookCount == 0) { std::cout << "No books in the library.\n"; return; }
        Book::printHeader();
        for (int i = 0; i < bookCount; ++i) books[i].display();
        std::cout << "Total books: " << bookCount << "\n";
    } else if (c == 2) {
        if (memberCount == 0) { std::cout << "No members registered.\n"; return; }
        Member::printHeader();
        for (int i = 0; i < memberCount; ++i) {
            Person* p = &members[i];     // base-class pointer to a derived object
            p->display();                // runtime polymorphism
        }
        std::cout << "Total members: " << memberCount << "\n";
    } else {
        if (txnCount == 0) { std::cout << "No transactions recorded.\n"; return; }
        Transaction::printHeader();
        for (int i = 0; i < txnCount; ++i) transactions[i].display();
        std::cout << "Total transactions: " << txnCount << "\n";
    }
}

// ================================================================== 3. SEARCH
void Library::searchRecord() {
    std::cout << "\n--- SEARCH RECORD ---\n"
              << "1. Search book (by ID or keyword)\n"
              << "2. Search member (by ID or name)\n"
              << "0. Back to main menu\n";
    int c = Utils::readInt("Choice: ", 0, 2);
    if (c == 1) searchBook();
    else if (c == 2) searchMember();
}

void Library::searchBook() {
    std::cout << "\n== Search Book ==\n1. By Book ID\n2. By keyword (title / author / category)\n";
    int mode = Utils::readInt("Choice: ", 1, 2);
    if (mode == 1) {
        int id = Utils::readInt("Book ID: ", 1, 999999);
        int idx = findBookIndex(id);
        if (idx < 0) throw LibraryException("No book found with ID " + std::to_string(id) + ".");
        std::cout << "\n";
        Book::printHeader();
        books[idx].display();
    } else {
        std::string key = Utils::toLower(Utils::readText("Keyword: "));
        int found = 0;
        for (int i = 0; i < bookCount; ++i) {
            if (Utils::toLower(books[i].getTitle()).find(key) != std::string::npos ||
                Utils::toLower(books[i].getAuthor()).find(key) != std::string::npos ||
                Utils::toLower(books[i].getCategory()).find(key) != std::string::npos) {
                if (found == 0) { std::cout << "\n"; Book::printHeader(); }
                books[i].display();
                ++found;
            }
        }
        if (found == 0) throw LibraryException("No book matches \"" + key + "\".");
        std::cout << found << " book(s) found.\n";
    }
}

void Library::searchMember() {
    std::cout << "\n== Search Member ==\n1. By Member ID\n2. By name keyword\n";
    int mode = Utils::readInt("Choice: ", 1, 2);
    if (mode == 1) {
        int id = Utils::readInt("Member ID: ", 1, 999999);
        int idx = findMemberIndex(id);
        if (idx < 0) throw LibraryException("No member found with ID " + std::to_string(id) + ".");
        std::cout << "\n";
        Member::printHeader();
        printPerson(members[idx]);
    } else {
        std::string key = Utils::toLower(Utils::readText("Name keyword: "));
        int found = 0;
        for (int i = 0; i < memberCount; ++i) {
            if (Utils::toLower(members[i].getName()).find(key) != std::string::npos) {
                if (found == 0) { std::cout << "\n"; Member::printHeader(); }
                printPerson(members[i]);
                ++found;
            }
        }
        if (found == 0) throw LibraryException("No member matches \"" + key + "\".");
        std::cout << found << " member(s) found.\n";
    }
}

// ================================================================== 4. UPDATE
void Library::updateRecord() {
    int c = pickEntity("UPDATE RECORD", false);
    if (c == 1) updateBook();
    else if (c == 2) updateMember();
}

void Library::updateBook() {
    int id = Utils::readInt("Book ID to update: ", 1, 999999);
    int idx = findBookIndex(id);
    if (idx < 0) throw LibraryException("No book found with ID " + std::to_string(id) + ".");
    std::cout << "\nCurrent record:\n";
    Book::printHeader();
    books[idx].display();
    std::cout << "\nPress Enter to keep the current value.\n";
    std::string t = Utils::readText("New title: ", true);
    std::string a = Utils::readText("New author: ", true);
    std::string c = Utils::readText("New category: ", true);
    if (!t.empty()) books[idx].setTitle(t);
    if (!a.empty()) books[idx].setAuthor(a);
    if (!c.empty()) books[idx].setCategory(c);
    std::cout << "Book updated successfully.\n";
}

void Library::updateMember() {
    int id = Utils::readInt("Member ID to update: ", 1, 999999);
    int idx = findMemberIndex(id);
    if (idx < 0) throw LibraryException("No member found with ID " + std::to_string(id) + ".");
    std::cout << "\nCurrent record:\n";
    Member::printHeader();
    printPerson(members[idx]);
    std::cout << "\nPress Enter to keep the current value.\n";
    std::string n  = Utils::readText("New name: ", true);
    std::string ph = Utils::readPhone("New phone (10 digits): ", true);
    std::string em = Utils::readEmail("New email: ", true);
    if (!n.empty())  members[idx].setName(n);
    if (!ph.empty()) members[idx].setPhone(ph);
    if (!em.empty()) members[idx].setEmail(em);
    std::cout << "Member updated successfully.\n";
}

// ================================================================== 5. DELETE
void Library::deleteRecord() {
    int c = pickEntity("DELETE RECORD", false);
    if (c == 1) deleteBook();
    else if (c == 2) deleteMember();
}

void Library::deleteBook() {
    int id = Utils::readInt("Book ID to delete: ", 1, 999999);
    int idx = findBookIndex(id);
    if (idx < 0) throw LibraryException("No book found with ID " + std::to_string(id) + ".");
    if (!books[idx].isAvailable())
        throw LibraryException("This book is currently issued and cannot be deleted.");
    Book::printHeader();
    books[idx].display();
    if (!Utils::confirm("Delete this book?")) { std::cout << "Deletion cancelled.\n"; return; }
    for (int i = idx; i < bookCount - 1; ++i) books[i] = books[i + 1];   // close the gap
    --bookCount;
    std::cout << "Book deleted successfully.\n";
}

void Library::deleteMember() {
    int id = Utils::readInt("Member ID to delete: ", 1, 999999);
    int idx = findMemberIndex(id);
    if (idx < 0) throw LibraryException("No member found with ID " + std::to_string(id) + ".");
    if (members[idx].getBorrowedCount() > 0)
        throw LibraryException("This member still has borrowed books and cannot be deleted.");
    Member::printHeader();
    printPerson(members[idx]);
    if (!Utils::confirm("Delete this member?")) { std::cout << "Deletion cancelled.\n"; return; }
    for (int i = idx; i < memberCount - 1; ++i) members[i] = members[i + 1];
    --memberCount;
    std::cout << "Member deleted successfully.\n";
}

// ================================================================== 6. TRANSACTION
void Library::transactionMenu() {
    std::cout << "\n--- MAIN TRANSACTION ---\n"
              << "1. Issue a book\n2. Return a book\n0. Back to main menu\n";
    int c = Utils::readInt("Choice: ", 0, 2);
    if (c == 1) issueBook();
    else if (c == 2) returnBook();
}

void Library::issueBook() {
    std::cout << "\n== Issue Book ==\n";
    int memberId = Utils::readInt("Member ID: ", 1, 999999);
    int mIdx = findMemberIndex(memberId);
    if (mIdx < 0) throw LibraryException("No member found with ID " + std::to_string(memberId) + ".");

    int bookId = Utils::readInt("Book ID: ", 1, 999999);
    int bIdx = findBookIndex(bookId);
    if (bIdx < 0) throw LibraryException("No book found with ID " + std::to_string(bookId) + ".");

    // Check every rule BEFORE changing any data, so a failure leaves nothing half-done.
    if (!books[bIdx].isAvailable()) throw LibraryException("Book is already issued to another member.");
    if (!members[mIdx].canBorrow())
        throw LibraryException("Member has reached the borrow limit of " + std::to_string(Member::MAX_BORROW) + " books.");

    std::string date = Utils::readDate("Issue date");

    Transaction t(nextTransactionId, bookId, memberId, date);
    if (txnCount == txnCapacity) growArray(transactions, txnCount, txnCapacity);
    books[bIdx].issue();
    members[mIdx].borrowBook();
    transactions[txnCount++] = t;
    ++nextTransactionId;

    std::cout << "\nBook issued successfully.\n"
              << "  Transaction ID : " << t.getTransactionId() << "\n"
              << "  Book           : " << books[bIdx].getTitle() << "\n"
              << "  Member         : " << members[mIdx].getName() << "\n"
              << "  Issue date     : " << date << "\n"
              << "  Due within     : " << Transaction::LOAN_DAYS << " days (fine Rs. "
              << Transaction::FINE_PER_DAY << " per late day)\n";
}

void Library::returnBook() {
    std::cout << "\n== Return Book ==\n";
    int bookId = Utils::readInt("Book ID: ", 1, 999999);
    int bIdx = findBookIndex(bookId);
    if (bIdx < 0) throw LibraryException("No book found with ID " + std::to_string(bookId) + ".");
    int tIdx = findOpenTransaction(bookId);
    if (tIdx < 0) throw LibraryException("This book is not currently issued.");
    int mIdx = findMemberIndex(transactions[tIdx].getMemberId());

    std::string date = Utils::readDate("Return date");
    transactions[tIdx].closeTransaction(date);      // validates the date, throws if wrong
    books[bIdx].returnBook();
    if (mIdx >= 0) members[mIdx].returnBook();

    std::cout << "\nBook returned successfully.\n"
              << "  Transaction ID : " << transactions[tIdx].getTransactionId() << "\n"
              << "  Book           : " << books[bIdx].getTitle() << "\n"
              << "  Issued on      : " << transactions[tIdx].getIssueDate() << "\n"
              << "  Returned on    : " << date << "\n"
              << "  Days late      : " << transactions[tIdx].daysLate() << "\n"
              << "  Fine           : Rs. " << std::fixed << std::setprecision(2)
              << transactions[tIdx].calculateFine() << "\n";
}

// ================================================================== 7. REPORT
void Library::report() const {
    int available = 0;
    for (int i = 0; i < bookCount; ++i) if (books[i].isAvailable()) ++available;

    int active = 0, returned = 0;
    double collected = 0, outstanding = 0;
    for (int i = 0; i < txnCount; ++i) {
        if (transactions[i].isReturned()) { ++returned; collected += transactions[i].calculateFine(); }
        else { ++active; outstanding += transactions[i].calculateFine(); }
    }

    std::cout << "\n=============== LIBRARY REPORT ===============\n"
              << "Report date          : " << Utils::today() << "\n"
              << "Total books          : " << bookCount << "\n"
              << "  Available          : " << available << "\n"
              << "  Issued             : " << bookCount - available << "\n"
              << "Total members        : " << memberCount << "\n"
              << "Total transactions   : " << txnCount << " (active " << active << ", returned " << returned << ")\n"
              << std::fixed << std::setprecision(2)
              << "Fine collected       : Rs. " << collected << "\n"
              << "Fine outstanding     : Rs. " << outstanding << "\n";

    std::cout << "\n--- Books by category ---\n";
    if (bookCount == 0) std::cout << "(no books)\n";
    std::map<std::string, int> perCategory;
    for (int i = 0; i < bookCount; ++i) ++perCategory[books[i].getCategory()];
    for (std::map<std::string, int>::const_iterator it = perCategory.begin(); it != perCategory.end(); ++it)
        std::cout << "  " << std::left << std::setw(20) << it->first << it->second << "\n";

    std::cout << "\n--- Currently issued books ---\n";
    if (active == 0) std::cout << "(none)\n";
    else {
        std::cout << std::left << std::setw(8) << "TxnID" << std::setw(28) << "Book" << std::setw(20)
                  << "Member" << std::setw(13) << "Issued on" << std::setw(11) << "Days late" << "Fine (Rs.)\n";
        for (int i = 0; i < txnCount; ++i) {
            if (transactions[i].isReturned()) continue;
            int b = findBookIndex(transactions[i].getBookId());
            int m = findMemberIndex(transactions[i].getMemberId());
            std::cout << std::left << std::setw(8) << transactions[i].getTransactionId()
                      << std::setw(28) << Utils::fit(b >= 0 ? books[b].getTitle() : "(deleted)", 26)
                      << std::setw(20) << Utils::fit(m >= 0 ? members[m].getName() : "(deleted)", 18)
                      << std::setw(13) << transactions[i].getIssueDate()
                      << std::setw(11) << transactions[i].daysLate()
                      << transactions[i].calculateFine() << "\n";
        }
    }
    std::cout << "==============================================\n";
}

// ================================================================== 8. SAVE / LOAD
void Library::saveLoadMenu() {
    std::cout << "\n--- SAVE / LOAD ---\n1. Save all data to files\n2. Load all data from files\n0. Back to main menu\n";
    int c = Utils::readInt("Choice: ", 0, 2);
    if (c == 1) saveAll();
    else if (c == 2) {
        if (Utils::confirm("Loading replaces the data currently in memory. Continue?")) loadAll(false);
        else std::cout << "Load cancelled.\n";
    }
}

void Library::saveAll() const {
    fileMgr.saveBooks(books, bookCount);
    fileMgr.saveMembers(members, memberCount);
    fileMgr.saveTransactions(transactions, txnCount);
    std::cout << "Data saved: " << bookCount << " book(s), " << memberCount << " member(s), "
              << txnCount << " transaction(s).\n";
}

void Library::loadAll(bool quiet) {
    bool b = fileMgr.loadBooks(books, bookCount, bookCapacity);
    bool m = fileMgr.loadMembers(members, memberCount, memberCapacity);
    bool t = fileMgr.loadTransactions(transactions, txnCount, txnCapacity);

    nextTransactionId = 1;
    for (int i = 0; i < txnCount; ++i)
        if (transactions[i].getTransactionId() >= nextTransactionId)
            nextTransactionId = transactions[i].getTransactionId() + 1;

    if (!b && !m && !t) {
        if (!quiet) std::cout << "No saved data files found.\n";
        return;
    }
    std::cout << "Data loaded: " << bookCount << " book(s), " << memberCount << " member(s), "
              << txnCount << " transaction(s).\n";
}
