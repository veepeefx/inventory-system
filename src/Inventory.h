#ifndef INVENTORY_SYSTEM_INVENTORY_H
#define INVENTORY_SYSTEM_INVENTORY_H
#include "DataBase.h"

class Inventory {
public:
    Inventory();

    ~Inventory();

    bool addItem();
    bool editItem(const int& id);
    bool removeItem(const int& id);
    void listItems();

private:
    DataBase* dataBase_;
};


#endif //INVENTORY_SYSTEM_INVENTORY_H