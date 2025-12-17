#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>

#include "ItemTableModel.h"
#include "../DataBase.h"

class InventoryView : public QWidget {
    Q_OBJECT

public:
    explicit InventoryView(DataBase* db, QWidget *parent = nullptr);

    ~InventoryView() override;

private:
    QVBoxLayout *mainLayout_;

    QTableView* table_;
    ItemTableModel* model_;


private
    slots:
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H