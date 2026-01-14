#ifndef INVENTORY_SYSTEM_ITEMTABLEMODEL_H
#define INVENTORY_SYSTEM_ITEMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QWidget>
#include <QVBoxLayout>

#include "BasicTableModel.h"
#include "../../../utils/Item.h"

class ItemTableModel : public BasicTableModel {
    Q_OBJECT

public:
    explicit ItemTableModel(DataBase& db, QWidget *parent = nullptr);
    ~ItemTableModel() override;

    void loadData() override;
    void loadData(const Search& search) override;
    int getId(int row) override;
    Item* getItem(const int& row);
    std::vector<int> getItemIds() const;

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<Item> items_;
};


#endif //INVENTORY_SYSTEM_ITEMTABLEMODEL_H