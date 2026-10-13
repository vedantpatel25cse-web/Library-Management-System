// LibraryException.h
// Custom exception classes used across the Library Management System.
#ifndef LIBRARY_EXCEPTION_H
#define LIBRARY_EXCEPTION_H

#include <stdexcept>
#include <string>

// Thrown for every business-rule / validation / file error in the project.
class LibraryException : public std::runtime_error {
public:
    explicit LibraryException(const std::string& message)
        : std::runtime_error(message) {}
};

// Thrown when standard input is closed (EOF) so that the program can exit cleanly.
class InputEndedException : public LibraryException {
public:
    InputEndedException() : LibraryException("Input stream ended.") {}
};

#endif
