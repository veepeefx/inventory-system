#ifndef INVENTORY_SYSTEM_SETTINGSVIEW_H
#define INVENTORY_SYSTEM_SETTINGSVIEW_H

#include <QWidget>
#include <QVBoxLayout>

class SettingsView : public QWidget {
    Q_OBJECT

public:
    explicit SettingsView(QWidget *parent = nullptr);

    ~SettingsView() override;

private:
    QVBoxLayout* mainLayout_ = nullptr;
    QVBoxLayout* searchFieldPresets = nullptr;


    QMap<QString, QWidget*> settingsMap_;

    void initSettings();
    QGridLayout* initOtherSettings();
    QGridLayout* initPresetSettings();

    void initControls();
    void initSearchField(int index);
    void deleteSearchField(int index);

private slots:
    // dynamically adjusts amount of search field settings
    void updateSearch(int val);
    void saveSettings();


signals:
    void returnMainMenu();
};


#endif //INVENTORY_SYSTEM_SETTINGSVIEW_H