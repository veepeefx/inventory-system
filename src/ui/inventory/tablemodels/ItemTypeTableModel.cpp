#include "ItemTypeTableModel.h"

#include "../../../database/DataBase.h"


ItemTypeTableModel::ItemTypeTableModel(DataBase& db, QWidget *parent)
    : BasicTableModel(db, parent) {}

ItemTypeTableModel::~ItemTypeTableModel() {}

void ItemTypeTableModel::loadData(const Search& search)
{
    beginResetModel();

    // if given search terms are empty we handle it like without search
    if (search.empty()) {
        itemTypes_ = db_.getItemTypes();
    } else {
        itemTypes_ = db_.getItemTypes();    // MISSING SEARCH FUNCTIONALITY
    }

    endResetModel();
}

bool ItemTypeTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count <= 0 || row + count > itemTypes_.size()) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    itemTypes_.erase(itemTypes_.begin() + row, itemTypes_.begin() + row + count);
    endRemoveRows();

    return true;
}

int ItemTypeTableModel::getId(int row)
{
    if (row < 0 || row >= itemTypes_.size()) {
        return -1;
    }

    return itemTypes_.at(row).id;
}

ItemType* ItemTypeTableModel::getItemType(const int &row)
{
    if (row < 0 || row >= itemTypes_.size()) {
        return nullptr;
    }

    return &itemTypes_.at(row);
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