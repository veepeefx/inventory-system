#include "SettingsView.h"

#include <iostream>

#include "../../utils/ui/UiTools.h"
#include "../../utils/ui/SearchFieldWidget.h"
#include "../../utils/Settings.h"
#include "../../SettingsManager.h"
#include "../../utils/ui/PopUpMessage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>


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
    const Settings& settings = SettingsManager::getSettings();
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    QLabel* header = new QLabel("Edit Presets:");
    header->setStyleSheet("font-weight: bold; font-size: 14pt;");
    layout->addWidget(header, row++, 0, 1, 2);

    vatDSB_ = UiTools::newDoubleSpinBox(
        "Preset VAT for item", 0, 1000, 2, 0.25, settings.presetVat, "%", {*layout, row++});

    currencyCB_ = UiTools::newComboBox(
        "Select preferred currency unit", CurrencySymbols, settings.currency, {*layout, row++});

    searchLayout = new QVBoxLayout();
    searchLayout->setAlignment(Qt::AlignTop);

    QSpinBox* numOfSearchFields_ = UiTools::newSpinBox(
        "Number of Search Fields", 1, 4, {*layout, row++});
    connect(numOfSearchFields_, &QSpinBox::valueChanged, this, &SettingsView::updateSearch);
    // setting range here as if we declare it before connect if searchFields.size() == 1
    // updateSearch isn't being called
    numOfSearchFields_->setRange(1, 4);
    numOfSearchFields_->setValue(settings.searchFields.size());
    layout->addLayout(searchLayout, row++, 0, 1, 2);

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
    QPushButton* resetButton = new QPushButton("Reset Settings", this);
    QPushButton* saveButton = new QPushButton("Save", this);

    connect(cancelButton, &QPushButton::clicked, this, &SettingsView::returnMainMenu);
    connect(resetButton, &QPushButton::clicked, this, &SettingsView::resetSettings);
    connect(saveButton, &QPushButton::clicked, this, &SettingsView::saveSettings);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(saveButton);

    mainLayout_->addLayout(buttonLayout);
}

void SettingsView::initSearchField(int index)
{
    SearchFieldWidget* search = new SearchFieldWidget(index, false, this);
    allSearchFields.push_back(search);
    searchLayout->addWidget(search);
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

void SettingsView::resetSettings()
{
    SettingsManager::resetSettings();
    emit returnMainMenu();

    PopUpMessage message(PopUpCode::SETTINGS_RESTART, this);
}

void SettingsView::saveSettings()
{
    Settings s;
    for (const auto& searchField : allSearchFields) {
        s.searchFields.push_back({
            searchField->getSearchMode(),
            searchField->getSearchType(),
            searchField->getCaseSensitivity()
        });
    }

    s.presetVat = vatDSB_->value();
    s.currency = currencyCB_->currentText();

    SettingsManager::saveSettings(s);
    emit returnMainMenu();

    PopUpMessage message(PopUpCode::SETTINGS_RESTART, this);
}
