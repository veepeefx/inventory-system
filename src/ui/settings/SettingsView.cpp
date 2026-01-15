#include "SettingsView.h"
#include "../../utils/ui/UiTools.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>

#include "../../utils/ui/SearchFieldWidget.h"


SettingsView::SettingsView(QWidget *parent) : QWidget(parent)
{
    mainLayout_ = new QVBoxLayout(this);
    QLabel* settingsLabel = new QLabel("Settings:");
    settingsLabel->setStyleSheet("font-weight: bold; font-size: 20pt;");
    mainLayout_->addWidget(settingsLabel);

    initSettings();
    mainLayout_->addStretch(0);
    initControls();
}

SettingsView::~SettingsView() {}

void SettingsView::initSettings()
{
    mainLayout_->addWidget(new QLabel(""));
    QHBoxLayout* settingsLayout = new QHBoxLayout;

    QGridLayout* presetSettingsLayout = initPresetSettings();
    QGridLayout* otherSettingsLayout = initOtherSettings();

    presetSettingsLayout->setAlignment(Qt::AlignTop);
    otherSettingsLayout->setAlignment(Qt::AlignTop);

    settingsLayout->addLayout(presetSettingsLayout);
    settingsLayout->addLayout(otherSettingsLayout);

    settingsLayout->setStretch(0, 1);
    settingsLayout->setStretch(1, 1);

    mainLayout_->addLayout(settingsLayout);
}

QGridLayout* SettingsView::initPresetSettings()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    QLabel* header = new QLabel("Edit Presets:");
    header->setStyleSheet("font-weight: bold; font-size: 14pt;");
    layout->addWidget(header, row++, 0, 1, 2);

    QSpinBox* numOfSearchFields_ = UiTools::newSpinBox(*layout, "Number of Search Fields", row++, 1, 4);
    connect(numOfSearchFields_, &QSpinBox::valueChanged, this, &SettingsView::updateSearch);

    searchLayout = new QVBoxLayout();
    layout->addLayout(searchLayout, row++, 0, 2, 2);

    numOfSearchFields_->setValue(2);   // TEMP FOR TESTING

    return layout;
}

QGridLayout* SettingsView::initOtherSettings()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    QLabel* header = new QLabel("Other Settings:");
    header->setStyleSheet("font-weight: bold; font-size: 14pt;");
    layout->addWidget(header, row, 0, 1, 2);

    return layout;
}

void SettingsView::initControls()
{
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    QPushButton* saveButton = new QPushButton("Save", this);
    saveButton->setEnabled(false);  // settings not implemented yet

    connect(cancelButton, &QPushButton::clicked, this, &SettingsView::returnMainMenu);
    connect(saveButton, &QPushButton::clicked, this, &SettingsView::saveSettings);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout_->addLayout(buttonLayout);
}

void SettingsView::initSearchField(int index)
{
    SearchFieldWidget* search = new SearchFieldWidget(index + 1, false, this);
    allSearchFields.push_back(search);

    searchLayout->addWidget(search);
    searchLayout->setAlignment(Qt::AlignTop);
}

void SettingsView::deleteSearchField()
{
    delete allSearchFields.back();
    allSearchFields.pop_back();
}

void SettingsView::updateSearch(int val)
{
    int current = searchLayout->count();
    if (current == val) { return; }

    // creates if too little
    if (current < val) {
        for (int i = current; i < val; i++) {
            initSearchField(i);
        }
    // removes if too much
    } else if (current > val) {
        for(int i = current - 1; i >= val; --i) {
            deleteSearchField();
        }
    }
}

void SettingsView::saveSettings()
{
    emit returnMainMenu();
}
