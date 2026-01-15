#ifndef INVENTORY_SYSTEM_TOOLS_H
#define INVENTORY_SYSTEM_TOOLS_H
#include <QGridLayout>
#include <QLineEdit>

class Tools {
public:

    // makes QLabel and QLineEdit together
    static QLineEdit* newLineEdit(QGridLayout& layout, const QString &text,
                                  int row, int col = 0, int colSpan = 1);



private:
    Tools() = delete;
    ~Tools() = delete;

};


#endif //INVENTORY_SYSTEM_TOOLS_H