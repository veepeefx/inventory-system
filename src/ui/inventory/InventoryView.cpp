#include "InventoryView.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "../../SettingsManager.h"
#include "../../utils/ui/PopUpMessage.h"
#include "../../utils/ui/SearchFieldWidget.h"
#include "editors/ItemEditor.h"
#include "editors/ItemTypeEditor.h"
#include "tablemodels/ItemTypeTableModel.h"


InventoryView::InventoryView(DataBase& db, InventoryMode mode, InventoryUse use, QWidget *parent)
: QWidget(parent), db_(db), mode_(mode), use_(use)
{
    mainLayout_ = new QVBoxLayout(this);

    initSearchBar();
    initTable();
    initControls();
}

InventoryView::~InventoryView() {}

void InventoryView::initTable()
{
    switch (mode_) {
        case InventoryMode::ITEM:       model_ = new ItemTableModel(db_, this);     break;
        case InventoryMode::ITEM_TYPE:  model_ = new ItemTypeTableModel(db_, this); break;
        default: return;
    }

    model_->loadData();

    table_ = new CustomTableView(*model_, this);
    mainLayout_->addWidget(table_);
}

void InventoryView::initSearchBar()
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignLeft);

    QLabel* searchLabel = new QLabel("Search:");
    searchLabel->setStyleSheet("font-size: 12pt; font-weight: bold;");
    layout->addWidget(searchLabel);

    std::vector<SearchFieldWidget*> searchFields;
    int fieldCount = SettingsManager::getSettings().searchFields.size();
    for (int i = 0; i < fieldCount; i++) {
        SearchFieldWidget* search = new SearchFieldWidget(i, true, this);
        search->applyInventorySizes();
        searchFields.push_back(search);
        layout->addWidget(search);
    }

    QPushButton* searchButton = new QPushButton("Search");
    searchButton->setMaximumWidth(200);
    layout->addWidget(searchButton);

    connect(searchButton, &QPushButton::clicked, this, [this, searchFields]() {
        makeSearch(searchFields);
    });

    mainLayout_->addLayout(layout);
}

void InventoryView::initControls()
{
    // init controls which are expected for each use
    switch (use_) {
        case InventoryUse::EDITING:     initEditingControls();    break;
        case InventoryUse::SELECTING:   initSelectingControls();  break;
    }
}

void InventoryView::initEditingControls()
{
    QHBoxLayout* controlLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton("Add");
    QPushButton* removeButton = new QPushButton("Remove");
    QPushButton* editButton = new QPushButton("Edit");
    QPushButton* backButton = new QPushButton("Back");

    connect(addButton, &QPushButton::clicked, this, [this]() { openEditor(); });
    connect(removeButton, &QPushButton::clicked, this, &InventoryView::removeButtonClicked);
    connect(editButton, &QPushButton::clicked, this, &InventoryView::editSelected);
    connect(table_, &QTableView::doubleClicked, this, &InventoryView::editSelected);
    connect(backButton, &QPushButton::clicked, this, [this]() { emit returnMainMenu(); });

    controlLayout->addWidget(addButton);
    controlLayout->addWidget(removeButton);
    controlLayout->addWidget(editButton);
    controlLayout->addWidget(backButton);

    mainLayout_->addLayout(controlLayout);
}

void InventoryView::initSelectingControls()
{
    QHBoxLayout* layout = new QHBoxLayout();
    QPushButton* selectButton = new QPushButton("Select");
    QPushButton* cancelButton = new QPushButton("Cancel");

    // currently only supported with Items
    if (mode_ == InventoryMode::ITEM) {
        connect(table_, &QTableView::doubleClicked, this, &InventoryView::selectItem);
        connect(selectButton, &QPushButton::clicked, this, &InventoryView::selectItem);
    } else {
        selectButton->setEnabled(false);
    }

    connect(cancelButton, &QPushButton::clicked, this, &InventoryView::close);

    layout->addWidget(cancelButton);
    layout->addWidget(selectButton);

    mainLayout_->addLayout(layout);
}


void InventoryView::makeSearch(const std::vector<SearchFieldWidget*>& searchFields)
{
    Search search;
    for (const auto& sf : searchFields) {
        // keeping track if search term is given
        QString text = sf->getSearchText().trimmed();
        if (!text.isEmpty()) {
            // saving all to search
            search.modes.push_back(sf->getSearchMode());
            search.terms.push_back(text.toStdString());
            search.types.push_back(sf->getSearchType());
            search.caseSensitivity.push_back(sf->getCaseSensitivity());
        }
    }

    // make fill with all data or with search data
    if (search.terms.empty()) { model_->loadData(); }
    else { model_->loadData(search); }
}

void InventoryView::updateInventoryView()
{
    model_->loadData();
}

void InventoryView::openEditor(int row)
{
    // if inventory is in item mode open item editor
    if (ItemTableModel* itemModel = dynamic_cast<ItemTableModel*>(model_)) {
        const Item* item = itemModel->getItem(row);
        ItemEditor* editor = new ItemEditor(db_, item, this);

        connect(editor, &BasicEditor::updateView, this, &InventoryView::updateInventoryView);
        editor->exec();

    // if inventory is in item type mode open item type editor
    } else if (ItemTypeTableModel* itemTypeModel = dynamic_cast<ItemTypeTableModel*>(model_)) {
        const ItemType* itemType = itemTypeModel->getItemType(row);
        ItemTypeEditor* editor = new ItemTypeEditor(db_, itemType, this);

        connect(editor, &BasicEditor::updateView, this, &InventoryView::updateInventoryView);
        editor->exec();
    }
}

void InventoryView::removeButtonClicked()
{
    int row = table_->selectedRowIndex();
    int id = model_->getId(row);

    // removes id if row valid (-1 incase of error)
    if (id >= 0) {
        db_.remove(id, mode_);
        updateInventoryView();
    }
}

void InventoryView::editSelected()
{
    int row = table_->selectedRowIndex();
    if (row >= 0) {
        openEditor(row);
    }
}

void InventoryView::selectItem()
{
    int row = table_->selectedRowIndex();

    if (ItemTableModel* itemModel = dynamic_cast<ItemTableModel*>(model_)) {
        const Item* item = itemModel->getItem(row);

        if (item->itemTypeId) {
            bool move = PopUpMessage::confirm(PopUpCode::ITEM_HAS_ITEM_TYPE, this);
            if (!move) { return; }
        }

        emit selectedItem(item->id);
        close();
    }
}