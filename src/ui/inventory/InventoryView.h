#ifndef INVENTORY_SYSTEM_INVENTORYVIEW_H
#define INVENTORY_SYSTEM_INVENTORYVIEW_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>

#include "tablemodels/ItemTableModel.h"
#include "../../database/DataBase.h"

struct SearchWidgets {
    QLabel* label = nullptr;
    QLineEdit* lineEdit = nullptr;
    QComboBox* modeBox = nullptr;
    QComboBox* typeBox = nullptr;
    QCheckBox* caseSensitivityBox = nullptr;

    void applySizes() const {
        if (lineEdit) { lineEdit->setFixedWidth(200); }
        if (modeBox) { modeBox->setFixedWidth(150); }
        if (typeBox) { typeBox->setFixedWidth(100); }
        if (caseSensitivityBox) { caseSensitivityBox->setFixedWidth(100); }
    }
};

class InventoryView : public QWidget {
    Q_OBJECT

public:
    explicit InventoryView(DataBase& db, InventoryMode mode, QWidget *parent = nullptr);

    ~InventoryView() override;

private:

    // how many search parameters are used in inventoryView
    const int SEARCH_PARAMETER_COUNT = 2;

    QVBoxLayout *mainLayout_;

    QTableView* table_ = nullptr;
    BasicTableModel* model_ = nullptr;
    DataBase& db_;
    InventoryMode mode_;

    void initTable();
    void initSearchBar();
    void initInventoryControls();
    int selectedRowIndex() const;

    void makeSearch(const std::vector<SearchWidgets>& searches);

signals:
    void returnMainMenu();

private slots:

    void updateInventoryView();
    void openEditor(int row = -1);

    void removeButtonClicked();
    void editButtonClicked();
};


#endif //INVENTORY_SYSTEM_INVENTORYVIEW_H