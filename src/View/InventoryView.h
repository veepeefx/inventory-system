#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QTableView>

#include "ItemTableModel.h"
#include "../database/DataBase.h"

class InventoryView : public QWidget {
    Q_OBJECT

public:
    explicit InventoryView(DataBase* db, QWidget *parent = nullptr);

    ~InventoryView() override;

private:
    QVBoxLayout *mainLayout_;

    QTableView* table_;
    ItemTableModel* model_;
    DataBase* db_;

    void initInventoryControls();
    int selectedRowIndex() const;

signals:
    void returnMainMenu();

private slots:

    void updateInventoryView();
    void openItemEditor(Item* item = nullptr);

    void removeButtonClicked();
    void editButtonClicked();
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H