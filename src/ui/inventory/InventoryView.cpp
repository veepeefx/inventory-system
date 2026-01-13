#include "InventoryView.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>

#include "ItemEditorDialog.h"


InventoryView::InventoryView(DataBase& db, QWidget *parent)
: QWidget(parent), db_(db)
{
    table_ = new QTableView(this);
    model_ = new ItemTableModel(this);

    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model_->setData(db_.getItems());
    table_->setModel(model_);

    mainLayout_ = new QVBoxLayout(this);

    initSearchBar();
    mainLayout_->addWidget(table_);
    initInventoryControls();

    // removing visual focus from table
    table_->clearSelection();
    table_->setCurrentIndex(QModelIndex());
}

InventoryView::~InventoryView() {}

void InventoryView::initSearchBar()
{
    QGridLayout* searchLayout = new QGridLayout();
    searchLayout->setAlignment(Qt::AlignLeft);

    QLabel* searchLabel = new QLabel("Search:");
    searchLabel->setStyleSheet("font-size: 12pt; font-weight: bold;");

    std::vector<Search> searches;
    for (int i = 0; i < SEARCH_PARAMETER_COUNT; i++) {
        Search search;
        search.label = new QLabel("Parameter " + QString::number(i + 1) + ":", this);
        search.lineEdit = new QLineEdit(this);
        search.modeBox = new QComboBox(this);
        search.typeBox = new QComboBox(this);
        search.caseSensitivityBox = new QCheckBox("Case-sensitive", this);
        search.applySizes();

        searches.push_back(search);
    }

    for (auto it = SearchModeLabels.begin(); it != SearchModeLabels.end(); it++) {
        // SearchMode enum -> int
        int key = static_cast<int>(it.key());
        QString text = it.value();

        for (const Search& s : searches) {
            s.modeBox->addItem(text, key);
        }
    }

    for (auto it = SearchTypeLabels.begin(); it != SearchTypeLabels.end(); it++) {
        int key = static_cast<int>(it.key());
        QString text = it.value();

        for (const Search& s : searches) {
            s.typeBox->addItem(text, key);
        }
    }

    QPushButton* searchButton = new QPushButton("Search");

    connect(searchButton, &QPushButton::clicked, this,
        [this, searches]() {
        makeSearch(searches);
    });

    int row = 0;
    searchLayout->addWidget(searchLabel, row, 0, 1, 1);

    row++;

    for (const Search& s : searches) {
        searchLayout->addWidget(s.label, row, 0, 1, 1);
        searchLayout->addWidget(s.typeBox, row, 1, 1, 1);
        searchLayout->addWidget(s.lineEdit, row, 2, 1, 1);
        searchLayout->addWidget(s.modeBox, row, 3, 1, 1);
        searchLayout->addWidget(s.caseSensitivityBox, row, 4, 1, 1);

        row++;
    }

    searchLayout->addWidget(searchButton, row, 0, 1, 1);

    row++;

    searchLayout->addItem( new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Minimum),
                           row, 5, 1, 1);

    mainLayout_->addLayout(searchLayout);
}

void InventoryView::initInventoryControls()
{
    QHBoxLayout* controlLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton("Add");
    QPushButton* removeButton = new QPushButton("Remove");
    QPushButton* editButton = new QPushButton("Edit");
    QPushButton* backButton = new QPushButton("Back");

    connect(addButton, &QPushButton::clicked, this, [this]() { openItemEditor(); });
    connect(removeButton, &QPushButton::clicked, this, &InventoryView::removeButtonClicked);
    connect(editButton, &QPushButton::clicked, this, &InventoryView::editButtonClicked);
    connect(backButton, &QPushButton::clicked, this, [this]() { emit returnMainMenu(); });

    controlLayout->addWidget(addButton);
    controlLayout->addWidget(removeButton);
    controlLayout->addWidget(editButton);
    controlLayout->addWidget(backButton);

    mainLayout_->addLayout(controlLayout);
}

int InventoryView::selectedRowIndex() const
{
    QItemSelectionModel* selection = table_->selectionModel();
    QModelIndexList rows = selection->selectedRows();

    int row;
    if (rows.isEmpty()) {
        row = -1;
    } else {
        row = rows.first().row();
    }

    return row;
}

void InventoryView::makeSearch(const std::vector<Search>& searches)
{
    // check if all search bars are empty don't make search rather just show items from id 0->
    bool allEmpty = true;

    for (const Search& s : searches) {
        if (!s.lineEdit->text().trimmed().isEmpty()) {
            allEmpty = false;
            break;
        }
    }

    if (allEmpty) {
        model_->setData(db_.getItems());
        return;
    }

    std::vector<std::string> vStr;
    std::vector<SearchMode> vMode;
    std::vector<SearchType> vType;
    std::vector<bool> vCaseSensitivity;

    for (const Search& s : searches) {
        if (!s.lineEdit->text().isEmpty()) {
            vStr.push_back(s.lineEdit->text().toStdString());
            vMode.push_back(static_cast<SearchMode>(s.modeBox->currentIndex()));
            vType.push_back(static_cast<SearchType>(s.typeBox->currentIndex()));
            vCaseSensitivity.push_back(s.caseSensitivityBox->isChecked());
        }
    }

    model_->setData(db_.searchItems(vStr, vMode, vType, vCaseSensitivity));
}

void InventoryView::updateInventoryView()
{
    model_->setData(db_.getItems());
}

void InventoryView::openItemEditor(const Item* item)
{
    ItemEditorDialog* dialog = new ItemEditorDialog(db_, this);
    connect(dialog, &ItemEditorDialog::itemUpdated, this, &InventoryView::updateInventoryView);

    // load item to editor
    dialog->loadItem(item);
    dialog->exec();
}

void InventoryView::removeButtonClicked()
{
    int row = selectedRowIndex();

    if (row >= 0) {
        db_.remove(*model_->getItem(row));
        updateInventoryView();
    }
}

void InventoryView::editButtonClicked()
{
    int row = selectedRowIndex();

    if (row >= 0) {
        openItemEditor(model_->getItem(row));
    }
}