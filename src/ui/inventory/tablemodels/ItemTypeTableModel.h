#ifndef INVENTORY_SYSTEM_ITEMTYPETABLEMODEL_H
#define INVENTORY_SYSTEM_ITEMTYPETABLEMODEL_H

#include <QWidget>
#include <QVBoxLayout>

#include "BasicTableModel.h"
#include "../../../utils/ItemType.h"

class DataBase;

class ItemTypeTableModel : public BasicTableModel {
    Q_OBJECT

public:
    explicit ItemTypeTableModel(DataBase& db, QWidget *parent = nullptr);
    ~ItemTypeTableModel() override;

    void loadData() override;
    void loadData(const Search& search) override;
    int getId(int row) override;


    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    ItemType* getType(const int& row);

private:
    std::vector<ItemType> itemTypes_;
};



#endif //INVENTORY_SYSTEM_ITEMTYPETABLEMODEL_H