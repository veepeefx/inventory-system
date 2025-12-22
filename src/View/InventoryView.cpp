#include "InventoryView.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>

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

    QLabel* search1Label = new QLabel("Parameter 1:");
    QLabel* search2Label = new QLabel("Parameter 2:");

    // multiple search bars add possibility to narrow down search
    QLineEdit* search1LineEdit = new QLineEdit();
    QLineEdit* search2LineEdit = new QLineEdit();
    search1LineEdit_->setFixedWidth(200);
    search2LineEdit_->setFixedWidth(200);

    QComboBox* search1Type = new QComboBox();
    QComboBox* search2Type = new QComboBox();
    search1Type_->setFixedWidth(200);
    search2Type_->setFixedWidth(200);

    for (auto it = searchModeStrings.begin(); it != searchModeStrings.end(); it++) {
        // SearchMode enum -> int
        int key = static_cast<int>(it.key());
        QString text = it.value();

        search1Type_->addItem(text, key);
        search2Type_->addItem(text, key);
    }

    // ADD USER TO HAVE POSSIBILITY TO CUSTOMIZE DEFAULT SEARCH TYPE
    search1Type_->setCurrentIndex(static_cast<int>(SearchMode::NAME));
    search2Type_->setCurrentIndex(static_cast<int>(SearchMode::PRODUCT_NUMBER));

    QPushButton* searchButton = new QPushButton("Search");

    connect(searchButton, &QPushButton::clicked, this,
        [search1LineEdit, search2LineEdit, search1Type, search2Type, this]() {
        makeSearch(search1LineEdit, search2LineEdit, search1Type, search2Type);
    });

    int row = 0;
    searchLayout->addWidget(searchLabel, row, 0, 1, 1);

    row++;

    searchLayout->addWidget(search1Label, row, 0, 1, 1);
    searchLayout->addWidget(search1LineEdit, row, 1, 1, 1);
    searchLayout->addWidget(search1Type, row, 2, 1, 1);

    row++;

    searchLayout->addWidget(search2Label, row, 0, 1, 1);
    searchLayout->addWidget(search2LineEdit, row, 1, 1, 1);
    searchLayout->addWidget(search2Type, row, 2, 1, 1);

    row++;

    searchLayout->addWidget(searchButton, row, 0, 1, 1);

    row++;

    searchLayout->addItem( new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Minimum),
                           row, 3, 1, 1);

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

void InventoryView::makeSearch(QLineEdit* search1LineEdit, QLineEdit* search2LineEdit,
                               QComboBox* search1Type, QComboBox* search2Type)
{
    QString qStr1 = search1LineEdit->text();
    QString qStr2 = search2LineEdit->text();

    if (qStr1.isEmpty() && qStr2.isEmpty()) {
        return;
    }

    SearchMode mode1 = static_cast<SearchMode>(search1Type->currentIndex());
    SearchMode mode2 = static_cast<SearchMode>(search2Type->currentIndex());

    // SEARCH
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
        db_.deleteItem(model_->getItem(row)->id);
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