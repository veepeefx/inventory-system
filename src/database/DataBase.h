#ifndef INVENTORY_SYSTEM_DATABASE_H
#define INVENTORY_SYSTEM_DATABASE_H

#include <string>
#include <vector>

#include "sqlite3.h"
#include "../utils/Item.h"
#include "../utils/ItemType.h"

#include "../utils/CommonEnums.h"

class DataBase {
public:
    DataBase(const std::string& filePath);

    ~DataBase();

    bool insert(const Item& item);
    bool insert(const ItemType& itemType);

    bool remove(int id, InventoryMode mode);

    bool updateItem(const ItemUpdate& updateItem, const int& itemId,
                    const std::string& orgName, const std::string& orgProdNumber);

    std::vector<Item> getItems();
    std::vector<ItemType> getItemTypes();

    std::vector<Item> searchItems(const Search& search);


private:

    sqlite3 *db = nullptr;
    void initTables();

    // dynamically binds all updated changes
    // returns index for next bind (id)
    int dynamicUpdateBinding(const ItemUpdate& item, sqlite3_stmt* stmt);

    std::vector<Item> fillItemsToVector(sqlite3_stmt* stmt);
    std::vector<ItemType> fillItemTypesToVector(sqlite3_stmt* stmt);

};


#endif //INVENTORY_SYSTEM_DATABASE_H