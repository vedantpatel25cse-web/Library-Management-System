// Transaction.cpp
#include "Transaction.h"
#include "LibraryException.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>

Transaction::Transaction()
    : transactionId(0), bookId(0), memberId(0), issueDate(""), returnDate("") {}

Transaction::Transaction(int transactionId, int bookId, int memberId,
                         const std::string& issueDate, const std::string& returnDate)
    : transactionId(transactionId), bookId(bookId), memberId(memberId),
      issueDate(issueDate), returnDate(returnDate) {
    if (transactionId <= 0 || bookId <= 0 || memberId <= 0)
        throw LibraryException("Transaction IDs must be positive numbers.");
    if (!Utils::isValidDate(issueDate))
        throw LibraryException("Invalid issue date in transaction.");
    if (!returnDate.empty()) {
        if (!Utils::isValidDate(returnDate))
            throw LibraryException("Invalid return date in transaction.");
        if (Utils::dateToDays(returnDate) < Utils::dateToDays(issueDate))
            throw LibraryException("Return date cannot be before issue date.");
    }
}

void Transaction::closeTransaction(const std::string& date) {
    if (isReturned()) throw LibraryException("This transaction is already closed.");
    if (!Utils::isValidDate(date)) throw LibraryException("Invalid return date.");
    if (Utils::dateToDays(date) < Utils::dateToDays(issueDate))
        throw LibraryException("Return date cannot be before the issue date (" + issueDate + ").");
    returnDate = date;
}

int Transaction::daysLate() const {
    long end  = isReturned() ? Utils::dateToDays(returnDate) : Utils::dateToDays(Utils::today());
    long held = end - Utils::dateToDays(issueDate);
    return held > LOAN_DAYS ? static_cast<int>(held - LOAN_DAYS) : 0;
}

double Transaction::calculateFine() const {
    return daysLate() * static_cast<double>(FINE_PER_DAY);
}

void Transaction::printHeader() {
    std::cout << std::left << std::setw(8) << "TxnID" << std::setw(8) << "BookID"
              << std::setw(10) << "MemberID" << std::setw(13) << "Issue Date"
              << std::setw(13) << "Return Date" << std::setw(10) << "Status" << "Fine (Rs.)\n"
              << std::string(73, '-') << "\n";
}

void Transaction::display() const {
    std::cout << std::left << std::setw(8) << transactionId << std::setw(8) << bookId
              << std::setw(10) << memberId << std::setw(13) << issueDate
              << std::setw(13) << (isReturned() ? returnDate : "-")
              << std::setw(10) << (isReturned() ? "Returned" : "Issued")
              << std::fixed << std::setprecision(2) << calculateFine() << "\n";
}
