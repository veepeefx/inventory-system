#ifndef INVENTORY_SYSTEM_ITEMTABLEMODEL_H
#define INVENTORY_SYSTEM_ITEMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QWidget>
#include <QVBoxLayout>
#include "../Item.h"

class ItemTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ItemTableModel(QWidget *parent = nullptr);
    ~ItemTableModel() override;

    void setData(const std::vector<Item>& items);

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<Item> items_;

};


#endif //INVENTORY_SYSTEM_ITEMTABLEMODEL_H