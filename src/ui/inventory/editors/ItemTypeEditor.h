#ifndef INVENTORY_SYSTEM_ITEMTYPEEDITOR_H
#define INVENTORY_SYSTEM_ITEMTYPEEDITOR_H

#include "BasicEditor.h"
#include "../CustomTableView.h"
#include "../tablemodels/ItemTableModel.h"


class ItemTypeEditor : public BasicEditor {
    Q_OBJECT

public:
    ItemTypeEditor(DataBase& db, const ItemType* itemType = nullptr, QWidget* parent = nullptr);
    ~ItemTypeEditor() = default;

private:
    const ItemType* loadedItemType_;

    // item type item listing
    ItemTableModel* itemModel_ = nullptr;
    CustomTableView* table_ = nullptr;

    QLineEdit* idLE_ = nullptr;
    QLineEdit* typeLE_ = nullptr;
    QLineEdit* nameLE_ = nullptr;
    QLineEdit* selfLocationLE_ = nullptr;
    QLineEdit* quantityLE_ = nullptr;
    QLineEdit* lastModifiedLE_ = nullptr;
    QLineEdit* createdLE_ = nullptr;

    void initEditor();
    void initItemList(QGridLayout& layout, int& row);
    void openItemType();

    bool updateItemsItemTypeIds(int itemTypeId);

private slots:
    void save();

    // add item to item type
    void addItem();

    // remove item from item type
    void removeItem();
};


#endif //INVENTORY_SYSTEM_ITEMTYPEEDITOR_H