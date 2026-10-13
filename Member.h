// Member.h
// A library member. Inherits id, name and phone from Person.
#ifndef MEMBER_H
#define MEMBER_H

#include "Person.h"

class Member : public Person {
private:
    std::string email;
    int         borrowedCount;     // books currently borrowed

public:
    static const int MAX_BORROW = 3;

    Member();
    Member(int memberId, const std::string& name, const std::string& phone,
           const std::string& email, int borrowedCount = 0);

    void display() const;          // overrides Person::display()
    static void printHeader();

    void borrowBook();             // throws if limit reached
    void returnBook();             // throws if nothing borrowed
    bool canBorrow() const { return borrowedCount < MAX_BORROW; }

    int                getMemberId() const     { return id; }
    const std::string& getEmail() const        { return email; }
    int                getBorrowedCount() const { return borrowedCount; }
    void               setEmail(const std::string& e);
};

#endif
