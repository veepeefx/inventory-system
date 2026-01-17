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

    initPrices(*layout, row);

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

void ItemEditor::initPrices(QGridLayout &layout, int& row)
{
    const Settings settings = SettingsManager::getSettings();
    int minVal = -999999999, maxVal = 999999999;

    priceDSB_ = UiTools::newDoubleSpinBox(
        "Price:", minVal, maxVal, 2, 0.01, 0, settings.currency, {layout, row});

    priceNoVatDSB_ = UiTools::newDoubleSpinBox(
        "Price (excl. VAT):", minVal, maxVal, 2, 0.01, 0, settings.currency, {layout, row, 2});

    vatDSB_ = UiTools::newDoubleSpinBox(
        "VAT:", 0, maxVal, 2, 0.25, settings.presetVat, "%", {layout, row++, 4});

    discountedPriceDSB_ = UiTools::newDoubleSpinBox(
        "Discounted Price:", minVal, maxVal, 2, 0.01, 0, settings.currency, {layout, row});

    discountDSB_ = UiTools::newDoubleSpinBox(
        "Discount:", 0, 100, 2, 0.25, 0, "%", {layout, row++, 2});

    connect(priceDSB_, &QDoubleSpinBox::valueChanged, this, &ItemEditor::priceChanged);
    connect(priceNoVatDSB_, &QDoubleSpinBox::valueChanged, this, &ItemEditor::priceNoVatChanged);
    connect(vatDSB_, &QDoubleSpinBox::valueChanged, this, &ItemEditor::vatChanged);
    connect(discountedPriceDSB_, &QDoubleSpinBox::valueChanged, this, &ItemEditor::discountedPriceChanged);
    connect(discountDSB_, &QDoubleSpinBox::valueChanged, this, &ItemEditor::discountChanged);
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
    descriptionTextEdit_->setText(QString::fromStdString(loadedItem_->description));

    // set prices (others will be calculated automatically)
    vatDSB_->setValue(loadedItem_->vat);
    priceNoVatDSB_->setValue(loadedItem_->priceNoVat);
    discountDSB_->setValue(loadedItem_->discount);

    // last modified and creation date
    lastModifiedLineEdit_->setText(QString::fromStdString(loadedItem_->modifiedAt));
    createdLineEdit_->setText(QString::fromStdString(loadedItem_->createdAt));
}

void ItemEditor::priceChanged()
{
    double priceNoVat = priceDSB_->value() / (1 + vatDSB_->value() / 100.0);
    priceNoVatDSB_->blockSignals(true);
    priceNoVatDSB_->setValue(priceNoVat);
    priceNoVatDSB_->blockSignals(false);

    double discountedPrice = priceDSB_->value() * (1 - discountDSB_->value() / 100.0);
    discountedPriceDSB_->blockSignals(true);
    discountedPriceDSB_->setValue(discountedPrice);
    discountedPriceDSB_->blockSignals(false);
}

void ItemEditor::priceNoVatChanged()
{
    double price = priceNoVatDSB_->value() * (1 + vatDSB_->value() / 100.0);
    priceDSB_->blockSignals(true);
    priceDSB_->setValue(price);
    priceDSB_->blockSignals(false);

    double discountedPrice = priceDSB_->value() * (1 - discountDSB_->value() / 100.0);
    discountedPriceDSB_->blockSignals(true);
    discountedPriceDSB_->setValue(discountedPrice);
    discountedPriceDSB_->blockSignals(false);
}

void ItemEditor::vatChanged()
{
    double price = priceNoVatDSB_->value() * (1 + vatDSB_->value() / 100.0);
    priceDSB_->blockSignals(true);
    priceDSB_->setValue(price);
    priceDSB_->blockSignals(false);

    double discountedPrice = priceDSB_->value() * (1 - discountDSB_->value() / 100.0);
    discountedPriceDSB_->blockSignals(true);
    discountedPriceDSB_->setValue(discountedPrice);
    discountedPriceDSB_->blockSignals(false);
}

void ItemEditor::discountedPriceChanged()
{
    if (discountedPriceDSB_->value() > priceDSB_->value()) {
        discountedPriceDSB_->blockSignals(true);
        discountedPriceDSB_->setValue(priceDSB_->value());
        discountedPriceDSB_->blockSignals(false);

        discountDSB_->blockSignals(true);
        discountDSB_->setValue(0);
        discountDSB_->blockSignals(false);

        return;
    }

    double discount = (priceDSB_->value() - discountedPriceDSB_->value()) / priceDSB_->value() * 100.0;
    discountDSB_->blockSignals(true);
    discountDSB_->setValue(discount);
    discountDSB_->blockSignals(false);
}

void ItemEditor::discountChanged()
{
    double discountedPrice = priceDSB_->value() * (1 - discountDSB_->value() / 100.0);
    discountedPriceDSB_->blockSignals(true);
    discountedPriceDSB_->setValue(discountedPrice);
    discountedPriceDSB_->blockSignals(false);
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

        // only relevant prices (which are saved into db and not just calculated)
        newItem.priceNoVat = priceDSB_->value();
        newItem.vat = vatDSB_->value();
        newItem.discount = discountDSB_->value();

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
        // prices from doubleSpinBoxes
        if (loadedItem_->priceNoVat !=  priceDSB_->value()) {
            updateItem.priceNoVat = priceDSB_->value();
        }
        if (loadedItem_->vat != vatDSB_->value()) {
            updateItem.vat = vatDSB_->value();
        }
        if (loadedItem_->discount != discountDSB_->value()) {
            updateItem.discount = discountDSB_->value();
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
