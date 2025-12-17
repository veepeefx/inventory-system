#include <iostream>
#include <sstream>

#include "Inventory.h"
#include <QApplication>

#include "View/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DataBase db("database.db");
    Inventory inventory(&db);
    MainWindow window(&db);

    window.show();
    return QApplication::exec();
}
/*
int main()
{
    bool run = true;
    Inventory inventory;

    while (run) {
        std::cout << "\nInventory system\n"
                     "[a] add item, [e {id}] edit item, [r {id}] remove item, [l] list items, [q] quit" << std::endl;

        std::string input;
        getline(std::cin, input);

        std::istringstream iss(input);

        char code;
        std::string idStr;
        int id;

        if (!(iss >> code)) {
            std::cout << "Invalid input." << std::endl;
            continue;
        }

        if (code == 'e' || code == 'r') {
            if (iss >> idStr) {
                id = std::stoi(idStr);
            } else {
                std::cout << "Invalid input." << std::endl;
                continue;
            }
        }

        switch (code) {
            case 'a':
                if (inventory.addItem()) {
                    std::cout << "Successfully added item" << std::endl;
                } else {
                    std::cout << "Failed to add item" << std::endl;
                }
                break;

            case 'e':

                if (inventory.editItem(id)) {
                    std::cout << "Successfully edited item" << std::endl;
                } else {
                    std::cout << "Failed to edit item" << std::endl;
                }
                break;

            case 'r':
                if (inventory.removeItem(id)) {
                    std::cout << "Successfully removed item" << std::endl;
                } else {
                    std::cout << "Failed to remove item" << std::endl;
                }
                break;

            case 'l':
                inventory.listItems();
                break;
            case 'q':
                std::cout << "Quitting..." << std::endl;
                run = false;
                break;

            default:
                std::cout << "Invalid input." << std::endl;
        }
    }
}
*/