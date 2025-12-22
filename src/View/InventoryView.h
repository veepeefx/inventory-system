#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QComboBox>
#include <QTableView>

#include "ItemTableModel.h"
#include "../database/DataBase.h"
#include "../utils/CommonEnums.h"


class InventoryView : public QWidget {
    Q_OBJECT

public:
    explicit InventoryView(DataBase& db, QWidget *parent = nullptr);

    ~InventoryView() override;

private:
    QVBoxLayout *mainLayout_;

    QTableView* table_;
    ItemTableModel* model_;
    DataBase& db_;

    void initSearchBar();
    void initInventoryControls();
    int selectedRowIndex() const;

    void makeSearch(QLineEdit* search1LineEdit, QLineEdit* search2LineEdit,
                QComboBox* search1Type, QComboBox* search2Type);

signals:
    void returnMainMenu();

private slots:

    void updateInventoryView();
    void openItemEditor(const Item* item = nullptr);

    void removeButtonClicked();
    void editButtonClicked();
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H