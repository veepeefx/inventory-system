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

    void initSettings();
    QGridLayout* initWindowSettings();
    QGridLayout* initPresetSettings();

    void initControls();

private slots:
    void saveSettings();

signals:
    void returnMainMenu();
};


#endif //INVENTORY_SYSTEM_SETTINGSVIEW_H