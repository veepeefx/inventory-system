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
    int column = 0;
    QPushButton* itemsButton = new QPushButton("Items");
    QPushButton* itemTypeButton = new QPushButton("Item Types");
    QPushButton* suppliersButton = new QPushButton("Suppliers");
    connect(itemsButton, &QPushButton::clicked, this, &MainWindow::onItemsButtonClicked);
    connect(itemTypeButton, &QPushButton::clicked, this, &MainWindow::onItemTypeButtonClicked);
    suppliersButton->setEnabled(false);
    suppliersButton->setToolTip("Future feature...");
    mainLayout_->addWidget(itemsButton, 0, column);
    mainLayout_->addWidget(itemTypeButton, 1, column);
    mainLayout_->addWidget(suppliersButton, 2, column);
    column++;

/*  FOR FUTURE FEATURES
    QPushButton* createPurchase = new QPushButton("Create Purchase");
    QPushButton* pastPurchase = new QPushButton("Past Purchases");
    QPushButton* pendingPurchase = new QPushButton("Pending Purchases");

    mainLayout_->addWidget(createPurchase, 0, column);
    mainLayout_->addWidget(pastPurchase, 1, column);
    mainLayout_->addWidget(pendingPurchase, 2, column);
    column++;

    QPushButton* createSale = new QPushButton("Create Sale");
    QPushButton* pastSales = new QPushButton("Past Sales");
    QPushButton* pendingSales = new QPushButton("Pending Sales");

    mainLayout_->addWidget(createSale, 0, column);
    mainLayout_->addWidget(pastSales, 1, column);
    mainLayout_->addWidget(pendingSales, 2, column);
    column++;
*/

    QPushButton* stocktakingButton = new QPushButton("Stocktaking");
    QPushButton* settingsButton = new QPushButton("Settings");
    QPushButton* exitButton = new QPushButton("Exit");
    stocktakingButton->setEnabled(false);
    stocktakingButton->setToolTip("Future feature...");
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);
    mainLayout_->addWidget(stocktakingButton, 0, column);
    mainLayout_->addWidget(settingsButton, 1, column);
    mainLayout_->addWidget(exitButton, 2, column);

    // set all buttons to size
    for (int i = 0; i < mainLayout_->count(); ++i) {
        if (QWidget* widget = mainLayout_->itemAt(i)->widget()) {
            if (QPushButton* button = qobject_cast<QPushButton*>(widget)) {
                button->setFixedSize(BUTTON_SIZE);
            }
        }
    }
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