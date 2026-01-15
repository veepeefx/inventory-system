#ifndef INVENTORY_SYSTEM_SETTINGSMANAGER_H
#define INVENTORY_SYSTEM_SETTINGSMANAGER_H

#include "utils/Settings.h"

class SettingsManager {
public:

    static bool saveSettings(const Settings& settings);
    static bool loadSettings();
    static const Settings& getSettings();
    static void resetSettings();

private:

    static Settings defaultSettings();

    SettingsManager() = delete;
    ~SettingsManager() = delete;
};


#endif //INVENTORY_SYSTEM_SETTINGSMANAGER_H