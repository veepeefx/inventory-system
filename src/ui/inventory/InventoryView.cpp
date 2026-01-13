#include "InventoryView.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>

#include "editors/ItemEditor.h"
#include "editors/ItemTypeEditor.h"
#include "tablemodels/ItemTypeTableModel.h"


InventoryView::InventoryView(DataBase& db, InventoryMode mode, QWidget *parent)
: QWidget(parent), db_(db), mode_(mode)
{
    table_ = new QTableView(this);

    switch (mode) {
        case InventoryMode::ITEM:
            model_ = new ItemTableModel(db, this);
            break;
        case InventoryMode::ITEM_TYPE:
            model_ = new ItemTypeTableModel(db, this);
            break;
        default:
            return;
    }

    model_->loadData();
    table_->setModel(model_);

    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

    std::vector<SearchWidgets> searches;
    for (int i = 0; i < SEARCH_PARAMETER_COUNT; i++) {
        SearchWidgets search;
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

        for (const SearchWidgets& s : searches) {
            s.modeBox->addItem(text, key);
        }
    }

    for (auto it = SearchTypeLabels.begin(); it != SearchTypeLabels.end(); it++) {
        int key = static_cast<int>(it.key());
        QString text = it.value();

        for (const SearchWidgets& s : searches) {
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

    for (const SearchWidgets& s : searches) {
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

    connect(addButton, &QPushButton::clicked, this, [this]() { openEditor(); });
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

void InventoryView::makeSearch(const std::vector<SearchWidgets>& searches)
{
    // check if all search bars are empty don't make search rather just show items from id 0->
    bool allEmpty = true;

    for (const SearchWidgets& s : searches) {
        if (!s.lineEdit->text().trimmed().isEmpty()) {
            allEmpty = false;
            break;
        }
    }

    if (allEmpty) {
        model_->loadData();
        return;
    }

    Search search;
    for (const SearchWidgets& s : searches) {
        if (!s.lineEdit->text().isEmpty()) {
            search.terms.push_back(s.lineEdit->text().toStdString());
            search.modes.push_back(static_cast<SearchMode>(s.modeBox->currentIndex()));
            search.types.push_back(static_cast<SearchType>(s.typeBox->currentIndex()));
            search.caseSensitivity.push_back(s.caseSensitivityBox->isChecked());
        }
    }
    model_->loadData(search);
}

void InventoryView::updateInventoryView()
{
    model_->loadData();
}

void InventoryView::openEditor(int row)
{
    BasicEditor* editor;
    switch (mode_) {
        case InventoryMode::ITEM:
            editor = new ItemEditor(db_, this);
            break;
        case InventoryMode::ITEM_TYPE:  editor = new ItemTypeEditor(db_, this);      break;
        default: return;
    }

    connect(editor, &BasicEditor::itemUpdated, this, &InventoryView::updateInventoryView);
    editor->exec();
}

void InventoryView::removeButtonClicked()
{
    int row = selectedRowIndex();
    int id = model_->getId(row);

    // removes id if row valid (-1 incase of error)
    if (id >= 0) {
        db_.remove(id, mode_);
        updateInventoryView();
    }
}

void InventoryView::editButtonClicked()
{
    int row = selectedRowIndex();
    openEditor();
}