#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QTableView>

#include "tablemodels/ItemTableModel.h"
#include "../../database/DataBase.h"
#include "../../utils/ui/SearchFieldWidget.h"
#include "CustomTableView.h"


class InventoryView : public QWidget {
    Q_OBJECT

public:
    explicit InventoryView(DataBase& db, InventoryMode mode, InventoryUse use,QWidget *parent = nullptr);

    ~InventoryView() override;

private:
    // inventory states
    InventoryMode mode_;
    InventoryUse use_;

    DataBase& db_;

    QVBoxLayout *mainLayout_;
    CustomTableView* table_ = nullptr;
    BasicTableModel* model_ = nullptr;

    void initTable();
    void initSearchBar();
    void initControls();
    void initEditingControls();
    void initSelectingControls();

    void makeSearch(const std::vector<SearchFieldWidget*>& searchFields);

signals:
    // return button (in editing mode)
    void returnMainMenu();

    // returns selected rows itemid (in selecting mode)
    void selectedItem(int id);

private slots:

    void updateInventoryView();
    void openEditor(int row = -1);

    // for editing mode:
    void removeButtonClicked();
    void editSelected();

    // for selecting mode
    void selectItem();
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H