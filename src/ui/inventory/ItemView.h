#ifndef INVENTORY_SYSTEM_ITEMINVENTORYVIEW_H
#define INVENTORY_SYSTEM_ITEMINVENTORYVIEW_H

#include "InventoryView.h"

class ItemView : public InventoryView {
    Q_OBJECT
public:
    ItemView(DataBase& db, InventoryUse use, QWidget* parent);
    ~ItemView();

private:
    ItemTableModel* itemModel_;
    void makeSearch(const std::vector<SearchFieldWidget *> &searchFields) override;

private slots:
    void openEditor(int row = -1) override;
    void updateView() override;

    void removeButtonClicked() override;

    void select() override;
};


#endif //INVENTORY_SYSTEM_ITEMINVENTORYVIEW_H