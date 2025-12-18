#ifndef INVENTORY_SYSTEM_ITEMEDITORDIALOG_H
#define INVENTORY_SYSTEM_ITEMEDITORDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTextEdit>

#include "../DataBase.h"

class ItemEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ItemEditorDialog(DataBase* db, QWidget *parent = nullptr);

    ~ItemEditorDialog() override;

    void newItem();
    void openItem(Item* item);

private:
    DataBase* db_;
    QVBoxLayout *mainLayout_;
    Item* loadedItem = nullptr;

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
    void initControls();

    void saveItem();
    void cancel();
    void loadItem();

signals:
    void itemUpdated();

private slots:

    void saveButtonClicked();
};


#endif //INVENTORY_SYSTEM_ITEMEDITORDIALOG_H