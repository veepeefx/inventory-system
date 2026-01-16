#include "UiTools.h"

#include <QLabel>

QLineEdit* UiTools::newLineEdit(const QString &text, const GridInfo& info)
{
    QLabel* label = new QLabel(text);
    QLineEdit* lineEdit = new QLineEdit();

    info.layout.addWidget(label, info.row, info.col);
    info.layout.addWidget(lineEdit, info.row, info.col + 1, 1, info.colSpan);

    return lineEdit;
}

QSpinBox *UiTools::newSpinBox(const QString &text, int min, int max, const GridInfo& info)
{
    QLabel* label = new QLabel(text);
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setRange(min, max);

    info.layout.addWidget(label, info.row, 0);
    info.layout.addWidget(spinBox, info.row, 1);

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

QDoubleSpinBox *UiTools::newDoubleSpinBox(const QString &text, int min, float step, float val, const GridInfo& info)
{
    QLabel* label = new QLabel(text);
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setMinimum(min);
    doubleSpinBox->setSingleStep(step);
    doubleSpinBox->setValue(val);

    info.layout.addWidget(label, info.row, info.col);
    info.layout.addWidget(doubleSpinBox, info.row, info.col + 1, 1, info.colSpan);

    return doubleSpinBox;
}
