#ifndef INVENTORY_SYSTEM_ITEMEDITORDIALOG_H
#define INVENTORY_SYSTEM_ITEMEDITORDIALOG_H

#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>

#include "BasicEditor.h"
#include "../../../database/DataBase.h"

class ItemEditor : public BasicEditor {
    Q_OBJECT

public:
    explicit ItemEditor(DataBase& db, const Item* item, QWidget *parent = nullptr);
    ~ItemEditor() override;

private:
    const Item *loadedItem_;

    QLineEdit* idLineEdit_;
    QLineEdit* itemTypeidLineEdit_;
    QLineEdit* productNumberLineEdit_;
    QLineEdit* eanLineEdit_;
    QLineEdit* nameLineEdit_;

    QDoubleSpinBox* priceDSB_;
    QDoubleSpinBox* priceNoVatDSB_;
    QDoubleSpinBox* vatDSB_;
    QDoubleSpinBox* discountDSB_;
    QDoubleSpinBox* discountedPriceDSB_;

    QLineEdit* quantityLineEdit_;
    QLineEdit* selfLocationLineEdit_;
    QTextEdit* descriptionTextEdit_;
    QLineEdit* lastModifiedLineEdit_;
    QLineEdit* createdLineEdit_;

    void initEditor();
    void initPrices(QGridLayout& layout, int& row);
    void openItem();

private slots:
    void priceChanged();
    void priceNoVatChanged();
    void vatChanged();
    void discountedPriceChanged();
    void discountChanged();

    void save();
};


#endif //INVENTORY_SYSTEM_ITEMEDITORDIALOG_H