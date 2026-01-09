#include "ItemEditorDialog.h"

#include <iostream>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include "InventoryView.h"


ItemEditorDialog::ItemEditorDialog(DataBase& db, QWidget *parent)
: QDialog(parent), db_(db)
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

    auto addField = [this, editorLayout, &row]
    (const QString& labelText, bool nextRow = true, int col = 0, int colSpan = 1) {
        QLabel* label = new QLabel(labelText, this);
        QLineEdit* lineEdit = new QLineEdit(this);
        editorLayout->addWidget(label, row, col);
        editorLayout->addWidget(lineEdit, row, col + 1, 1, colSpan);
        if (nextRow) { row++; }
        return lineEdit;
    };

    auto addSpacer = [this, editorLayout, &row] () {
        QLabel* label = new QLabel("", this);
        editorLayout->addWidget(label, row, 0);
        row++;
    };

    idLineEdit_ = addField("ID:");
    idLineEdit_->setDisabled(true);     // id cant be changed

    productNumberLineEdit_ = addField("Product Number:", false);
    eanLineEdit_ = addField("Ean:", true, 2);

    nameLineEdit_ = addField("Name:", true, 0, 3);

    addSpacer();

    priceLineEdit_ = addField("Price:", false);
    priceNoVatLineEdit_ = addField("Price (excl. VAT):", false, 2);
    vatLineEdit_ = addField("VAT:", true, 4);

    discountedPriceLineEdit_ = addField("Discounted Price:", false);
    discountLineEdit_ = addField("Discount:", true, 2);

    addSpacer();

    quantityLineEdit_ = addField("Quantity:");

    selfLocationLineEdit_ = addField("Self location:");

    addSpacer();

    QLabel* descriptionLabel = new QLabel("Description:", this);
    descriptionTextEdit_ = new QTextEdit(this);
    editorLayout->addWidget(descriptionLabel, row, 0, 1, 1);
    editorLayout->addWidget(descriptionTextEdit_, row, 1, 3, 3);

    row += 4;

    lastModifiedLineEdit_ = addField("Last Modified:", false);
    lastModifiedLineEdit_->setDisabled(true);   // updates automatically
    createdLineEdit_ = addField("Created At:", true, 2);
    createdLineEdit_->setDisabled(true);        // updates automatically

    addSpacer();

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

void ItemEditorDialog::loadItem(const Item* item)
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

        if (!db_.insertItem(newItem)) {
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
