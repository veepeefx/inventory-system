#ifndef INVENTORY_SYSTEM_SETTINGS_H
#define INVENTORY_SYSTEM_SETTINGS_H
#include "CommonEnums.h"
#include <QJsonObject>
#include <QJsonArray>

struct SearchField {
    SearchMode mode;
    SearchType type;
    bool caseSensitive;

    QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["mode"] = static_cast<int>(mode);
        obj["type"] = static_cast<int>(type);
        obj["caseSensitive"] = caseSensitive;
        return obj;
    }

    static SearchField fromJson(const QJsonObject& obj)
    {
        SearchField res;
        res.mode = static_cast<SearchMode>(obj["mode"].toInt());
        res.type = static_cast<SearchType>(obj["type"].toInt());
        res.caseSensitive = obj["caseSensitive"].toBool();
        return res;
    }
};

struct Settings {
    std::vector<SearchField> searchFields;
    double presetVat;

    QJsonObject toJson() const
    {
        QJsonArray arr;
        for (const auto& field : searchFields) {
            arr.append(field.toJson());
        }
        QJsonObject obj;
        obj["searchFields"] = arr;
        obj["presetVat"] = presetVat;
        return obj;
    }

    static Settings fromJson(const QJsonObject& obj)
    {
        Settings res;
        QJsonArray arr = obj["searchFields"].toArray();
        for (const auto& field : arr) {
            res.searchFields.push_back(SearchField::fromJson(field.toObject()));
        }
        res.presetVat = obj["presetVat"].toDouble();
        return res;
    }
};

#endif //INVENTORY_SYSTEM_SETTINGS_H