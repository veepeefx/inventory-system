#include "SettingsView.h"

#include <QPushButton>
#include <QVBoxLayout>


SettingsView::SettingsView(QWidget *parent) : QWidget(parent)
{
    mainLayout_ = new QVBoxLayout(this);

    initSettings();
    initControls();
}

SettingsView::~SettingsView() {}

void SettingsView::initSettings()
{
    QHBoxLayout* settingsLayout = new QHBoxLayout;

    QGridLayout* windowSettingsLayout = initWindowSettings();
    QGridLayout* presetSettingsLayout = initPresetSettings();

    settingsLayout->addLayout(windowSettingsLayout);
    settingsLayout->addLayout(presetSettingsLayout);
}

QGridLayout* SettingsView::initWindowSettings()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

    return layout;
}

QGridLayout* SettingsView::initPresetSettings()
{
    QGridLayout* layout = new QGridLayout();
    int row = 0;

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

void SettingsView::saveSettings()
{
    emit returnMainMenu();
}
