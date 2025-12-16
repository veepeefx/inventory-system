#include "Inventory.h"

#include <iostream>
#include <sstream>

#include "DataBase.h"


Inventory::Inventory()
{
    dataBase_ = new DataBase("database.db");
}

Inventory::~Inventory()
{
    delete dataBase_;
}

bool Inventory::addItem()
{
    std::cout << "\nAdding item... \nGive item name and quantity" << std::endl;

    std::string input;
    getline(std::cin, input);

    std::istringstream iss(input);
    Item item;

    if (!(iss >> item.name >> item.quantity)) {
        return false;
    }

    if (!dataBase_->insertItem(item)) {
        return false;
    }

    return true;
}

bool Inventory::editItem(const int& id)
{
    std::cout << "\nEditing item... \nGive changed name and quantity" << std::endl;

    std::string input;
    getline(std::cin, input);

    std::istringstream iss(input);

    std::string tempName;
    int tempQuantity;

    if (!(iss >> tempName >> tempQuantity)) {
        return false;
    }
    ItemUpdate item;
    item.name = tempName;
    item.quantity = tempQuantity;

    if (!dataBase_->updateItem(id, item)) {
        return false;
    }
    return true;
}

bool Inventory::removeItem(const int& id)
{
    std::cout << "\nRemoving item..." << std::endl;
    if (!dataBase_->deleteItem(id)) {
        return false;
    }

    return true;
}

void Inventory::listItems()
{
    std::cout << "\nItems in inventory:" << std::endl;
    dataBase_->list_all();
}
