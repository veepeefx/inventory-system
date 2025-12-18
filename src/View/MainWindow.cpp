#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

#include "InventoryView.h"

MainWindow::MainWindow(DataBase* db, QWidget *parent) : QMainWindow(parent)
{
    db_ = db;

    QWidget *central = new QWidget();

    viewStack_ = new QStackedWidget(this);
    viewStack_->addWidget(central);

    InventoryView* itemInventory = new InventoryView(db_);
    viewStack_->addWidget(itemInventory);

    mainLayout_ = new QGridLayout(central);

    initMainMenu();

    // return feature for InventoryView
    connect(itemInventory, &InventoryView::returnMainMenu, this, &MainWindow::switchViewToMainMenu);

    setCentralWidget(viewStack_);
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

    itemsButton->setFixedSize(BUTTON_SIZE);
    itemTypeButton->setFixedSize(BUTTON_SIZE);
    settingsButton->setFixedSize(BUTTON_SIZE);
    exitButton->setFixedSize(BUTTON_SIZE);

    connect(itemsButton, &QPushButton::clicked, this, &MainWindow::onItemsButtonClicked);
    connect(itemTypeButton, &QPushButton::clicked, this, &MainWindow::onItemTypeButtonClicked);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);

    mainLayout_->addWidget(itemsButton, 1, 1);
    mainLayout_->addWidget(itemTypeButton, 2, 1);
    mainLayout_->addWidget(settingsButton, 3, 1);
    mainLayout_->addWidget(exitButton, 4, 1);
}

void MainWindow::onItemsButtonClicked()
{
    viewStack_->setCurrentIndex(1);
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

void MainWindow::switchViewToMainMenu()
{
    viewStack_->setCurrentIndex(0);
}