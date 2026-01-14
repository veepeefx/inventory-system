#ifndef INVENTORY_SYSTEM_ITEMTYPEEDITOR_H
#define INVENTORY_SYSTEM_ITEMTYPEEDITOR_H

#include "BasicEditor.h"


class ItemTypeEditor : public BasicEditor {
    Q_OBJECT

public:
    ItemTypeEditor(DataBase& db, QWidget* parent = nullptr);
    ~ItemTypeEditor();

    void openItemType(const ItemType* type = nullptr);

private:
    const ItemType* loadedItemType = nullptr;

    void initItemList(const QGridLayout & layout, int& row);

    void initEditor();
    void initItemList(QGridLayout& layout, int& row);

private slots:
    // add item to item type
    void addItem();

    // remove item from item type
    void removeItem();
};


#endif //INVENTORY_SYSTEM_ITEMTYPEEDITOR_H