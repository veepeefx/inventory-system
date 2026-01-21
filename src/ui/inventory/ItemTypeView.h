#ifndef INVENTORY_SYSTEM_ITEMTYPEVIEW_H
#define INVENTORY_SYSTEM_ITEMTYPEVIEW_H
#include "InventoryView.h"
#include "tablemodels/ItemTypeTableModel.h"

class ItemTypeView : public InventoryView {
    Q_OBJECT
public:
    ItemTypeView(DataBase& db, InventoryUse use, QWidget* parent);
    ~ItemTypeView();


private:
    ItemTypeTableModel* itemTypeModel_;
    void makeSearch(const std::vector<SearchFieldWidget *> &searchFields) override;

private slots:
    void openEditor(int row = -1) override;
    void updateView() override;

    void removeButtonClicked() override;

    void select() override;

};


#endif //INVENTORY_SYSTEM_ITEMTYPEVIEW_H