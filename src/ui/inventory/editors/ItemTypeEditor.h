#ifndef INVENTORY_SYSTEM_ITEMTYPEEDITOR_H
#define INVENTORY_SYSTEM_ITEMTYPEEDITOR_H

#include "BasicEditor.h"


class ItemTypeEditor : public BasicEditor {
    Q_OBJECT

public:
    ItemTypeEditor(DataBase& db, QWidget* parent = nullptr);

    ~ItemTypeEditor();

    void loadItemType(const ItemType* type = nullptr);

private:
    void initEditor();


private slots:

};


#endif //INVENTORY_SYSTEM_ITEMTYPEEDITOR_H