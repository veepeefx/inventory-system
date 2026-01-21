#ifndef INVENTORY_SYSTEM_MAINWINDOW_H
#define INVENTORY_SYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>

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

    void initViews(DataBase& db);
    void initMainMenu();

private slots:
    void onItemsButtonClicked();
    void onItemTypeButtonClicked();
    void onSuppliersButtonClicked();

    void onStocktakingButtonClicked();
    void onSettingsButtonClicked();
    void onExitButtonClicked();

    void switchViewToMainMenu();
};


#endif //INVENTORY_SYSTEM_MAINWINDOW_H