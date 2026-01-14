#ifndef INVENTORY_SYSTEM_ITEMEDITORDIALOG_H
#define INVENTORY_SYSTEM_ITEMEDITORDIALOG_H

#include <QLineEdit>
#include <QTextEdit>

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
    QLineEdit* productNumberLineEdit_;
    QLineEdit* eanLineEdit_;
    QLineEdit* nameLineEdit_;
    QLineEdit* priceLineEdit_;
    QLineEdit* priceNoVatLineEdit_;
    QLineEdit* vatLineEdit_;
    QLineEdit* discountedPriceLineEdit_;
    QLineEdit* discountLineEdit_;
    QLineEdit* quantityLineEdit_;
    QLineEdit* selfLocationLineEdit_;
    QTextEdit* descriptionTextEdit_;
    QLineEdit* lastModifiedLineEdit_;
    QLineEdit* createdLineEdit_;

    void initEditor();
    void openItem();

private slots:
    void save();
};


#endif //INVENTORY_SYSTEM_ITEMEDITORDIALOG_H