#include "ItemTypeEditor.h"

#include <iostream>

#include "../tablemodels/ItemTableModel.h"
#include "../../../utils/ui/UiTools.h"
#include "../InventoryView.h"


#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <unordered_set>


ItemTypeEditor::ItemTypeEditor(DataBase& db, const ItemType* itemType, QWidget* parent)
    : BasicEditor(db, parent), loadedItemType_(itemType)
{
    initEditor();
    initControls();
    connect(this, &BasicEditor::saveData, this, &ItemTypeEditor::save);

    openItemType();

    setWindowTitle("Item Type Editor");
}

void ItemTypeEditor::initEditor()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    idLE_ = UiTools::newLineEdit("ID:", {*layout, row++});
    idLE_->setDisabled(true);    // cant be modified

    typeLE_ = UiTools::newLineEdit("Type Number:", {*layout, row++});
    nameLE_ = UiTools::newLineEdit("Type Name:", {*layout, row++, 0, 3});
    selfLocationLE_ = UiTools::newLineEdit("Self Location:", {*layout, row});
    quantityLE_ = UiTools::newLineEdit("Total Quantity:", {*layout, row++, 2});
    quantityLE_->setDisabled(true); // user cannot change it is counted from items

    layout->addWidget(new QLabel(""), row++, 0);

    initItemList(*layout, row);

    layout->addWidget(new QLabel(""), row++, 0);

    lastModifiedLE_ = UiTools::newLineEdit("Last Modified:", {*layout, row});
    lastModifiedLE_->setDisabled(true);   // updates automatically
    createdLE_ = UiTools::newLineEdit("Created At:", {*layout, row++, 2});
    createdLE_->setDisabled(true);        // updates automatically

    layout->addWidget(new QLabel(""), row++, 0);

    mainLayout_->addLayout(layout);
}

void ItemTypeEditor::initItemList(QGridLayout &layout, int& row)
{
    // items table
    layout.addWidget(new QLabel("Linked Items:"), row++, 0);

    itemModel_ = new ItemTableModel(db_, this);
    table_ = new CustomTableView(*itemModel_, this);

    layout.addWidget(table_, row++, 0, 1, 4);

    // controls
    QHBoxLayout* tableControlLayout = new QHBoxLayout();

    QPushButton* addItemButton = new QPushButton("Add", this);
    QPushButton* removeItemButton = new QPushButton("Remove", this);

    connect(addItemButton, &QPushButton::clicked, this, &ItemTypeEditor::addItem);
    connect(removeItemButton, &QPushButton::clicked, this, &ItemTypeEditor::removeItem);

    tableControlLayout->addWidget(addItemButton);
    tableControlLayout->addWidget(removeItemButton);
    tableControlLayout->addStretch(0);

    layout.addLayout(tableControlLayout, row++, 0, 1, 4);
}

void ItemTypeEditor::openItemType()
{
    if (loadedItemType_ == nullptr) { return; }

    idLE_->setText(QString::number(loadedItemType_->id));
    nameLE_->setText(QString::fromStdString(loadedItemType_->name));
    typeLE_->setText(QString::fromStdString(loadedItemType_->typeNumber));
    selfLocationLE_->setText(QString::fromStdString(loadedItemType_->selfLocation));
    quantityLE_->setText(QString::number(loadedItemType_->totalQuantity));
    lastModifiedLE_->setText(QString::fromStdString(loadedItemType_->modifiedAt));
    createdLE_->setText(QString::fromStdString(loadedItemType_->createdAt));

    for (int itemId : loadedItemType_->items) {
        itemModel_->addRow(itemId);
    }
}

bool ItemTypeEditor::updateItemsItemTypeIds(int itemTypeId)
{
    const std::vector<int>& newItemIds = itemModel_->getItemIds();
    std::unordered_set oldSet(loadedItemType_->items.begin(), loadedItemType_->items.end());
    std::unordered_set newSet(newItemIds.begin(), newItemIds.end());

    std::vector<int> removed;
    std::vector<int> added;
    for (int id : oldSet) {
        if (!newSet.contains(id)) {
            removed.push_back(id);
        }
    }
    for (int id : newSet) {
        if (!oldSet.contains(id)) {
            added.push_back(id);
        }
    }

    // removing itemtypeid if it was removed from itemtype
    bool rem = db_.updateItemTypes(-1, removed);

    // adding itemtypeifd if it was added to itemtype
    bool add = db_.updateItemTypes(itemTypeId, added);

    // returns true if all processes were successful
    return rem && add;
}

void ItemTypeEditor::save()
{
    if (loadedItemType_ == nullptr) {
        ItemType newItemType;
        newItemType.name = nameLE_->text().toStdString();
        newItemType.typeNumber = typeLE_->text().toStdString();
        newItemType.selfLocation = selfLocationLE_->text().toStdString();

        // save to db
        if (!db_.insert(newItemType)) {

            // POP UP ALERT
            return;
        }
        // updating items with their new itemtypeid when we get the id
        db_.updateItemTypes(newItemType.id, itemModel_->getItemIds());

    } else {
        ItemTypeUpdate updateItemType;

        if (loadedItemType_->name != nameLE_->text().toStdString()) {
            updateItemType.name = nameLE_->text().toStdString();
        }
        if (loadedItemType_->typeNumber != typeLE_->text().toStdString()) {
            updateItemType.typeNumber = typeLE_->text().toStdString();
        }
        if (loadedItemType_->selfLocation != selfLocationLE_->text().toStdString()) {
            updateItemType.selfLocation = selfLocationLE_->text().toStdString();
        }

        // update to db
        if (!db_.update(updateItemType, loadedItemType_->id, loadedItemType_->name,
            loadedItemType_->typeNumber)) {
            return;
        }

        updateItemsItemTypeIds(loadedItemType_->id);
    }

    emit updateView();
    accept();
}

void ItemTypeEditor::addItem()
{
    // open new InventoryView with selecting
    InventoryView* inventory = new InventoryView(db_, InventoryMode::ITEM, InventoryUse::SELECTING, this);
    inventory->setWindowTitle("Select Item");
    inventory->setWindowModality(Qt::ApplicationModal);
    inventory->setWindowFlags(Qt::Dialog | Qt::Popup);

    // connect signal that row is selected to adding row to item model
    connect(inventory, &InventoryView::selectedItem, this, [this](int id) {
        itemModel_->addRow(id);
    });

    inventory->show();
}

void ItemTypeEditor::removeItem()
{
    int row = table_->selectedRowIndex();
    itemModel_->removeRow(row);
}
