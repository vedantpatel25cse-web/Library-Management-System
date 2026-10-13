// Transaction.h
// One issue/return record linking a Book and a Member.
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    int         transactionId;
    int         bookId;
    int         memberId;
    std::string issueDate;      // DD-MM-YYYY
    std::string returnDate;     // DD-MM-YYYY, empty while the book is still issued

public:
    static const int    LOAN_DAYS    = 14;   // free borrowing period
    static const int    FINE_PER_DAY = 5;    // Rs. per late day

    Transaction();
    Transaction(int transactionId, int bookId, int memberId,
                const std::string& issueDate, const std::string& returnDate = "");

    void   display() const;
    static void printHeader();

    bool   isReturned() const { return !returnDate.empty(); }
    void   closeTransaction(const std::string& date);   // sets the return date
    int    daysLate() const;                            // 0 if on time
    double calculateFine() const;                       // daysLate() * FINE_PER_DAY

    int                getTransactionId() const { return transactionId; }
    int                getBookId() const        { return bookId; }
    int                getMemberId() const      { return memberId; }
    const std::string& getIssueDate() const     { return issueDate; }
    const std::string& getReturnDate() const    { return returnDate; }
};

#endif
