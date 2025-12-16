#ifndef INVENTORY_SYSTEM_ITEM_H
#define INVENTORY_SYSTEM_ITEM_H
#include <optional>
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

// used for updating item
struct ItemUpdate {
    std::optional<std::string> name;
    std::optional<std::string> productNumber;
    std::optional<int> quantity;
    std::optional<std::string> ean;
    std::optional<std::string> selfLocation;
    std::optional<double> priceNoVat;
    std::optional<double> vat;
    std::optional<double> discount;
};


#endif //INVENTORY_SYSTEM_ITEM_H