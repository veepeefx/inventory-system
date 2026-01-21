#include "ItemTypeView.h"
#include "editors/ItemTypeEditor.h"

ItemTypeView::ItemTypeView(DataBase& db, InventoryUse use, QWidget* parent)
    : InventoryView(db, use, parent)
{
    itemTypeModel_ = new ItemTypeTableModel(db_, this);
    itemTypeModel_->loadData();

    table_->setModel(*itemTypeModel_);
}

ItemTypeView::~ItemTypeView() {}

void ItemTypeView::makeSearch(const std::vector<SearchFieldWidget*>& searchFields)
{
    lastSearch_ = getSearch(searchFields);
    itemTypeModel_->loadData(lastSearch_);
}

void ItemTypeView::openEditor(int row)
{
    const ItemType* itemType = itemTypeModel_->getItemType(row);
    ItemTypeEditor* editor = new ItemTypeEditor(db_, itemType, this);

    connect(editor, &BasicEditor::reloadView, this, &ItemTypeView::updateView);
    editor->exec();
}

void ItemTypeView::updateView()
{
    itemTypeModel_->loadData(lastSearch_);
}

void ItemTypeView::removeButtonClicked()
{
    int row = table_->selectedRowIndex();
    int id = itemTypeModel_->getId(row);

    // removes id if row valid (-1 incase of error)
    if (id >= 0) {
        db_.remove(id, InventoryMode::ITEM_TYPE);
        updateView();
    }
}

void ItemTypeView::select()
{
    int row = table_->selectedRowIndex();
    const ItemType* type = itemTypeModel_->getItemType(row);

    emit selectedId(type->id);
    close();
}
