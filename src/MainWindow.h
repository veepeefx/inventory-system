#ifndef INVENTORY_SYSTEM_MAINWINDOW_H
#define INVENTORY_SYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QGridLayout *mainLayout_;
    void initMainMenu();

    int BASE_BUTTON_WIDTH = 200;
    int BASE_BUTTON_HEIGHT = 75;

private slots:
    void onItemsButtonClicked();
    void onItemTypeButtonClicked();
    void onSettingsButtonClicked();
    void onExitButtonClicked();
};


#endif //INVENTORY_SYSTEM_MAINWINDOW_H