#include "InventoryView.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "../../SettingsManager.h"
#include "../../utils/ui/SearchFieldWidget.h"
#include "editors/ItemEditor.h"
#include "editors/ItemTypeEditor.h"
#include "tablemodels/ItemTypeTableModel.h"


InventoryView::InventoryView(DataBase& db, InventoryUse use, QWidget *parent)
: QWidget(parent), db_(db), use_(use)
{
    mainLayout_ = new QVBoxLayout(this);

    initSearchBar();

    table_ = new CustomTableView(this);
    mainLayout_->addWidget(table_);

    initControls();
}

InventoryView::~InventoryView() {}

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

    connect(table_, &QTableView::doubleClicked, this, &InventoryView::select);
    connect(selectButton, &QPushButton::clicked, this, &InventoryView::select);
    connect(cancelButton, &QPushButton::clicked, this, &InventoryView::close);

    layout->addWidget(cancelButton);
    layout->addWidget(selectButton);

    mainLayout_->addLayout(layout);
}

Search InventoryView::getSearch(const std::vector<SearchFieldWidget*>& searchFields)
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

    return search;
}

void InventoryView::editSelected()
{
    int row = table_->selectedRowIndex();

    // edit button only works if something is selected
    if (row >= 0) {
        openEditor(row);
    }
}
