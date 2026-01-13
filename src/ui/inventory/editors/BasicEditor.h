#ifndef INVENTORY_SYSTEM_BASICEDITOR_H
#define INVENTORY_SYSTEM_BASICEDITOR_H

#include "../../../database/DataBase.h"

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>

class BasicEditor : public QDialog {
    Q_OBJECT

public:
    BasicEditor(DataBase& db, QWidget *parent = nullptr);

    ~BasicEditor();

protected:
    DataBase& db_;
    QVBoxLayout *mainLayout_ = nullptr;

    void initControls();
    QLineEdit* newLineEdit(QGridLayout& layout, const QString &text, int row, int col = 0, int colSpan = 1);

private slots:
    void saveButtonClicked();

signals:
    void saveData();
    void itemUpdated();

};


#endif //INVENTORY_SYSTEM_BASICEDITOR_H