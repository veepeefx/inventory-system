#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

#include "inventory/InventoryView.h"
#include "settings/SettingsView.h"

MainWindow::MainWindow(DataBase& db, QWidget *parent)
: QMainWindow(parent)
{
    QWidget *central = new QWidget();
    mainLayout_ = new QGridLayout(central);

    viewStack_ = new QStackedWidget(this);
    viewStack_->addWidget(central);

    initViews(db);
    initMainMenu();

    setCentralWidget(viewStack_);
    setWindowTitle("Inventory System");
    resize(WINDOW_SIZE);
}

MainWindow::~MainWindow() {}

void MainWindow::initViews(DataBase& db)
{
    InventoryView* itemInventory = new InventoryView(
        db, InventoryMode::ITEM, InventoryUse::EDITING, this);
    viewStack_->addWidget(itemInventory);

    InventoryView* itemTypeInventory = new InventoryView(
        db, InventoryMode::ITEM_TYPE, InventoryUse::EDITING, this);
    viewStack_->addWidget(itemTypeInventory);

    SettingsView* settings = new SettingsView();
    viewStack_->addWidget(settings);

    // return feature
    connect(itemInventory, &InventoryView::returnMainMenu, this, &MainWindow::switchViewToMainMenu);
    connect(itemTypeInventory, &InventoryView::returnMainMenu, this, &MainWindow::switchViewToMainMenu);
    connect(settings, &SettingsView::returnMainMenu, this, &MainWindow::switchViewToMainMenu);

}

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
    viewStack_->setCurrentIndex(Views::INVENTORY_ITEMS);
}

void MainWindow::onItemTypeButtonClicked()
{
    viewStack_->setCurrentIndex(Views::INVENTORY_ITEM_TYPES);
}

void MainWindow::onSettingsButtonClicked()
{
    viewStack_->setCurrentIndex(Views::SETTINGS);
}

void MainWindow::onExitButtonClicked()
{
    this->close();
}

void MainWindow::switchViewToMainMenu()
{
    viewStack_->setCurrentIndex(Views::MAIN_MENU);
}