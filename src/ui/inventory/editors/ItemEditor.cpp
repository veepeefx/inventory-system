#include "ItemEditor.h"
#include "../../../utils/ui/UiTools.h"
#include "../../../SettingsManager.h"

#include <iostream>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>


ItemEditor::ItemEditor(DataBase& db, const Item* item, QWidget *parent)
: BasicEditor(db, parent), loadedItem_(item)
{
    initEditor();
    initControls();
    connect(this, &BasicEditor::saveData, this, &ItemEditor::save);

    openItem();

    setWindowTitle("Item Editor");
}

ItemEditor::~ItemEditor() {}

void ItemEditor::initEditor()
{
    QGridLayout *layout = new QGridLayout();
    int row = 0;

    idLineEdit_ = UiTools::newLineEdit("ID:", {*layout, row++});
    idLineEdit_->setDisabled(true);     // id cant be changed

    productNumberLineEdit_ = UiTools::newLineEdit("Product Number:", {*layout, row});
    eanLineEdit_ = UiTools::newLineEdit("Ean:", {*layout, row++, 2});

    nameLineEdit_ = UiTools::newLineEdit("Name:", {*layout, row, 0, 3});

    layout->addWidget(new QLabel(""), row++, 0);

    priceLineEdit_ = UiTools::newLineEdit("Price:", {*layout, row});
    priceNoVatLineEdit_ = UiTools::newLineEdit("Price (excl. VAT):", {*layout, row, 2});
    vatLineEdit_ = UiTools::newLineEdit("VAT:", {*layout, row++, 4});
    vatLineEdit_->setText(std::to_string(SettingsManager::getSettings().presetVat).data());

    discountedPriceLineEdit_ = UiTools::newLineEdit("Discounted Price:", {*layout, row});
    discountLineEdit_ = UiTools::newLineEdit("Discount:", {*layout, row++, 2});

    layout->addWidget(new QLabel(""), row++, 0);

    quantityLineEdit_ = UiTools::newLineEdit("Quantity:", {*layout, row++});

    selfLocationLineEdit_ = UiTools::newLineEdit("Self location:", {*layout, row++});

    layout->addWidget(new QLabel(""), row++, 0);

    QLabel* descriptionLabel = new QLabel("Description:", this);
    descriptionTextEdit_ = new QTextEdit(this);
    layout->addWidget(descriptionLabel, row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(descriptionTextEdit_, row++, 1, 1, 3);

    lastModifiedLineEdit_ = UiTools::newLineEdit("Last Modified:", {*layout, row});
    lastModifiedLineEdit_->setDisabled(true);   // updates automatically
    createdLineEdit_ = UiTools::newLineEdit("Created At:", {*layout, row++, 2});
    createdLineEdit_->setDisabled(true);        // updates automatically

    layout->addWidget(new QLabel(""), row, 0);

    mainLayout_->addLayout(layout);
}

void ItemEditor::openItem()
{
    if (loadedItem_ == nullptr) { return; }

    // update all lineEdits in editor to correspond loaded item
    idLineEdit_->setText(QString::number(loadedItem_->id));
    nameLineEdit_->setText(QString::fromStdString(loadedItem_->name));
    productNumberLineEdit_->setText(QString::fromStdString(loadedItem_->productNumber));
    quantityLineEdit_->setText(QString::number(loadedItem_->quantity));
    eanLineEdit_->setText(QString::fromStdString(loadedItem_->ean));
    selfLocationLineEdit_->setText(QString::fromStdString(loadedItem_->selfLocation));
    priceNoVatLineEdit_->setText(QString::number(loadedItem_->priceNoVat));
    vatLineEdit_->setText(QString::number(loadedItem_->vat));
    discountLineEdit_->setText(QString::number(loadedItem_->discount));
    descriptionTextEdit_->setText(QString::fromStdString(loadedItem_->description));

    double price = loadedItem_->priceNoVat * (1 + loadedItem_->vat / 100);
    priceLineEdit_->setText(QString::number(price));

    double discountedPrice = price * (1 - loadedItem_->discount / 100);
    discountedPriceLineEdit_->setText(QString::number(discountedPrice));

    // last modified and creation date
    lastModifiedLineEdit_->setText(QString::fromStdString(loadedItem_->modifiedAt));
    createdLineEdit_->setText(QString::fromStdString(loadedItem_->createdAt));
}

void ItemEditor::save()
{
    // new item
    if (loadedItem_ == nullptr) {

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

        if (loadedItem_->name != nameLineEdit_->text().toStdString()) {
            updateItem.name = nameLineEdit_->text().toStdString();
        }
        if (loadedItem_->productNumber != productNumberLineEdit_->text().toStdString()) {
            updateItem.productNumber = productNumberLineEdit_->text().toStdString();
        }
        if (loadedItem_->quantity != quantityLineEdit_->text().toInt()) {
            updateItem.quantity = quantityLineEdit_->text().toInt();
        }
        if (loadedItem_->ean != eanLineEdit_->text().toStdString()) {
            updateItem.ean = eanLineEdit_->text().toStdString();
        }
        if (loadedItem_->selfLocation != selfLocationLineEdit_->text().toStdString()) {
            updateItem.selfLocation = selfLocationLineEdit_->text().toStdString();
        }
        if (loadedItem_->priceNoVat !=  priceNoVatLineEdit_->text().toDouble()) {
            updateItem.priceNoVat = priceNoVatLineEdit_->text().toDouble();
        }
        if (loadedItem_->vat != vatLineEdit_->text().toDouble()) {
            updateItem.vat = vatLineEdit_->text().toDouble();
        }
        if (loadedItem_->discount != discountLineEdit_->text().toDouble()) {
            updateItem.discount = discountLineEdit_->text().toDouble();
        }
        if (loadedItem_->description != descriptionTextEdit_->toPlainText().toStdString()) {
            updateItem.description = descriptionTextEdit_->toPlainText().toStdString();
        }

        if (!db_.update(updateItem, loadedItem_->id, loadedItem_->name,
                           loadedItem_->productNumber)) {
            // POP UP ALERT IF BOTH NAME AND PRODUCT NUMBER IS MISSING
            return;
        }
    }

    emit updateView();
    accept();
}
