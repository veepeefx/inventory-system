#include "InventoryView.h"

#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ItemEditorDialog.h"


InventoryView::InventoryView(DataBase* db, QStackedWidget* stack, QWidget *parent)
: QWidget(parent), db_(db)
{
    table_ = new QTableView(this);
    model_ = new ItemTableModel(this);

    model_->setData(db_->getItems());
    table_->setModel(model_);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton("Add");
    QPushButton* removeButton = new QPushButton("Remove");
    QPushButton* editButton = new QPushButton("Edit");
    QPushButton* backButton = new QPushButton("Back");

    connect(addButton, &QPushButton::clicked, this, [this, db]() {
        ItemEditorDialog* dialog = new ItemEditorDialog(db, this);
        connect(dialog, &ItemEditorDialog::itemUpdated, this, &InventoryView::updateInventoryView);
        dialog->exec();
    });

    connect(backButton, &QPushButton::clicked, this, [stack]() {
        stack->setCurrentIndex(0); // index 0 is mainmenu
    });

    controlLayout->addWidget(addButton);
    controlLayout->addWidget(removeButton);
    controlLayout->addWidget(editButton);
    controlLayout->addWidget(backButton);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addWidget(table_);
    mainLayout_->addLayout(controlLayout);
}

InventoryView::~InventoryView() {}

void InventoryView::updateInventoryView()
{
    model_->setData(db_->getItems());
}