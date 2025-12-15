#ifndef INVENTORY_SYSTEM_DATABASE_H
#define INVENTORY_SYSTEM_DATABASE_H

#include <string>

#include "sqlite3.h"
#include "Item.h"

class DataBase {
public:
    DataBase(const std::string& filePath);

    ~DataBase();

    bool insertItem(const Item& item);
    bool deleteItem(const int& itemId);
    bool list_all();

private:

    sqlite3 *db = nullptr;
    void initTables();
};


#endif //INVENTORY_SYSTEM_DATABASE_H