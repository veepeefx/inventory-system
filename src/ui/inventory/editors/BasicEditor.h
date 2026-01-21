#ifndef INVENTORY_SYSTEM_BASICEDITOR_H
#define INVENTORY_SYSTEM_BASICEDITOR_H

#include "../../../database/DataBase.h"

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>

class BasicEditor : public QDialog {
    Q_OBJECT

public:
    explicit BasicEditor(DataBase& db, QWidget *parent = nullptr);

    ~BasicEditor();

protected:
    DataBase& db_;
    QVBoxLayout *mainLayout_ = nullptr;

    void initControls();

private:
    const QSize EDITOR_SIZE = {800, 600};

private slots:
    void saveButtonClicked();

signals:
    void saveData();
    void reloadView();

};


#endif //INVENTORY_SYSTEM_BASICEDITOR_H