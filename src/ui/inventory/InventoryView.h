#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include "tablemodels/ItemTableModel.h"
#include "../../database/DataBase.h"
#include "../../utils/ui/SearchFieldWidget.h"
#include "CustomTableView.h"
#include "../View.h"


class InventoryView : public View {
    Q_OBJECT

public:
    explicit InventoryView(DataBase& db, InventoryUse use, QWidget *parent = nullptr);
    ~InventoryView() override;

protected:
    InventoryUse use_;
    DataBase& db_;
    Search lastSearch_ = {};

    QVBoxLayout *mainLayout_ = nullptr;
    CustomTableView* table_ = nullptr;

    // ui initialization functions
    void initSearchBar();
    void initControls();
    void initEditingControls();
    void initSelectingControls();

    void viewShown() override;

    // returns Search struct from ui given by user
    static Search getSearch(const std::vector<SearchFieldWidget*>& searchFields);

    // calls new items to table
    virtual void makeSearch(const std::vector<SearchFieldWidget*>& searchFields) = 0;

signals:
    // returns selected rows id (in selecting mode)
    void selectedId(int id);

    // sets default search parameters
    void clearSearches();

private slots:
    virtual void updateView() = 0;
    virtual void openEditor(int row = -1) = 0;

    // function must emit selectedId and close editor (in selecting mode)
    virtual void select() = 0;

    // for editing mode:
    virtual void removeButtonClicked() = 0;
    void editSelected();

};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H