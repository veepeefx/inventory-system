#ifndef INVENTORY_SYSTEM_COMMONENUMS_H
#define INVENTORY_SYSTEM_COMMONENUMS_H

#include <QMap>

enum class SearchMode {
    NAME,
    PRODUCT_NUMBER,
    SELF_LOCATION,
    EAN,
    ID
};

static const QMap<SearchMode, QString> SearchModeLabels {
    { SearchMode::NAME, "Name" },
    { SearchMode::PRODUCT_NUMBER, "Product Number" },
    { SearchMode::SELF_LOCATION, "Self Location"},
    { SearchMode::EAN, "EAN"},
    { SearchMode::ID, "Unique ID"}
};

enum class SearchType {
    STARTS_WITH,
    ENDS_WITH,
    CONTAINS,
    EQUALS
};

static const QMap<SearchType, QString> SearchTypeLabels {
    { SearchType::STARTS_WITH, "Starts" },
    { SearchType::ENDS_WITH, "Ends" },
    { SearchType::CONTAINS, "Contains" },
    { SearchType::EQUALS, "Equals" }
};

struct Search {
    std::vector<std::string> terms;
    std::vector<SearchMode> modes;
    std::vector<SearchType> types;
    std::vector<bool> caseSensitivity;

    bool empty() const { return terms.empty(); }
};

// INVENTORY STATES
enum class InventoryMode {
    ITEM,
    ITEM_TYPE
};

enum class InventoryUse {
    EDITING,
    SELECTING
};

static const QStringList CurrencySymbols {
    "€", "kr", "$", "£", "¥", "₹", "₽"
};

#endif //INVENTORY_SYSTEM_COMMONENUMS_H