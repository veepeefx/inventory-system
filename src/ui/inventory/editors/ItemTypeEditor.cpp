#include "ItemTypeEditor.h"
#include "../tablemodels/ItemTableModel.h"

#include <QLabel>
#include <QTableView>
#include <QPushButton>


ItemTypeEditor::ItemTypeEditor(DataBase& db, QWidget* parent)
    : BasicEditor(db, parent)
{
    initEditor();
    initControls();

    setWindowTitle("Item Type Editor");
}

ItemTypeEditor::~ItemTypeEditor()
{}

void ItemTypeEditor::openItemType(const ItemType* type)
{
    loadedItemType = type;
    if (type == nullptr) { return; }
}

void ItemTypeEditor::initEditor()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    QLineEdit* idLE = newLineEdit(*layout, "ID:", row++);
    idLE->setDisabled(true);    // cant be modified

    QLineEdit* typeLE = newLineEdit(*layout, "Type Number:", row++);
    QLineEdit* nameLE = newLineEdit(*layout, "Type Name:", row++, 0, 3);
    QLineEdit* selfLocationLE = newLineEdit(*layout, "Self Location:", row);
    QLineEdit* quantityLE = newLineEdit(*layout, "Total Quantity:", row++, 2);

    layout->addWidget(new QLabel(""), row++, 0);

    initItemList(*layout, row);

    layout->addWidget(new QLabel(""), row++, 0);

    QLineEdit* lastModifiedLE = newLineEdit(*layout, "Last Modified:", row);
    lastModifiedLE->setDisabled(true);   // updates automatically
    QLineEdit* createdLE = newLineEdit(*layout, "Created At:", row++, 2);
    createdLE->setDisabled(true);        // updates automatically

    layout->addWidget(new QLabel(""), row++, 0);

    mainLayout_->addLayout(layout);
}

void ItemTypeEditor::initItemList(QGridLayout &layout, int& row)
{
    // items table
    layout.addWidget(new QLabel("Linked Items:"), row++, 0);

    QTableView* table = new QTableView(this);
    ItemTableModel* itemTable = new ItemTableModel(db_, this);
    table->setModel(itemTable);

    layout.addWidget(table, row++, 0, 1, 4);

    // controls
    QHBoxLayout* tableControlLayout = new QHBoxLayout();

    QPushButton* addItemButton = new QPushButton("Add", this);
    QPushButton* removeItemButton = new QPushButton("Remove", this);

    if (loadedItemType == nullptr) {
        addItemButton->setEnabled(false);
        addItemButton->setToolTip("Create item type before adding items");
        removeItemButton->setEnabled(false);
        removeItemButton->setToolTip("Create item type before adding items");
    }

    connect(addItemButton, &QPushButton::clicked, this, &ItemTypeEditor::addItem);
    connect(removeItemButton, &QPushButton::clicked, this, &ItemTypeEditor::removeItem);

    tableControlLayout->addWidget(addItemButton);
    tableControlLayout->addWidget(removeItemButton);
    tableControlLayout->addStretch(0);

    layout.addLayout(tableControlLayout, row++, 0, 1, 4);
}

void ItemTypeEditor::addItem()
{
}

void ItemTypeEditor::removeItem()
{
}
