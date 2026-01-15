#ifndef INVENTORY_SYSTEM_UITOOLS_H
#define INVENTORY_SYSTEM_UITOOLS_H

#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

class UiTools {
public:

    // makes QLabel and QLineEdit together
    static QLineEdit* newLineEdit(QGridLayout& layout, const QString &text,
                                  int row, int col = 0, int colSpan = 1);

    static QSpinBox* newSpinBox(QGridLayout& layout, const QString &text,
                                int row, int min = 0, int max = 1);

    static QComboBox* newComboBox(QGridLayout& layout, const QString &text,
                                  int row);


private:
    UiTools() = delete;
    ~UiTools() = delete;

};


#endif //INVENTORY_SYSTEM_UITOOLS_H