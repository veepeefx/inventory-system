#include "ItemView.h"

#include "editors/ItemEditor.h"
#include "../../utils/ui/PopUpMessage.h"


ItemView::ItemView(DataBase& db, InventoryUse use, QWidget* parent)
    : InventoryView(db, use, parent)
{
    itemModel_ = new ItemTableModel(db_, this);
    itemModel_->loadData();

    table_->setModel(*itemModel_);
}

ItemView::~ItemView() {}

void ItemView::makeSearch(const std::vector<SearchFieldWidget*>& searchFields)
{
    lastSearch_ = getSearch(searchFields);
    itemModel_->loadData(lastSearch_);
}

void ItemView::openEditor(int row)
{
    const Item* item = itemModel_->getItem(row);
    ItemEditor* editor = new ItemEditor(db_, item, this);

    connect(editor, &BasicEditor::reloadView, this, &ItemView::updateView);
    editor->exec();
}

void ItemView::updateView()
{
    itemModel_->loadData(lastSearch_);
}

void ItemView::removeButtonClicked()
{
    int row = table_->selectedRowIndex();
    int id = itemModel_->getId(row);

    // removes id if row valid (-1 incase of error)
    if (id >= 0) {
        db_.remove(id, InventoryMode::ITEM);
        updateView();
    }
}

void ItemView::select()
{
    int row = table_->selectedRowIndex();
    const Item* item = itemModel_->getItem(row);

    if (item->itemTypeId) {
        bool move = PopUpMessage::confirm(PopUpCode::ITEM_HAS_ITEM_TYPE, this);
        if (!move) { return; }
    }

    emit selectedId(item->id);
    close();
}
