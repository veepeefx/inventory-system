#include <iostream>

#include "Inventory.h"

int main()
{
    bool run = true;
    Inventory inventory;

    while (run) {
        std::cout << "\nInventory system\n"
                     "[a] add item, [e] edit item, [l] list items, [q] quit" << std::endl;

        std::string input;
        getline(std::cin, input);

        switch (input[0]) {
            case 'a':
                if (inventory.addItem()) {
                    std::cout << "Successfully added item" << std::endl;
                } else {
                    std::cout << "Failed to add item" << std::endl;
                }
                break;
            case 'e':
                if (inventory.editItem()) {
                    std::cout << "Successfully edited item" << std::endl;
                } else {
                    std::cout << "Failed to edit item" << std::endl;
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
