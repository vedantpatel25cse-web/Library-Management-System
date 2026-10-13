# Library Management System (C++ Console Project)

**Enrollment No.:** IU2541230002 | **Project Code:** CPP01 | **Subject:** C++ Guided Project (Semester 3)

A console-based Library Management System written in C++ using Object-Oriented Programming.
It manages books, members and issue/return transactions, calculates late-return fines and stores all
data permanently in text files.

## Features
- Add, display, search, update and delete **books** and **members**
- Issue and return books (max 3 books per member, 14 free days, fine Rs. 5 per late day)
- Transaction history and summary **report** (availability, fines, overdue books, books by category)
- Save / load data (files are also loaded automatically at start-up)
- Input validation (IDs, 10-digit phone, e-mail, real calendar dates) and exception handling

## Project flow
`USER -> MAIN MENU (main.cpp) -> LIBRARY (system class) -> Book / Member / Transaction -> FileManager -> books.txt, members.txt, transactions.txt`

## Files
| File | Purpose |
|------|---------|
| main.cpp | Creates the `Library` object, shows the menu, calls system functions |
| Library.h / .cpp | System class: dynamic arrays and all menu operations |
| Person.h / .cpp | Base class (id, name, phone, virtual `display()`) |
| Member.h / .cpp | Derived from Person (email, borrowed count) |
| Book.h / .cpp | Book entity (issue / return) |
| Transaction.h / .cpp | Issue-return record and fine calculation |
| FileManager.h / .cpp | Saves / loads the three data files |
| Utils.h / .cpp | Validated input, date helpers |
| LibraryException.h | Custom exception classes |
| books.txt, members.txt, transactions.txt | Sample data (loaded automatically) |
| build.bat / build.sh / Makefile | Build scripts |

## How to build and run
**Windows (MinGW g++ / Code::Blocks / Dev-C++):** double-click `build.bat`
(or run `g++ -std=c++11 -o LibraryManagement.exe main.cpp Library.cpp FileManager.cpp Book.cpp Member.cpp Person.cpp Transaction.cpp Utils.cpp`).

**Linux / macOS:** `make` then `./LibraryManagement` (or run `./build.sh`).

Requires a C++11 compiler. Keep the `.txt` data files in the same folder as the program.

## Data file formats (`|` separated)
- books.txt: `bookId|title|author|category|available(1/0)`
- members.txt: `memberId|name|phone|email|borrowedCount`
- transactions.txt: `transactionId|bookId|memberId|issueDate|returnDate(- if not returned)`

## C++ concepts demonstrated
Classes and objects, encapsulation, default and parameterized constructors, destructors, inheritance
(`Member` from `Person`), runtime polymorphism (virtual `display()`), dynamic memory (`new[]` / `delete[]`
with automatic growth), file handling, validation and exception handling (`LibraryException`).
