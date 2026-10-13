# Makefile - Library Management System (IU2541230002 - CPP01)
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET   = LibraryManagement
SRCS     = main.cpp Library.cpp FileManager.cpp Book.cpp Member.cpp Person.cpp Transaction.cpp Utils.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe *.o
