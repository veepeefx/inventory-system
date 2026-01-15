#include "ItemTypeEditor.h"
#include "../tablemodels/ItemTableModel.h"
#include "../../../utils/Tools.h"


#include <QLabel>
#include <QTableView>
#include <QPushButton>


ItemTypeEditor::ItemTypeEditor(DataBase& db, const ItemType* itemType, QWidget* parent)
    : BasicEditor(db, parent), loadedItemType_(itemType)
{
    initEditor();
    initControls();
    connect(this, &BasicEditor::saveData, this, &ItemTypeEditor::save);

    openItemType();

    setWindowTitle("Item Type Editor");
}

ItemTypeEditor::~ItemTypeEditor()
{}

void ItemTypeEditor::initEditor()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    idLE_ = Tools::newLineEdit(*layout, "ID:", row++);
    idLE_->setDisabled(true);    // cant be modified

    typeLE_ = Tools::newLineEdit(*layout, "Type Number:", row++);
    nameLE_ = Tools::newLineEdit(*layout, "Type Name:", row++, 0, 3);
    selfLocationLE_ = Tools::newLineEdit(*layout, "Self Location:", row);
    quantityLE_ = Tools::newLineEdit(*layout, "Total Quantity:", row++, 2);
    quantityLE_->setDisabled(true); // user cannot change it is counted from items

    layout->addWidget(new QLabel(""), row++, 0);

    initItemList(*layout, row);

    layout->addWidget(new QLabel(""), row++, 0);

    lastModifiedLE_ = Tools::newLineEdit(*layout, "Last Modified:", row);
    lastModifiedLE_->setDisabled(true);   // updates automatically
    createdLE_ = Tools::newLineEdit(*layout, "Created At:", row++, 2);
    createdLE_->setDisabled(true);        // updates automatically

    layout->addWidget(new QLabel(""), row++, 0);

    mainLayout_->addLayout(layout);
}

void ItemTypeEditor::initItemList(QGridLayout &layout, int& row)
{
    // items table
    layout.addWidget(new QLabel("Linked Items:"), row++, 0);

    QTableView* table = new QTableView(this);
    itemModel_ = new ItemTableModel(db_, this);
    table->setModel(itemModel_);

    layout.addWidget(table, row++, 0, 1, 4);

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
        if (loadedItemType_->items != itemModel_->getItemIds()) {
            updateItemType.items = itemModel_->getItemIds();
        }

        // update to db
        if (!db_.update(updateItemType, loadedItemType_->id, loadedItemType_->name,
            loadedItemType_->typeNumber)) {
            return;
        }
    }

    emit updateView();
    accept();
}

void ItemTypeEditor::addItem()
{
}

void ItemTypeEditor::removeItem()
{
}
