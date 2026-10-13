// Utils.cpp
#include "Utils.h"
#include "LibraryException.h"

#include <cctype>
#include <ctime>
#include <cstdio>
#include <iostream>

namespace Utils {

std::string trim(const std::string& s) {
    std::size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    std::size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

std::string toLower(std::string s) {
    for (std::size_t i = 0; i < s.size(); ++i)
        s[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
    return s;
}

std::string fit(const std::string& s, std::size_t width) {
    if (s.size() <= width) return s;
    if (width < 3) return s.substr(0, width);
    return s.substr(0, width - 2) + "..";
}

// ------------------------------------------------------------------ validation
bool isValidPhone(const std::string& phone) {
    if (phone.size() != 10) return false;
    for (std::size_t i = 0; i < phone.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(phone[i]))) return false;
    return true;
}

bool isValidEmail(const std::string& email) {
    if (email.empty() || email.size() > 50) return false;
    if (email.find(' ') != std::string::npos || email.find('|') != std::string::npos) return false;
    std::size_t at = email.find('@');
    if (at == std::string::npos || at == 0) return false;
    if (email.find('@', at + 1) != std::string::npos) return false;
    std::size_t dot = email.find('.', at);
    return dot != std::string::npos && dot > at + 1 && dot + 1 < email.size();
}

static bool parseDate(const std::string& s, int& d, int& m, int& y) {
    if (s.size() != 10) return false;
    if ((s[2] != '-' && s[2] != '/') || (s[5] != '-' && s[5] != '/')) return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    d = std::atoi(s.substr(0, 2).c_str());
    m = std::atoi(s.substr(3, 2).c_str());
    y = std::atoi(s.substr(6, 4).c_str());
    return true;
}

bool isValidDate(const std::string& date) {
    int d, m, y;
    if (!parseDate(date, d, m, y)) return false;
    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = days[m - 1];
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (m == 2 && leap) maxDay = 29;
    return d <= maxDay;
}

// ------------------------------------------------------------------ input
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw InputEndedException();
#ifdef ECHO_INPUT                       // used only to create transcripts for testing
    std::cout << line << "\n";
#endif
    return trim(line);
}

std::string readText(const std::string& prompt, bool allowEmpty) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) {
            if (allowEmpty) return s;
            std::cout << "  Invalid input: this field cannot be empty.\n";
        } else if (s.find('|') != std::string::npos) {
            std::cout << "  Invalid input: the '|' character is not allowed.\n";
        } else if (s.size() > 60) {
            std::cout << "  Invalid input: maximum 60 characters allowed.\n";
        } else {
            return s;
        }
    }
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::string s = readLine(prompt);
        bool ok = !s.empty() && s.size() <= 9;
        for (std::size_t i = 0; ok && i < s.size(); ++i)
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) ok = false;
        if (ok) {
            int v = std::atoi(s.c_str());
            if (v >= minValue && v <= maxValue) return v;
        }
        std::cout << "  Invalid input: enter a whole number between "
                  << minValue << " and " << maxValue << ".\n";
    }
}

std::string readPhone(const std::string& prompt, bool allowEmpty) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty() && allowEmpty) return s;
        if (isValidPhone(s)) return s;
        std::cout << "  Invalid phone: enter exactly 10 digits.\n";
    }
}

std::string readEmail(const std::string& prompt, bool allowEmpty) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty() && allowEmpty) return s;
        if (isValidEmail(s)) return s;
        std::cout << "  Invalid email: use a format like name@example.com.\n";
    }
}

std::string readDate(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt + " [DD-MM-YYYY, Enter = today]: ");
        if (s.empty() || s == "0") return today();
        if (isValidDate(s)) return normalizeDate(s);
        std::cout << "  Invalid date: use DD-MM-YYYY (a real calendar date).\n";
    }
}

bool confirm(const std::string& prompt) {
    while (true) {
        std::string s = toLower(readLine(prompt + " (y/n): "));
        if (s == "y" || s == "yes") return true;
        if (s == "n" || s == "no") return false;
        std::cout << "  Please type y or n.\n";
    }
}

// ------------------------------------------------------------------ dates
std::string today() {
    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    char buf[48];
    std::snprintf(buf, sizeof(buf), "%02d-%02d-%04d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    return buf;
}

std::string normalizeDate(const std::string& date) {
    int d, m, y;
    if (!parseDate(date, d, m, y)) return date;
    char buf[48];
    std::snprintf(buf, sizeof(buf), "%02d-%02d-%04d", d, m, y);
    return buf;
}

long dateToDays(const std::string& date) {
    int d, m, y;
    if (!parseDate(date, d, m, y)) throw LibraryException("Invalid date: " + date);
    // Civil-calendar day count (proleptic Gregorian), independent of time zones.
    y -= (m <= 2);
    long era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = static_cast<unsigned>(y - era * 400);
    unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long>(doe) - 719468;
}

}  // namespace Utils
