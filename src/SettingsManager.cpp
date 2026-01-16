#include "SettingsManager.h"

#include <iostream>
#include <QFile>
#include <QJsonDocument>

static QString SETTINGS_FILE_PATH = "settings.json";
static Settings settings_;

bool SettingsManager::saveSettings(const Settings& settings)
{
    QFile file(SETTINGS_FILE_PATH);
    if (!file.open(QIODevice::WriteOnly)) { return false; }

    QJsonDocument doc(settings.toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool SettingsManager::loadSettings()
{
    QFile file(SETTINGS_FILE_PATH);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "MAKING FILE";
        resetSettings();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) { return false; }

    settings_ = Settings::fromJson(doc.object());
    return true;
}

const Settings& SettingsManager::getSettings()
{
    return settings_;
}

void SettingsManager::resetSettings()
{
    settings_ = defaultSettings();
    saveSettings(settings_);
}

Settings SettingsManager::defaultSettings()
{
    Settings s;

    s.searchFields.push_back({SearchMode::NAME, SearchType::STARTS_WITH, false});
    s.searchFields.push_back({SearchMode::PRODUCT_NUMBER, SearchType::STARTS_WITH, false});
    s.presetVat = 25.5;

    return s;
}
