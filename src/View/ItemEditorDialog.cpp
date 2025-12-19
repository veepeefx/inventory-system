#include "ItemEditorDialog.h"

#include <iostream>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include "InventoryView.h"


ItemEditorDialog::ItemEditorDialog(DataBase* db, QWidget *parent) : QDialog(parent), db_(db)
{
    mainLayout_ = new QVBoxLayout(this);

    initEditor();
    initControls();

    setWindowTitle("Item Editor");
    resize(800,500);
}

ItemEditorDialog::~ItemEditorDialog() {}

void ItemEditorDialog::initEditor()
{
    QGridLayout *editorLayout = new QGridLayout();

    int row = 0;

    QLabel *idLabel = new QLabel("ID:");
    idLineEdit_ = new QLineEdit(this);
    idLineEdit_->setDisabled(true); // id isn't changable
    editorLayout->addWidget(idLabel, row, 0, 1, 1);
    editorLayout->addWidget(idLineEdit_, row, 1, 1, 1);

    row++;

    QLabel *productNumberLabel = new QLabel("Product Number:", this);
    productNumberLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(productNumberLabel, row, 0, 1, 1);
    editorLayout->addWidget(productNumberLineEdit_, row, 1, 1, 1);

    QLabel *eanLabel = new QLabel("Ean:", this);
    eanLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(eanLabel, row, 2, 1, 1);
    editorLayout->addWidget(eanLineEdit_, row, 3, 1, 1);

    row++;

    QLabel *nameLabel = new QLabel("Name:", this);
    nameLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(nameLabel, row, 0, 1, 1);
    editorLayout->addWidget(nameLineEdit_, row, 1, 1, 3);

    row++;

    QLabel* spacerLabel = new QLabel("", this);
    editorLayout->addWidget(spacerLabel, row, 1, 1, 1);

    row++;

    QLabel* priceLabel = new QLabel("Price:", this);
    priceLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(priceLabel, row, 0, 1, 1);
    editorLayout->addWidget(priceLineEdit_, row, 1, 1, 1);

    QLabel* priceNoVatLabel = new QLabel("Price (excl. VAT):", this);
    priceNoVatLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(priceNoVatLabel, row, 2, 1, 1);
    editorLayout->addWidget(priceNoVatLineEdit_, row, 3, 1, 1);

    QLabel* vatLabel = new QLabel("VAT:", this);
    vatLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(vatLabel, row, 4, 1, 1);
    editorLayout->addWidget(vatLineEdit_, row, 5, 1, 1);

    row++;

    QLabel* discountedPriceLabel = new QLabel("Discounted Price:", this);
    discountedPriceLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(discountedPriceLabel, row, 0, 1, 1);
    editorLayout->addWidget(discountedPriceLineEdit_, row, 1, 1, 1);

    QLabel* discountLabel = new QLabel("Discount:", this);
    discountLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(discountLabel, row, 2, 1, 1);
    editorLayout->addWidget(discountLineEdit_, row, 3, 1, 1);

    row++;

    editorLayout->addWidget(spacerLabel, row, 0, 1, 1);

    row++;

    QLabel* quantityLabel = new QLabel("Quantity:", this);
    quantityLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(quantityLabel, row, 0, 1, 1);
    editorLayout->addWidget(quantityLineEdit_, row, 1, 1, 1);

    row++;

    QLabel* selfLocationLabel = new QLabel("Self Location:", this);
    selfLocationLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(selfLocationLabel, row, 0, 1, 1);
    editorLayout->addWidget(selfLocationLineEdit_, row, 1, 1, 1);

    row++;

    editorLayout->addWidget(spacerLabel, row, 0, 1, 1);

    row++;

    QLabel* descriptionLabel = new QLabel("Description:", this);
    descriptionTextEdit_ = new QTextEdit(this);
    editorLayout->addWidget(descriptionLabel, row, 0, 1, 1);
    editorLayout->addWidget(descriptionTextEdit_, row, 1, 3, 3);

    row += 4;

    QLabel* lastModifiedLabel = new QLabel("Last Modified:", this);
    lastModifiedLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(lastModifiedLabel, row, 0, 1, 1);
    editorLayout->addWidget(lastModifiedLineEdit_, row, 1, 1, 1);

    QLabel* createdLabel = new QLabel("Created:", this);
    createdLineEdit_ = new QLineEdit(this);
    editorLayout->addWidget(createdLabel, row, 2, 1, 1);
    editorLayout->addWidget(createdLineEdit_, row, 3, 1, 1);

    row++;

    editorLayout->addWidget(spacerLabel, row, 0, 1, 1);

    mainLayout_->addLayout(editorLayout);
}

void ItemEditorDialog::initControls()
{
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    QPushButton* saveButton = new QPushButton("Save", this);

    connect(cancelButton, &QPushButton::clicked, this, &ItemEditorDialog::reject);
    connect(saveButton, &QPushButton::clicked, this, &ItemEditorDialog::saveButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout_->addLayout(buttonLayout);
}

void ItemEditorDialog::loadItem(Item* item)
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
    }
}

void ItemEditorDialog::saveButtonClicked()
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

        if (!db_->insertItem(newItem)) {
            // MAKE ERROR POP UP HERE AND ASK TO TRY AGAIN (NAME OR PRODUCT NUMBER REQUIRED)
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

        db_->updateItem(loadedItem->id, updateItem);
    }

    emit itemUpdated();
    accept();
}
