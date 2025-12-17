#include "ItemEditorDialog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include "InventoryView.h"


ItemEditorDialog::ItemEditorDialog(DataBase* db, QWidget *parent) : QDialog(parent), db_(db)
{
    setWindowTitle("Item Editor");
    mainLayout_ = new QVBoxLayout(this);

    initEditor();

    QPushButton* cancelButton = new QPushButton("Cancel", this);
    QPushButton* saveButton = new QPushButton("Save", this);

    connect(cancelButton, &QPushButton::clicked, this, &ItemEditorDialog::close);
    connect(saveButton, &QPushButton::clicked, this, &ItemEditorDialog::saveButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout_->addLayout(buttonLayout);
    mainLayout_->setAlignment(Qt::AlignCenter);

    resize(800,500);
    QDialog::exec();
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
void ItemEditorDialog::newItem()
{

}

void ItemEditorDialog::loadItem()
{

}

void ItemEditorDialog::saveButtonClicked()
{

}
