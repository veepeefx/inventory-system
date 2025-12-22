#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QComboBox>
#include <QTableView>

#include "ItemTableModel.h"
#include "../database/DataBase.h"

enum class SearchMode {
    NAME,
    PRODUCT_NUMBER,
    SELF_LOCATION,
    EAN,
    ID
};

static const QMap<SearchMode, QString> searchModeStrings {
        { SearchMode::NAME, "Name" },
        { SearchMode::PRODUCT_NUMBER, "Product Number" },
        { SearchMode::SELF_LOCATION, "Self Location"},
        { SearchMode::EAN, "EAN"},
        { SearchMode::ID, "Unique ID"}
};

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

    // search widgets
    QLineEdit* search1LineEdit_;
    QLineEdit* search2LineEdit_;
    QComboBox* search1Type_;
    QComboBox* search2Type_;

    void initSearchBar();
    void initInventoryControls();
    int selectedRowIndex() const;

    void makeSearch(QLineEdit* search1LineEdit, QLineEdit* search2LineEdit,
                QComboBox* search1Type, QComboBox* search2Type);
    void sqlSearch(std::string search, SearchMode mode);

signals:
    void returnMainMenu();

private slots:

    void updateInventoryView();
    void openItemEditor(const Item* item = nullptr);

    void removeButtonClicked();
    void editButtonClicked();
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H