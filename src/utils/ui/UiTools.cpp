#include "UiTools.h"

#include <QLabel>

QLineEdit* UiTools::newLineEdit(QGridLayout& layout, const QString &text, int row, int col, int colSpan)
{
    QLabel* label = new QLabel(text);
    QLineEdit* lineEdit = new QLineEdit();

    layout.addWidget(label, row, col);
    layout.addWidget(lineEdit, row, col + 1, 1, colSpan);

    return lineEdit;
}

QSpinBox *UiTools::newSpinBox(QGridLayout &layout, const QString &text, int row, int min, int max)
{
    QLabel* label = new QLabel(text);
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setRange(min, max);

    layout.addWidget(label, row, 0);
    layout.addWidget(spinBox, row, 1);

    return spinBox;
}

QComboBox *UiTools::newComboBox(QGridLayout &layout, const QString &text, int row)
{
    QLabel* label = new QLabel(text);
    QComboBox* comboBox = new QComboBox();

    layout.addWidget(label, row, 0);
    layout.addWidget(comboBox, row, 1);

    return comboBox;
}
