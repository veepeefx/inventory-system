#include "Inventory.h"

#include <iostream>


Inventory::Inventory()
{}

Inventory::~Inventory()
{}

bool Inventory::addItem()
{
    std::cout << "Adding item" << std::endl;
    return true;
}

bool Inventory::editItem()
{
    std::cout << "Editing item" << std::endl;
    return true;
}

void Inventory::listItems()
{
    std::cout << "Items in inventory:" << std::endl;
    std::cout << "..." << std::endl;
}
