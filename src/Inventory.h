#ifndef INVENTORY_SYSTEM_INVENTORY_H
#define INVENTORY_SYSTEM_INVENTORY_H

class Inventory {
public:
    Inventory();

    ~Inventory();

    bool addItem();
    bool editItem();
    void listItems();

private:
};


#endif //INVENTORY_SYSTEM_INVENTORY_H