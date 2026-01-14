#include "BasicEditor.h"

#include <QLabel>
#include <QPushButton>


BasicEditor::BasicEditor(DataBase& db, QWidget *parent)
: QDialog(parent), db_(db)
{
    mainLayout_ = new QVBoxLayout(this);
    resize(EDITOR_SIZE);
}

BasicEditor::~BasicEditor()
{}

void BasicEditor::initControls()
{
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    QPushButton* saveButton = new QPushButton("Save", this);

    connect(cancelButton, &QPushButton::clicked, this, &BasicEditor::reject);
    connect(saveButton, &QPushButton::clicked, this, &BasicEditor::saveButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout_->addLayout(buttonLayout);
}

QLineEdit* BasicEditor::newLineEdit(QGridLayout& layout, const QString &text, int row, int col, int colSpan)
{
    QLabel* label = new QLabel(text);
    QLineEdit* lineEdit = new QLineEdit();

    layout.addWidget(label, row, col);
    layout.addWidget(lineEdit, row, col + 1, 1, colSpan);

    return lineEdit;
}

void BasicEditor::saveButtonClicked()
{
    emit saveData();
}