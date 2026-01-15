#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QTableView>

#include "tablemodels/ItemTableModel.h"
#include "../../database/DataBase.h"
#include "../../utils/ui/SearchFieldWidget.h"


class InventoryView : public QWidget {
    Q_OBJECT

public:
    explicit InventoryView(DataBase& db, InventoryMode mode, QWidget *parent = nullptr);

    ~InventoryView() override;

private:

    QVBoxLayout *mainLayout_;

    QTableView* table_ = nullptr;
    BasicTableModel* model_ = nullptr;
    DataBase& db_;
    InventoryMode mode_;

    void initTable();
    void initSearchBar();
    void initInventoryControls();
    int selectedRowIndex() const;

    void makeSearch(const std::vector<SearchFieldWidget*>& searchFields);

signals:
    void returnMainMenu();

private slots:

    void updateInventoryView();
    void openEditor(int row = -1);

    void removeButtonClicked();
    void editButtonClicked();
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H