#include "ItemEditor.h"

#include <iostream>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>


ItemEditor::ItemEditor(DataBase& db, QWidget *parent)
: BasicEditor(db, parent)
{
    initEditor();
    initControls();
    connect(this, &BasicEditor::saveData, this, &ItemEditor::save);

    setWindowTitle("Item Editor");
}

ItemEditor::~ItemEditor() {}

void ItemEditor::initEditor()
{
    QGridLayout *layout = new QGridLayout();
    int row = 0;

    idLineEdit_ = newLineEdit(*layout, "ID:", row++);
    idLineEdit_->setDisabled(true);     // id cant be changed

    productNumberLineEdit_ = newLineEdit(*layout, "Product Number:", row);
    eanLineEdit_ = newLineEdit(*layout, "Ean:", row++, 2);

    nameLineEdit_ = newLineEdit(*layout, "Name:", row++, 0, 3);

    layout->addWidget(new QLabel(""), row++, 0);

    priceLineEdit_ = newLineEdit(*layout, "Price:", row);
    priceNoVatLineEdit_ = newLineEdit(*layout, "Price (excl. VAT):", row, 2);
    vatLineEdit_ = newLineEdit(*layout, "VAT:", row++, 4);

    discountedPriceLineEdit_ = newLineEdit(*layout, "Discounted Price:", row);
    discountLineEdit_ = newLineEdit(*layout, "Discount:", row++, 2);

    layout->addWidget(new QLabel(""), row++, 0);

    quantityLineEdit_ = newLineEdit(*layout, "Quantity:", row++);

    selfLocationLineEdit_ = newLineEdit(*layout, "Self location:", row++);

    layout->addWidget(new QLabel(""), row++, 0);

    QLabel* descriptionLabel = new QLabel("Description:", this);
    descriptionTextEdit_ = new QTextEdit(this);
    layout->addWidget(descriptionLabel, row, 0, 1, 1);
    layout->addWidget(descriptionTextEdit_, row, 1, 3, 3);

    row += 4;

    lastModifiedLineEdit_ = newLineEdit(*layout, "Last Modified:", row);
    lastModifiedLineEdit_->setDisabled(true);   // updates automatically
    createdLineEdit_ = newLineEdit(*layout, "Created At:", row++, 2);
    createdLineEdit_->setDisabled(true);        // updates automatically

    layout->addWidget(new QLabel(""), row, 0);

    mainLayout_->addLayout(layout);
}

void ItemEditor::loadItem(const Item* item)
{
    // setting loadedItem
    loadedItem = item;

    if (loadedItem != nullptr) {
        idLineEdit_->setText(QString::number(item->id));
        nameLineEdit_->setText(QString::fromStdString(item->name));
        productNumberLineEdit_->setText(QString::fromStdString(item->productNumber));
        quantityLineEdit_->setText(QString::number(item->quantity));
        eanLineEdit_->setText(QString::fromStdString(item->ean));
        selfLocationLineEdit_->setText(QString::fromStdString(item->selfLocation));
        priceNoVatLineEdit_->setText(QString::number(item->priceNoVat));
        vatLineEdit_->setText(QString::number(item->vat));
        discountLineEdit_->setText(QString::number(item->discount));
        descriptionTextEdit_->setText(QString::fromStdString(item->description));

        double price = item->priceNoVat * (1 + item->vat / 100);
        priceLineEdit_->setText(QString::number(price));

        double discountedPrice = price * (1 - item->discount / 100);
        discountedPriceLineEdit_->setText(QString::number(discountedPrice));

        // last modified and creation date
        lastModifiedLineEdit_->setText(QString::fromStdString(item->modifiedAt));
        createdLineEdit_->setText(QString::fromStdString(item->createdAt));
    }
}

void ItemEditor::save()
{
    // new item
    if (loadedItem == nullptr) {

        Item newItem;
        newItem.name = nameLineEdit_->text().toStdString();
        newItem.productNumber = productNumberLineEdit_->text().toStdString();
        newItem.quantity = quantityLineEdit_->text().toInt();
        newItem.ean = eanLineEdit_->text().toStdString();
        newItem.selfLocation = selfLocationLineEdit_->text().toStdString();
        newItem.priceNoVat = priceNoVatLineEdit_->text().toDouble();
        newItem.vat = vatLineEdit_->text().toDouble();
        newItem.discount = discountLineEdit_->text().toDouble();
        newItem.description = descriptionTextEdit_->toPlainText().toStdString();

        if (!db_.insert(newItem)) {
            // POP UP ALERT IF BOTH NAME AND PRODUCT NUMBER IS MISSING
            return;
        }

    // updating item
    } else {

        ItemUpdate updateItem;

        if (loadedItem->name != nameLineEdit_->text().toStdString()) {
            updateItem.name = nameLineEdit_->text().toStdString();
        }
        if (loadedItem->productNumber != productNumberLineEdit_->text().toStdString()) {
            updateItem.productNumber = productNumberLineEdit_->text().toStdString();
        }
        if (loadedItem->quantity != quantityLineEdit_->text().toInt()) {
            updateItem.quantity = quantityLineEdit_->text().toInt();
        }
        if (loadedItem->ean != eanLineEdit_->text().toStdString()) {
            updateItem.ean = eanLineEdit_->text().toStdString();
        }
        if (loadedItem->selfLocation != selfLocationLineEdit_->text().toStdString()) {
            updateItem.selfLocation = selfLocationLineEdit_->text().toStdString();
        }
        if (loadedItem->priceNoVat !=  priceNoVatLineEdit_->text().toDouble()) {
            updateItem.priceNoVat = priceNoVatLineEdit_->text().toDouble();
        }
        if (loadedItem->vat != vatLineEdit_->text().toDouble()) {
            updateItem.vat = vatLineEdit_->text().toDouble();
        }
        if (loadedItem->discount != discountLineEdit_->text().toDouble()) {
            updateItem.discount = discountLineEdit_->text().toDouble();
        }
        if (loadedItem->description != descriptionTextEdit_->toPlainText().toStdString()) {
            updateItem.description = descriptionTextEdit_->toPlainText().toStdString();
        }

        if (!db_.updateItem(updateItem, loadedItem->id, loadedItem->name,
                           loadedItem->productNumber)) {
            // POP UP ALERT IF BOTH NAME AND PRODUCT NUMBER IS MISSING
            return;
        }
    }

    emit itemUpdated();
    accept();
}
