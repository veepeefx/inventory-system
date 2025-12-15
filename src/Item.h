#ifndef INVENTORY_SYSTEM_ITEM_H
#define INVENTORY_SYSTEM_ITEM_H
#include <string>

struct Item {
    std::string name;
    std::string productNumber;
    int quantity = 0;
    std::string ean;
    std::string selfLocation;
    double priceNoVat = 0.0;
    double vat = 0.0;
    double discount = 0.0;
};


#endif //INVENTORY_SYSTEM_ITEM_H