#include "SettingsView.h"
#include "../../utils/ui/UiTools.h"
#include "../../utils/CommonEnums.h"

#include <QCheckBox>
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
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    QLabel* header = new QLabel("Edit Presets:");
    header->setStyleSheet("font-weight: bold; font-size: 14pt;");
    layout->addWidget(header, row++, 0, 1, 2);

    QSpinBox* numOfFields = UiTools::newSpinBox(*layout, "Number of Search Fields", row++, 1, 4);
    settingsMap_["numOfSearchFields"] = numOfFields;
    connect(numOfFields, &QSpinBox::valueChanged, this, &SettingsView::updateSearch);

    searchFieldPresets = new QVBoxLayout();
    layout->addLayout(searchFieldPresets, row++, 0, 2, 2);

    numOfFields->setValue(2);   // TEMP FOR TESTING

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
    QHBoxLayout* layout = new QHBoxLayout();
    QLabel* search = new QLabel("Parameter " + QString::number(index + 1) + ":");
    QComboBox* modeComboBox = new QComboBox();
    QComboBox* typeComboBox = new QComboBox();
    QCheckBox* caseSensitive = new QCheckBox("Case Sensitivity");

    // fills combo boxes
    for (auto it = SearchModeLabels.begin(); it != SearchModeLabels.end(); it++) {
        int key = static_cast<int>(it.key());
        QString text = it.value();
        modeComboBox->addItem(text, key);
    }
    for (auto it = SearchTypeLabels.begin(); it != SearchTypeLabels.end(); it++) {
        int key = static_cast<int>(it.key());
        QString text = it.value();
        typeComboBox->addItem(text,key);
    }

    layout->addWidget(search);
    layout->addWidget(modeComboBox);
    layout->addWidget(typeComboBox);
    layout->addWidget(caseSensitive);

    searchFieldPresets->setAlignment(Qt::AlignTop);
    searchFieldPresets->addLayout(layout);
}

void SettingsView::deleteSearchField(int index)
{
    QLayoutItem* lastItem = searchFieldPresets->takeAt(index);
    if (lastItem && lastItem->layout()) {
        QLayout* subLayout = lastItem->layout();

        while (QLayoutItem* child = subLayout->takeAt(0)) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
                child->widget()->deleteLater();
            }
        }
        delete subLayout;
    }
}

void SettingsView::updateSearch(int val)
{
    int current = searchFieldPresets->count();
    if (current == val) { return; }

    // creates if too little
    if (current < val) {
        for (int i = current; i < val; i++) {
            initSearchField(i);
        }
    // removes if too much
    } else if (current > val) {
        for(int i = current - 1; i >= val; --i) {
            deleteSearchField(i);
        }
    }
}

void SettingsView::saveSettings()
{
    emit returnMainMenu();
}
