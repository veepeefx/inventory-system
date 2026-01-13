#include "ItemTypeEditor.h"


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

    // add widgets

    mainLayout_->addLayout(layout);
}

