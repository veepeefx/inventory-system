#ifndef INVENTORY_SYSTEM_ITEMTYPE_H
#define INVENTORY_SYSTEM_ITEMTYPE_H
#include <vector>
#include <string>

struct ItemType {
    int id = 0;
    std::string name;
    std::string typeNumber;
    std::string selfLocation;
    std::vector<int> items;     // item ids saved
    int totalQuantity = 0;
};

struct ItemTypeUpdate {
    std::optional<std::string> name;
    std::optional<std::string> typeNumber;
    std::optional<std::string> selfLocation;
    std::optional<std::vector<int>> items;
    std::optional<int> totalQuantity;
};

#endif //INVENTORY_SYSTEM_ITEMTYPE_H