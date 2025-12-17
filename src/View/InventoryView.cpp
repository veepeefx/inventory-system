#include "InventoryView.h"
#include <QVBoxLayout>


InventoryView::InventoryView(DataBase* db, QWidget *parent) : QWidget(parent)
{
    table_ = new QTableView(this);
    model_ = new ItemTableModel(this);

    model_->setData(db->getItems());
    table_->setModel(model_);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addWidget(table_);
}

InventoryView::~InventoryView() {}