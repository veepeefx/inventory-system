#include "Tools.h"

#include <QLabel>

QLineEdit* Tools::newLineEdit(QGridLayout& layout, const QString &text, int row, int col, int colSpan)
{
    QLabel* label = new QLabel(text);
    QLineEdit* lineEdit = new QLineEdit();

    layout.addWidget(label, row, col);
    layout.addWidget(lineEdit, row, col + 1, 1, colSpan);

    return lineEdit;
}
