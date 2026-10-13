#!/bin/sh
# Build and run on Linux / macOS
g++ -std=c++11 -Wall -Wextra -O2 -o LibraryManagement main.cpp Library.cpp FileManager.cpp Book.cpp Member.cpp Person.cpp Transaction.cpp Utils.cpp && ./LibraryManagement
