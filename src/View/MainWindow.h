#ifndef INVENTORY_SYSTEM_MAINWINDOW_H
#define INVENTORY_SYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "../DataBase.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(DataBase* db, QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    DataBase* db_;

    QStackedWidget* viewStack_;
    QGridLayout *mainLayout_;
    void initMainMenu();

    QSize BUTTON_SIZE = {200, 75};

private slots:
    void onItemsButtonClicked();
    void onItemTypeButtonClicked();
    void onSettingsButtonClicked();
    void onExitButtonClicked();

    void switchViewToMainMenu();
};


#endif //INVENTORY_SYSTEM_MAINWINDOW_H