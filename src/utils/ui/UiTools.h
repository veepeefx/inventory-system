#ifndef INVENTORY_SYSTEM_UITOOLS_H
#define INVENTORY_SYSTEM_UITOOLS_H

#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

struct GridInfo {
    QGridLayout& layout;
    int row;
    int col = 0;
    int colSpan = 1;
};

class UiTools {
public:

    // makes QLabel and QLineEdit together
    static QLineEdit* newLineEdit(const QString &text, const GridInfo& info);

    static QSpinBox* newSpinBox(const QString &text, int min, int max, const GridInfo& info);

    static QComboBox* newComboBox(const QString &text, const QStringList& items,
                                  const QString& val, const GridInfo& info);

    static QDoubleSpinBox* newDoubleSpinBox(const QString &text, int min, int max, int decimals,
                        double step, double val, const QString& suffix, const GridInfo& info);


private:
    UiTools() = delete;
    ~UiTools() = delete;

};


#endif //INVENTORY_SYSTEM_UITOOLS_H