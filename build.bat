@echo off
REM Setup / build file for Windows (needs MinGW g++ : https://www.mingw-w64.org or Code::Blocks / Dev-C++ compiler)
echo Compiling Library Management System...
g++ -std=c++11 -Wall -Wextra -O2 -o LibraryManagement.exe main.cpp Library.cpp FileManager.cpp Book.cpp Member.cpp Person.cpp Transaction.cpp Utils.cpp
if errorlevel 1 (
    echo.
    echo Compilation FAILED. Check that g++ is installed and in PATH.
    pause
    exit /b 1
)
echo.
echo Build successful. Starting program...
LibraryManagement.exe
pause
