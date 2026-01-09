#include "SettingsView.h"
#include <QVBoxLayout>


SettingsView::SettingsView(QWidget *parent) : QWidget(parent)
{
    mainLayout_ = new QVBoxLayout(this);
}

SettingsView::~SettingsView()
{
}