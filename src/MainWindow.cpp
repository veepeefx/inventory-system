#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    mainLayout_ = new QGridLayout(central);

    initMainMenu();

    setCentralWidget(central);
    setWindowTitle("Inventory System");
    resize(800, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::initMainMenu()
{
    QPushButton* itemsButton = new QPushButton("Items");
    QPushButton* itemTypeButton = new QPushButton("Item Types");
    QPushButton* settingsButton = new QPushButton("Settings");
    QPushButton* exitButton = new QPushButton("Exit");

    itemsButton->setFixedSize(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT);
    itemTypeButton->setFixedSize(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT);
    settingsButton->setFixedSize(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT);
    exitButton->setFixedSize(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT);

    connect(itemsButton, &QPushButton::clicked, this, &MainWindow::onItemsButtonClicked);
    connect(itemTypeButton, &QPushButton::clicked, this, &MainWindow::onItemsButtonClicked);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);

    mainLayout_->addWidget(itemsButton, 1, 1);
    mainLayout_->addWidget(itemTypeButton, 2, 1);
    mainLayout_->addWidget(settingsButton, 3, 1);
    mainLayout_->addWidget(exitButton, 4, 1);
}

void MainWindow::onItemsButtonClicked()
{

}

void MainWindow::onItemTypeButtonClicked()
{

}

void MainWindow::onSettingsButtonClicked()
{

}

void MainWindow::onExitButtonClicked()
{
    this->close();
}