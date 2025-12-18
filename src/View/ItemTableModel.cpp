#include "ItemTableModel.h"

ItemTableModel::ItemTableModel(QWidget *parent)
    : QAbstractTableModel(parent) {}

ItemTableModel::~ItemTableModel() {}

void ItemTableModel::setData(const std::vector<Item>& items)
{
    beginResetModel();
    items_ = items;
    endResetModel();
}

int ItemTableModel::rowCount(const QModelIndex&) const
{
    return items_.size();
}

int ItemTableModel::columnCount(const QModelIndex&) const
{
    return 8;
}

QVariant ItemTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const Item& item = items_[index.row()];

    const double price = item.priceNoVat * (1 + item.vat/100) * (1 - item.discount/100);

    switch (index.column()) {
        case 0: return item.productNumber.data();
        case 1: return item.name.data();
        case 2: return item.quantity;
        case 3: return item.priceNoVat;
        case 4: return item.vat;
        case 5: return item.discount;
        case 6: return price;
        case 7: return item.selfLocation.data();
    }
    return {};
}

Item* ItemTableModel::getItem(const int& row)
{
    return &items_.at(row);
}

QVariant ItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return {};
    }

    switch (section) {
        case 0: return "Product Number";
        case 1: return "Name";
        case 2: return "Quantity";
        case 3: return "Price w/o vat (€)";
        case 4: return "Vat (%)";
        case 5: return "Discount (%)";
        case 6: return "Price (€)";
        case 7: return "Self Location";
    }

    return {};
}

