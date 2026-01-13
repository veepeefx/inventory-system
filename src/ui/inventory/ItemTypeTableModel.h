#ifndef INVENTORY_SYSTEM_ITEMTYPETABLEMODEL_H
#define INVENTORY_SYSTEM_ITEMTYPETABLEMODEL_H

#include <QAbstractTableModel>
#include <QWidget>
#include <QVBoxLayout>
#include "../../utils/ItemType.h"

class ItemTypeTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ItemTypeTableModel(QWidget *parent = nullptr);
    ~ItemTypeTableModel() override;

    void setData(const std::vector<ItemType>& itemTypes);

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    ItemType* getItem(const int& row);

private:
    std::vector<ItemType> itemTypes_;
};



#endif //INVENTORY_SYSTEM_ITEMTYPETABLEMODEL_H