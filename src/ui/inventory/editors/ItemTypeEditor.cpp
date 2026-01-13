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

void ItemTypeEditor::loadItemType(const ItemType *type)
{
    if (type != nullptr) {

    }
}

void ItemTypeEditor::initEditor()
{
    QGridLayout* layout = new QGridLayout();

    // add widgets

    mainLayout_->addLayout(layout);
}

