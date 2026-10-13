// main.cpp
// Library Management System - IU2541230002 - CPP01
// main() only creates the system object, shows the menu and calls system functions.

#include <iostream>
#include "Library.h"
#include "LibraryException.h"
#include "Utils.h"

static void showMenu() {
    std::cout << "\n===============================================\n"
              << "        LIBRARY MANAGEMENT SYSTEM\n"
              << "===============================================\n"
              << " 1. Add Record\n"
              << " 2. Display Records\n"
              << " 3. Search Record\n"
              << " 4. Update Record\n"
              << " 5. Delete Record\n"
              << " 6. Main Transaction (Issue / Return Book)\n"
              << " 7. Report\n"
              << " 8. Save / Load\n"
              << " 9. Exit\n"
              << "-----------------------------------------------\n";
}

int main() {
    Library lib;
    try {
        lib.loadAll(true);                   // start with saved data, if any
    } catch (const std::exception& e) {
        std::cout << "Could not load saved data: " << e.what() << "\n";
    }

    bool running = true;
    while (running) {
        try {
            showMenu();
            int choice = Utils::readInt("Enter your choice (1-9): ", 1, 9);
            switch (choice) {
                case 1: lib.addRecord();       break;
                case 2: lib.displayRecords();  break;
                case 3: lib.searchRecord();    break;
                case 4: lib.updateRecord();    break;
                case 5: lib.deleteRecord();    break;
                case 6: lib.transactionMenu(); break;
                case 7: lib.report();          break;
                case 8: lib.saveLoadMenu();    break;
                case 9:
                    if (Utils::confirm("Save data before exiting?")) lib.saveAll();
                    std::cout << "Thank you for using the Library Management System. Goodbye!\n";
                    running = false;
                    break;
            }
        } catch (const InputEndedException&) {
            std::cout << "\nInput ended. Exiting.\n";
            running = false;
        } catch (const LibraryException& e) {
            std::cout << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }
    return 0;
}
