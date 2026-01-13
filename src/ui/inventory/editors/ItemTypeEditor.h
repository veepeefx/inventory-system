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

    void initEditor();

};


#endif //INVENTORY_SYSTEM_ITEMTYPEEDITOR_H