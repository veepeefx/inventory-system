#include "MainWindow.h"

#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>

#include "inventory/InventoryView.h"
#include "inventory/ItemTypeView.h"
#include "inventory/ItemView.h"
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

void MainWindow::addView(View* view)
{
    viewStack_->addWidget(view);
    connect(view, &View::returnMainMenu, this, [this]()
        { setViewIndex(Views::MAIN_MENU); });
}

void MainWindow::initViews(DataBase& db)
{
    ItemView* itemInventory = new ItemView(db, InventoryUse::EDITING, this);
    addView(itemInventory);

    ItemTypeView* itemTypeInventory = new ItemTypeView(db, InventoryUse::EDITING, this);
    addView(itemTypeInventory);

    SettingsView* settings = new SettingsView();
    addView(settings);

    // updates view when shown
    connect(viewStack_, &QStackedWidget::currentChanged, this, &MainWindow::viewChanged);
}

void MainWindow::initMainMenu()
{
    int column = 0;
    QPushButton* itemsButton = new QPushButton("Items");
    QPushButton* itemTypeButton = new QPushButton("Item Types");
    QPushButton* suppliersButton = new QPushButton("Suppliers");

    connect(itemsButton, &QPushButton::clicked, this, [this]()
        { setViewIndex(Views::INVENTORY_ITEMS); });
    connect(itemTypeButton, &QPushButton::clicked, this, [this]()
        { setViewIndex(Views::INVENTORY_ITEM_TYPES); });
    connect(suppliersButton, &QPushButton::clicked, this, [this]()
        { std::cerr << "Not implemented yet" << std::endl; });

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

    connect(stocktakingButton, &QPushButton::clicked, this, [this]()
        { std::cerr << "Not implemented yet" << std::endl; });
    connect(settingsButton, &QPushButton::clicked, this, [this]()
        { setViewIndex(Views::SETTINGS); });
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);

    mainLayout_->addWidget(stocktakingButton, 0, column);
    mainLayout_->addWidget(settingsButton, 1, column);
    mainLayout_->addWidget(exitButton, 2, column);

    // set all buttons to size
    for (int i = 0; i < mainLayout_->count(); ++i) {
        if (QPushButton* button = qobject_cast<QPushButton*>(mainLayout_->itemAt(i)->widget())) {
            button->setFixedSize(BUTTON_SIZE);
        }
    }
}

void MainWindow::viewChanged(int index)
{
    QWidget* widget = viewStack_->widget(index);

    // call viewShown() to update view when shown
    if (View* view = qobject_cast<View*>(widget)) {
        view->viewShown();
    }
}

void MainWindow::setViewIndex(int index)
{
    viewStack_->setCurrentIndex(index);
}

void MainWindow::onExitButtonClicked()
{
    this->close();
}