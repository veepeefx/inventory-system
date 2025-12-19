#ifndef INVENTORY_SYSTEM_DATABASE_H
#define INVENTORY_SYSTEM_DATABASE_H

#include <string>
#include <vector>

#include "sqlite3.h"
#include "../Item.h"

class DataBase {
public:
    DataBase(const std::string& filePath);

    ~DataBase();

    bool insertItem(const Item& item);
    bool updateItem(const int& id, const ItemUpdate& item);
    bool deleteItem(const int& itemId);
    std::vector<Item> getItems();

private:

    sqlite3 *db = nullptr;
    void initTables();

    // dynamically binds all updated changes
    // returns index for next bind (id)
    int dynamicUpdateBinding(const ItemUpdate& item, sqlite3_stmt* stmt);

};


#endif //INVENTORY_SYSTEM_DATABASE_H