#include "InventoryView.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "ItemEditorDialog.h"


InventoryView::InventoryView(DataBase* db, QWidget *parent)
: QWidget(parent), db_(db)
{
    table_ = new QTableView(this);
    model_ = new ItemTableModel(this);

    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model_->setData(db_->getItems());
    table_->setModel(model_);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addWidget(table_);

    initInventoryControls();
}

InventoryView::~InventoryView() {}

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

void InventoryView::updateInventoryView()
{
    model_->setData(db_->getItems());
}

void InventoryView::openItemEditor(Item* item)
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
        db_->deleteItem(model_->getItem(row)->id);
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