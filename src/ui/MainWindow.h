#ifndef INVENTORY_SYSTEM_MAINWINDOW_H
#define INVENTORY_SYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "View.h"
#include "../database/DataBase.h"

enum Views {
    MAIN_MENU,
    INVENTORY_ITEMS,
    INVENTORY_ITEM_TYPES,
    SETTINGS
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(DataBase& db, QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QSize WINDOW_SIZE = {1000, 600};
    QSize BUTTON_SIZE = {200, 75};

    QStackedWidget* viewStack_;
    QGridLayout *mainLayout_;

    // adds to viewStack_ and connects return to main menu
    void addView(View* view);

    void initViews(DataBase& db);
    void initMainMenu();

private slots:
    void viewChanged(int index);
    void setViewIndex(int index);
    void onExitButtonClicked();

};


#endif //INVENTORY_SYSTEM_MAINWINDOW_H