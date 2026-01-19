#include "ItemTableModel.h"

#include "../../../SettingsManager.h"
#include "../../../database/DataBase.h"

ItemTableModel::ItemTableModel(DataBase& db, QWidget *parent)
    : BasicTableModel(db, parent) {}

ItemTableModel::~ItemTableModel() {}

void ItemTableModel::loadData()
{
    beginResetModel();
    items_ = db_.getItems();
    endResetModel();
}

void ItemTableModel::loadData(const Search& search)
{
    beginResetModel();
    items_ = db_.searchItems(search);
    endResetModel();
}

bool ItemTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row < 0 || count <= 0 || row + count > items_.size()) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    items_.erase(items_.begin() + row, items_.begin() + row + count);
    endRemoveRows();

    return true;
}

int ItemTableModel::getId(int row)
{
    if (row < 0 || row >= items_.size()) {
        return -1;
    }

    return items_.at(row).id;
}

Item* ItemTableModel::getItem(int row)
{
    if (row < 0 || row >= items_.size()) {
        return nullptr;
    }

    return &items_.at(row);
}

std::vector<int> ItemTableModel::getItemIds() const
{
    std::vector<int> vec;
    for (const auto& item : items_) {
        vec.push_back(item.id);
    }
    return vec;
}

void ItemTableModel::addRow(int itemId)
{
    int row = static_cast<int>(items_.size());

    beginInsertRows(QModelIndex(), row, row);
    items_.push_back(db_.getItem(itemId));
    endInsertRows();
}

int ItemTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(items_.size());
}

int ItemTableModel::columnCount(const QModelIndex&) const
{
    return 8;
}

QVariant ItemTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    const Item& item = items_[index.row()];

    if (role == Qt::DisplayRole) {
        const double price = item.priceNoVat * (1 + item.vat/100.0) * (1 - item.discount/100.0);

        QString currency = SettingsManager::getSettings().currency;

        switch (index.column()) {
            case 0: return item.productNumber.data();
            case 1: return item.name.data();
            case 2: return item.quantity;
            case 3: return QString::number(item.priceNoVat) + " " + currency;
            case 4: return QString::number(item.vat) + " %";
            case 5: return QString::number(item.discount) + " %";
            case 6: return QString::number(price) + " " + currency;
            case 7: return item.selfLocation.data();
            default: return "NaN";
        }
    }
    
    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
            case 2: case 4: case 5:
                return {Qt::AlignCenter | Qt::AlignVCenter};
            case 3: case 6:
                return {Qt::AlignRight | Qt::AlignVCenter};
            default:
                return {Qt::AlignLeft | Qt::AlignVCenter};
        }
    }

    return {};
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
        case 3: return "Price w/o VAT";
        case 4: return "VAT";
        case 5: return "Discount";
        case 6: return "Price";
        case 7: return "Self Location";
        default: return "NaN";
    }
}

