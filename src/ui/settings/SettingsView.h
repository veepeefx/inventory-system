#ifndef INVENTORY_SYSTEM_SETTINGSVIEW_H
#define INVENTORY_SYSTEM_SETTINGSVIEW_H

#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QWidget>
#include <QVBoxLayout>

#include "../../utils/ui/SearchFieldWidget.h"


class SettingsView : public QWidget {
    Q_OBJECT

public:
    explicit SettingsView(QWidget *parent = nullptr);

    ~SettingsView() override;

private:

    QVBoxLayout* mainLayout_ = nullptr;
    QVBoxLayout* searchLayout = nullptr;

    QVector<QComboBox*> searchTypes;
    QVector<SearchFieldWidget*> allSearchFields;

    QDoubleSpinBox* vatDSB_ = nullptr;


    void initSettings();
    QGridLayout* initOtherSettings();
    QGridLayout* initPresetSettings();

    void initControls();
    void initSearchField(int index);
    void deleteSearchField();

private slots:
    // dynamically adjusts amount of search field settings
    void updateSearch(int val);
    void saveSettings();


signals:
    void returnMainMenu();
};


#endif //INVENTORY_SYSTEM_SETTINGSVIEW_H