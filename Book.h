// Book.h
#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    int         bookId;
    std::string title;
    std::string author;
    std::string category;
    bool        available;

public:
    Book();
    Book(int bookId, const std::string& title, const std::string& author,
         const std::string& category, bool available = true);

    void display() const;
    static void printHeader();

    void issue();          // marks the book as issued; throws if already issued
    void returnBook();     // marks the book as available; throws if not issued

    int                getBookId() const   { return bookId; }
    const std::string& getTitle() const    { return title; }
    const std::string& getAuthor() const   { return author; }
    const std::string& getCategory() const { return category; }
    bool               isAvailable() const { return available; }

    void setTitle(const std::string& t);
    void setAuthor(const std::string& a);
    void setCategory(const std::string& c);
};

#endif
