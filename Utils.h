// Utils.h
// Helper functions for validated console input, dates and text formatting.
#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {

// ---- text helpers ----
std::string trim(const std::string& s);
std::string toLower(std::string s);
std::string fit(const std::string& s, std::size_t width);   // shorten long text for tables

// ---- validation ----
bool isValidPhone(const std::string& phone);   // exactly 10 digits
bool isValidEmail(const std::string& email);   // simple structure check
bool isValidDate(const std::string& date);     // DD-MM-YYYY, real calendar date

// ---- validated input (re-prompts until valid) ----
std::string readLine(const std::string& prompt);
std::string readText(const std::string& prompt, bool allowEmpty = false);
int         readInt(const std::string& prompt, int minValue, int maxValue);
std::string readPhone(const std::string& prompt, bool allowEmpty = false);
std::string readEmail(const std::string& prompt, bool allowEmpty = false);
std::string readDate(const std::string& prompt);   // Enter / 0 = today's date
bool        confirm(const std::string& prompt);    // y / n

// ---- dates ----
std::string today();                               // DD-MM-YYYY
std::string normalizeDate(const std::string& date);
long        dateToDays(const std::string& date);   // days since 1970-01-01

}  // namespace Utils

#endif
