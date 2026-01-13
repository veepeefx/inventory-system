#include "ItemTypeTableModel.h"

#include "../../database/DataBase.h"


ItemTypeTableModel::ItemTypeTableModel(QWidget *parent)
    : QAbstractTableModel(parent) {}

ItemTypeTableModel::~ItemTypeTableModel() {}

void ItemTypeTableModel::setData(const std::vector<ItemType>& itemTypes)
{
    beginResetModel();
    itemTypes_ = itemTypes;
    endResetModel();
}

int ItemTypeTableModel::rowCount(const QModelIndex&) const
{
    return itemTypes_.size();
}

int ItemTypeTableModel::columnCount(const QModelIndex&) const
{
    return 4;
}

QVariant ItemTypeTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const ItemType& itemType = itemTypes_[index.row()];

    switch (index.column()) {
        case 0: return itemType.typeNumber.data();
        case 1: return itemType.name.data();
        case 2: return itemType.totalQuantity;
        case 3: return itemType.selfLocation.data();
        default: return "NaN";
    }
}

ItemType* ItemTypeTableModel::getItem(const int &row)
{
    return &itemTypes_.at(row);
}

QVariant ItemTypeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return {};
    }

    switch (section) {
        case 0: return "Type Number";
        case 1: return "Type name";
        case 2: return "Quantity";
        case 3: return "Self Location";
        default: return "NaN";
    }
}